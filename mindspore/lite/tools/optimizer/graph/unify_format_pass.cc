/**
 * Copyright 2021 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tools/optimizer/graph/unify_format_pass.h"
#include <unordered_map>
#include <utility>
#include "ops/op_utils.h"
#include "src/common/common.h"
#include "src/common/utils.h"
#include "tools/anf_exporter/anf_exporter.h"

using mindspore::lite::NCHW_SHAPE;
namespace mindspore {
namespace opt {
namespace {
constexpr size_t kNCHWDimNumber = 4;
const std::vector<int> NH2NC = {0, 3, 1, 2};
const std::vector<int> NC2NH = {0, 2, 3, 1};
bool IsSpecialType(const CNodePtr &cnode) {
  if (CheckPrimitiveType(cnode, prim::kPrimTupleGetItem) || CheckPrimitiveType(cnode, prim::kPrimDepend) ||
      CheckPrimitiveType(cnode, prim::kPrimMakeTuple) || CheckPrimitiveType(cnode, kPrimMakeTupleV2) ||
      CheckPrimitiveType(cnode, prim::kPrimReturn)) {
    return true;
  }
  return false;
}
}  // namespace

void UnifyFormatPass::GetTransNodeFormatType(const CNodePtr &cnode, TransTypePair *trans_info) {
  MS_ASSERT(cnode != nullptr);
  auto prim_node = cnode->input(0);
  auto prim = GetValueNode<PrimitivePtr>(prim_node);
  MS_ASSERT(prim != nullptr);
  auto &specify_nhwc_op_map = GetNHWCOpMap();
  auto &specify_nchw_op_map = GetNCHWOpMap();
  if (fmk_type_ == lite::converter::FmkType_TFLITE) {
    if (specify_nchw_op_map.find(prim->name()) == specify_nchw_op_map.end()) {
      return;
    }
    trans_info->pre_ = kNHWC2NCHW;
    trans_info->post_ = kNCHW2NHWC;
  } else if (fmk_type_ == lite::converter::FmkType_TF) {
    if (specify_nhwc_op_map.find(prim->name()) != specify_nhwc_op_map.end() && GetFormat(cnode) == NCHW) {
      trans_info->pre_ = kNCHW2NHWC;
      trans_info->post_ = kNHWC2NCHW;
    }
    if (specify_nchw_op_map.find(prim->name()) != specify_nchw_op_map.end()) {
      trans_info->pre_ = kNHWC2NCHW;
      trans_info->post_ = kNCHW2NHWC;
    }
  } else {
    if (specify_nhwc_op_map.find(prim->name()) != specify_nhwc_op_map.end()) {
      if (fmk_type_ == lite::converter::FmkType_ONNX && prim->GetAttr(ops::kFormat) != nullptr &&
          GetValue<int64_t>(prim->GetAttr(ops::kFormat)) == NHWC) {
        return;
      }
      trans_info->pre_ = kNCHW2NHWC;
      trans_info->post_ = kNHWC2NCHW;
    }
  }
}

bool UnifyFormatPass::TransTransFusion(const FuncGraphPtr &func_graph, const CNodePtr &cnode) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  if (!CheckPrimitiveType(cnode, prim::kPrimTranspose) || !CheckPrimitiveType(cnode->input(1), prim::kPrimTranspose)) {
    return false;
  }
  std::vector<int> post_perm;
  if (GetTransposePerm(cnode->input(2), &post_perm) != lite::RET_OK) {
    MS_LOG(ERROR) << "get tanspose perm failed.";
    return false;
  }
  std::vector<int> pre_perm;
  auto pre_node = cnode->input(1);
  auto pre_cnode = pre_node->cast<CNodePtr>();
  if (pre_cnode == nullptr) {
    return false;
  }
  if (GetTransposePerm(pre_cnode->input(2), &pre_perm) != lite::RET_OK) {
    MS_LOG(ERROR) << "get tanspose perm failed.";
    return false;
  }
  if ((pre_perm == NH2NC && post_perm == NC2NH) || (pre_perm == NC2NH && post_perm == NH2NC)) {
    func_graph->manager()->Replace(cnode, pre_cnode->input(1));
    return true;
  }
  return false;
}

STATUS UnifyFormatPass::PostTransposeFusion(const FuncGraphPtr &func_graph, const CNodePtr &cnode) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  if (!CheckPrimitiveType(cnode, prim::kPrimTranspose)) {
    return lite::RET_OK;
  }
  std::vector<int> cur_perm;
  if (GetTransposePerm(cnode->input(2), &cur_perm) != lite::RET_OK) {
    MS_LOG(ERROR) << "get transpose perm failed.";
    return lite::RET_ERROR;
  }
  auto node_users = func_graph->manager()->node_users()[cnode];
  for (auto &node_user : node_users) {
    auto post_node = node_user.first;
    if (CheckPrimitiveType(post_node, prim::kPrimTranspose)) {
      std::vector<int> post_trans_perm;
      auto post_trans_node = post_node->cast<CNodePtr>();
      if (GetTransposePerm(post_trans_node->input(2), &post_trans_perm) != lite::RET_OK) {
        MS_LOG(ERROR) << "get post transpose node perm failed.";
        return lite::RET_ERROR;
      }
      if ((cur_perm == NH2NC && post_trans_perm == NC2NH) || (cur_perm == NC2NH && post_trans_perm == NH2NC)) {
        func_graph->manager()->Replace(post_node, cnode->input(1));
      }
    }
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::GenNewInput(const FuncGraphPtr &func_graph, const CNodePtr &cnode, std::vector<int> perm,
                                    bool before, size_t index) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  AnfNodePtr new_input = nullptr;
  if (need_reset_) {
    new_input = transpose_strategy_.TransposeDependOnShape(func_graph, cnode, perm, before, index);
  } else {
    new_input = transpose_strategy_.TransposePairFuseWhenInsert(func_graph, cnode, perm, before, index);
  }
  if (new_input == nullptr) {
    MS_LOG(ERROR) << "generate a transpose node failed.";
    return lite::RET_ERROR;
  }
  if (new_input == cnode->input(index) || new_input == cnode) {
    return lite::RET_OK;
  } else if (utils::isa<CNodePtr>(new_input)) {
    auto new_cnode_input = new_input->cast<CNodePtr>();
    int status = lite::RET_OK;
    if (CheckPrimitiveType(new_cnode_input, prim::kPrimTranspose)) {
      if (need_reset_) {
        if (before) {
          pre_insert_trans_.insert(new_cnode_input);
        } else {
          post_insert_trans_.insert(new_cnode_input);
        }
      }
      status = node_infer_shape_.InferShape(new_cnode_input);
    }
    if (status != lite::RET_OK && status != lite::RET_INFER_INVALID) {
      MS_LOG(ERROR) << "infer shape failed.";
      return lite::RET_ERROR;
    }
  }
  auto manager = func_graph->manager();
  MS_ASSERT(manager != nullptr);
  auto tr = manager->Transact();
  if (before) {
    tr.SetEdge(cnode, index, new_input);
    tr.Commit();
  } else {
    func_graph->manager()->Replace(cnode, new_input);
    if (!need_reset_ && PostTransposeFusion(func_graph, new_input->cast<CNodePtr>()) != lite::RET_OK) {
      MS_LOG(ERROR) << "post transpose fusion failed.";
      return lite::RET_ERROR;
    }
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::InsertPreTransNode(const FuncGraphPtr &func_graph, const CNodePtr &cnode,
                                           const std::vector<int> &perm) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  auto prim_node = cnode->input(0);
  auto prim = GetValueNode<PrimitivePtr>(prim_node);
  MS_ASSERT(prim != nullptr);
  auto &specify_nhwc_op_map = GetNHWCOpMap();
  auto &specify_nchw_op_map = GetNCHWOpMap();
  if (specify_nhwc_op_map.find(prim->name()) == specify_nhwc_op_map.end() &&
      specify_nchw_op_map.find(prim->name()) == specify_nchw_op_map.end()) {
    MS_LOG(ERROR) << "op don't meet nhwc condition.";
    return lite::RET_ERROR;
  }
  std::vector<size_t> insert_index = specify_nchw_op_map.find(prim->name()) == specify_nchw_op_map.end()
                                       ? specify_nhwc_op_map.at(prim->name())
                                       : specify_nchw_op_map.at(prim->name());
  if (insert_index.empty()) {
    if (CheckPrimitiveType(cnode, prim::kPrimResizeGrad) && prim->GetAttr(ops::kMethod) != nullptr &&
        GetValue<int64_t>(prim->GetAttr(ops::kMethod)) == static_cast<int64_t>(mindspore::ResizeMethod::NEAREST)) {
      insert_index.push_back(1);
    } else {
      for (size_t i = 1; i < cnode->size(); ++i) {
        insert_index.push_back(i);
      }
    }
  }
  for (auto &index : insert_index) {
    if (GenNewInput(func_graph, cnode, perm, true, index) != lite::RET_OK) {
      MS_LOG(ERROR) << "generate a new input failed.";
      return lite::RET_ERROR;
    }
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::InsertPreTransNode(const FuncGraphPtr &func_graph, const CNodePtr &cnode,
                                           TransTypePair *trans_insert_info) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  MS_ASSERT(trans_insert_info != nullptr);
  TransTypePair trans_info;
  auto origin_inputs = cnode->inputs();
  lite::AnfExporter::RemoveIfMakeTuple(cnode);
  RemoveIfMonad(cnode);
  if (!transpose_strategy_.CanFusionIfInsert(func_graph, cnode, &trans_info, trans_insert_info)) {
    cnode->set_inputs(origin_inputs);
    return lite::RET_NO_CHANGE;
  }
  cnode->set_inputs(origin_inputs);
  auto status = transpose_strategy_.ChangeOpAxis(func_graph, cnode);
  if (status == lite::RET_NOT_SUPPORT) {
    return lite::RET_NO_CHANGE;
  } else if (status != lite::RET_OK) {
    MS_LOG(ERROR) << "change op attr failed.";
    return lite::RET_ERROR;
  }
  auto before_perm = trans_insert_info->pre_ == kNHWC2NCHW ? NH2NC : NC2NH;
  for (size_t i = 1; i < cnode->size(); ++i) {
    if (IsMonadNode(cnode->input(i))) {
      continue;
    }
    if (CheckPrimitiveType(cnode->input(i), prim::kPrimMakeTuple) ||
        CheckPrimitiveType(cnode->input(i), kPrimMakeTupleV2)) {
      auto input_make_tuple = cnode->input(i)->cast<CNodePtr>();
      MS_ASSERT(input_make_tuple != nullptr);
      for (size_t j = 1; j < input_make_tuple->size(); ++j) {
        if (GenNewInput(func_graph, input_make_tuple, before_perm, true, j) != lite::RET_OK) {
          MS_LOG(ERROR) << "generate a new input failed.";
          return lite::RET_ERROR;
        }
      }
      continue;
    }
    if (GenNewInput(func_graph, cnode, before_perm, true, i) != lite::RET_OK) {
      MS_LOG(ERROR) << "generate a new input failed.";
      return lite::RET_ERROR;
    }
  }
  status = node_infer_shape_.InferShape(cnode);
  if (status != lite::RET_OK && status != lite::RET_INFER_INVALID) {
    MS_LOG(ERROR) << "infer shape failed.";
    return lite::RET_ERROR;
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::InsertPostTransNode(const FuncGraphPtr &func_graph, const CNodePtr &cnode,
                                            const std::vector<int> &perm) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  if (!cnode->abstract()->isa<abstract::AbstractTuple>()) {
    if (GenNewInput(func_graph, cnode, perm, false) != lite::RET_OK) {
      MS_LOG(ERROR) << "generate a new input failed.";
      return lite::RET_ERROR;
    }
  } else {
    auto node_users = func_graph->manager()->node_users()[cnode];
    for (auto &node_user : node_users) {
      auto post_node = node_user.first;
      CNodePtr tuple_get_item = nullptr;
      if (!CheckPrimitiveType(post_node, prim::kPrimTupleGetItem)) {
        if (!train_flag_) {
          MS_LOG(ERROR) << "post node is invalid.";
          return lite::RET_ERROR;
        } else {
          tuple_get_item = GenTupleGetItemNode(func_graph, cnode, 0);
          post_node = tuple_get_item;
          func_graph->manager()->Replace(cnode, tuple_get_item);
        }
      }
      if (func_graph->manager()->node_users()[post_node].empty()) {
        continue;
      }
      auto post_cnode = post_node->cast<CNodePtr>();
      if (GenNewInput(func_graph, post_cnode, perm, false) != lite::RET_OK) {
        MS_LOG(ERROR) << "generate a new input failed.";
        return lite::RET_ERROR;
      }
      if (tuple_get_item != nullptr) {
        func_graph->manager()->Replace(tuple_get_item, tuple_get_item->input(1));
      }
    }
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::HandleGraphInput(const FuncGraphPtr &func_graph, const CNodePtr &cnode) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  if (fmk_type_ == lite::converter::FmkType_TF || fmk_type_ == lite::converter::FmkType_TFLITE) {
    return lite::RET_NO_CHANGE;
  }
  for (size_t i = 1; i < cnode->size(); ++i) {
    auto node = cnode->input(i);
    if (!utils::isa<ParameterPtr>(node)) {
      continue;
    }
    auto param_node = node->cast<ParameterPtr>();
    if (param_node->has_default()) {
      continue;
    }
    auto abstract_base = param_node->abstract();
    if (abstract_base == nullptr) {
      MS_LOG(ERROR) << "Abstract of parameter is nullptr, " << param_node->name();
      return lite::RET_ERROR;
    }
    if (!utils::isa<abstract::AbstractTensorPtr>(abstract_base)) {
      MS_LOG(ERROR) << "Abstract of parameter should be anstract tensor, " << param_node->name();
      return lite::RET_ERROR;
    }
    auto abstract_tensor = utils::cast<abstract::AbstractTensorPtr>(abstract_base);
    if (!utils::isa<abstract::ShapePtr>(abstract_tensor->BuildShape())) {
      MS_LOG(ERROR) << "Shape of Abstract of parameter should be ShapePtr, " << param_node->name();
      return lite::RET_ERROR;
    }
    auto shape_vector = utils::cast<abstract::ShapePtr>(abstract_tensor->BuildShape())->shape();
    if (shape_vector.size() != kNCHWDimNumber) {
      continue;
    }
    if (func_graph->get_inputs().size() == 1 && fmk_type_ == lite::converter::FmkType_ONNX && shape_vector[3] == 3 &&
        shape_vector[1] == -1) {
      continue;
    }
    std::vector<int64_t> new_dims = {shape_vector[NCHW_SHAPE::NCHW_N], shape_vector[NCHW_SHAPE::NCHW_H],
                                     shape_vector[NCHW_SHAPE::NCHW_W], shape_vector[NCHW_SHAPE::NCHW_C]};
    abstract_tensor->set_shape(std::make_shared<abstract::Shape>(new_dims));
    auto trans_cnode = GenTransposeNode(func_graph, param_node, NH2NC, param_node->fullname_with_scope() + "_pre");
    if (trans_cnode == nullptr) {
      MS_LOG(ERROR) << "generate a transpose node failed.";
      return lite::RET_ERROR;
    }
    auto status = node_infer_shape_.InferShape(trans_cnode);
    if (status != lite::RET_OK && status != lite::RET_INFER_INVALID) {
      MS_LOG(ERROR) << "infer shape failed.";
      return lite::RET_ERROR;
    }
    func_graph->manager()->Replace(param_node, trans_cnode);
  }
  return lite::RET_OK;
}

STATUS UnifyFormatPass::HandleGraphNode(const FuncGraphPtr &func_graph, const CNodePtr &cnode) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  auto prim_node = cnode->input(0);
  auto prim = GetValueNode<PrimitivePtr>(prim_node);
  MS_ASSERT(prim != nullptr);
  if (prim->GetAttr(kTransDone) != nullptr && GetValue<bool>(prim->GetAttr(kTransDone))) {
    return lite::RET_OK;
  }
  prim->AddAttr(kTransDone, MakeValue<bool>(true));
  TransTypePair trans_info;
  GetTransNodeFormatType(cnode, &trans_info);
  if (!need_reset_ && (trans_info.pre_ == kNONE || trans_info.post_ == kNONE)) {
    if (TransTransFusion(func_graph, cnode)) {
      return lite::RET_OK;
    }
    std::unordered_map<AnfNodePtr, AnfNodePtr> match;
    PreProcessFowardInsert(func_graph, cnode, &match);
    auto status = node_infer_shape_.InferShape(cnode);
    PostProcessFowardInsert(func_graph, cnode, match);
    if (status != lite::RET_OK && status != lite::RET_INFER_INVALID) {
      MS_LOG(ERROR) << "infer shape failed: " << cnode->fullname_with_scope();
      return lite::RET_ERROR;
    }
    return lite::RET_NO_CHANGE;
  }
  auto before_perm = trans_info.pre_ == kNHWC2NCHW ? NH2NC : NC2NH;
  auto after_perm = trans_info.post_ == kNCHW2NHWC ? NC2NH : NH2NC;
  std::unordered_map<AnfNodePtr, AnfNodePtr> match;
  PreProcessFowardInsert(func_graph, cnode, &match);
  if (InsertPreTransNode(func_graph, cnode, before_perm) != lite::RET_OK) {
    MS_LOG(ERROR) << "insert pre node failed." << cnode->fullname_with_scope();
    return lite::RET_ERROR;
  }
  auto status = node_infer_shape_.InferShape(cnode);
  if (status != lite::RET_OK && status != lite::RET_INFER_INVALID) {
    MS_LOG(ERROR) << "infer shape failed.";
    return lite::RET_ERROR;
  }
  PostProcessFowardInsert(func_graph, cnode, match);
  if (InsertPostTransNode(func_graph, cnode, after_perm) != lite::RET_OK) {
    MS_LOG(ERROR) << "insert post node failed." << cnode->fullname_with_scope();
    return lite::RET_ERROR;
  }
  return lite::RET_OK;
}

void UnifyFormatPass::PreProcessFowardInsert(const FuncGraphPtr &func_graph, const CNodePtr &cnode,
                                             std::unordered_map<AnfNodePtr, AnfNodePtr> *match) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  auto graph_name = GetValue<std::string>(func_graph->get_attr("graph_name"));
  if (sub_inputs_map_.find(graph_name) == sub_inputs_map_.end()) {
    return;
  }
  auto manager = func_graph->manager();
  MS_ASSERT(manager != nullptr);
  auto tr = manager->Transact();
  for (size_t i = 1; i < cnode->size(); ++i) {
    if (sub_inputs_map_[graph_name].find(cnode->input(i)) == sub_inputs_map_[graph_name].end()) {
      continue;
    }
    match->insert(std::make_pair(sub_inputs_map_[graph_name][cnode->input(i)], cnode->input(i)));
    tr.SetEdge(cnode, i, sub_inputs_map_[graph_name][cnode->input(i)]);
    tr.Commit();
  }
}

void UnifyFormatPass::PostProcessFowardInsert(const FuncGraphPtr &func_graph, const CNodePtr &cnode,
                                              const std::unordered_map<AnfNodePtr, AnfNodePtr> &match) {
  MS_ASSERT(func_graph != nullptr && cnode != nullptr);
  if (match.empty()) {
    return;
  }
  auto manager = func_graph->manager();
  MS_ASSERT(manager != nullptr);
  auto tr = manager->Transact();
  for (size_t i = 1; i < cnode->size(); ++i) {
    if (match.find(cnode->input(i)) != match.end()) {
      tr.SetEdge(cnode, i, match.at(cnode->input(i)));
      tr.Commit();
    }
    if (CheckPrimitiveType(cnode->input(i), prim::kPrimTranspose)) {
      auto trans_cnode = cnode->input(i)->cast<CNodePtr>();
      for (size_t j = 1; j < trans_cnode->size(); ++j) {
        if (match.find(trans_cnode->input(j)) == match.end()) {
          continue;
        }
        tr.SetEdge(trans_cnode, j, match.at(trans_cnode->input(j)));
        tr.Commit();
      }
    }
  }
}

void UnifyFormatPass::SetSubGraphInput(const CNodePtr &cnode, const FuncGraphPtr &sub_graph) {
  MS_ASSERT(cnode != nullptr && sub_graph != nullptr);
  auto subgraph_name = GetValue<std::string>(sub_graph->get_attr("graph_name"));
  sub_inputs_map_[subgraph_name] = {};
  auto sub_inputs = sub_graph->get_inputs();
  for (auto &node : sub_inputs) {
    auto param_node = node->cast<ParameterPtr>();
    MS_ASSERT(param_node != nullptr);
    auto node_name = node->fullname_with_scope();
    auto last_underline = node_name.find_last_of("_");
    node_name = node_name.substr(0, last_underline);
    last_underline = node_name.find_last_of("_");
    auto index = std::stoi(node_name.substr(last_underline + 1)) + 3;
    if (utils::isa<CNodePtr>(cnode->input(index)) && CheckPrimitiveType(cnode->input(index), prim::kPrimTranspose)) {
      std::vector<int> shape = {-1};
      auto trans_cnode = cnode->input(index)->cast<CNodePtr>();
      MS_ASSERT(trans_cnode != nullptr);
      auto trans_prim = GetValueNode<PrimitivePtr>(trans_cnode->input(0));
      if (trans_prim->GetAttr(kInferDone) != nullptr && GetValue<bool>(trans_prim->GetAttr(kInferDone))) {
        shape = node_infer_shape_.GetInputShape(cnode, index);
      }
      auto type = trans_cnode->abstract()->cast<abstract::AbstractTensorPtr>()->element()->GetTypeTrack();
      std::vector<int64_t> shape_vec(shape.begin(), shape.end());
      param_node->set_abstract(std::make_shared<abstract::AbstractTensor>(type, shape_vec));
    } else {
      sub_inputs_map_[subgraph_name][node] = cnode->input(index);
    }
  }
}

void UnifyFormatPass::SetSubGraphOutput(const CNodePtr &cnode, const FuncGraphPtr &sub_graph) {
  MS_ASSERT(cnode != nullptr && sub_graph != nullptr);
  auto return_node = sub_graph->get_return();
  auto origin_input = return_node->inputs();
  lite::AnfExporter::RemoveIfDepend(return_node);
  lite::AnfExporter::RemoveIfMakeTuple(return_node);
  for (size_t i = 1; i < return_node->size(); ++i) {
    if (!CheckPrimitiveType(return_node->input(i), prim::kPrimTranspose)) {
      continue;
    }
    auto node_name = return_node->input(i)->fullname_with_scope();
    if (node_name.substr(node_name.size() - 5) != "_post") {
      continue;
    }
    auto trans_cnode = return_node->input(i)->cast<CNodePtr>();
    MS_ASSERT(trans_cnode != nullptr);
    auto trans_input = trans_cnode->input(1);
    auto trans_input_name = trans_input->fullname_with_scope();
    if (utils::isa<ParameterPtr>(trans_input)) {
      trans_input->cast<ParameterPtr>()->set_name(node_name);
    } else if (utils::isa<CNodePtr>(trans_input)) {
      trans_input->cast<CNodePtr>()->set_fullname_with_scope(node_name);
    }
    trans_input_name = trans_input_name.substr(0, trans_input_name.find_last_of("_")) + "_cnode";
    trans_cnode->set_fullname_with_scope(trans_input_name);
  }
  return_node->set_inputs(origin_input);
}

void UnifyFormatPass::SetSubGraphAbstract(const CNodePtr &cnode, const FuncGraphPtr &sub_graph) {
  MS_ASSERT(cnode != nullptr && sub_graph != nullptr);
  auto return_node = sub_graph->get_return();
  auto origin_inputs = return_node->inputs();
  lite::AnfExporter::RemoveIfDepend(return_node);
  lite::AnfExporter::RemoveIfMakeTuple(return_node);
  AbstractBasePtrList abstract_list;
  bool infer_done = true;
  for (size_t i = 1; i < return_node->size(); ++i) {
    auto abstract_base = GetCNodeInputAbstract(return_node, i);
    MS_ASSERT(abstract_base != nullptr);
    abstract_list.emplace_back(abstract_base->Clone());
    auto abstract_tensor = abstract_base->cast<abstract::AbstractTensorPtr>();
    MS_ASSERT(abstract_tensor != nullptr);
    auto shape_ptr = utils::cast<abstract::ShapePtr>(abstract_tensor->BuildShape());
    MS_ASSERT(shape_ptr != nullptr);
    auto shape = shape_ptr->shape();
    if (std::find(shape.begin(), shape.end(), -1) != shape.end()) {
      infer_done = false;
    }
    if (utils::isa<CNodePtr>(return_node->input(i))) {
      auto input_cnode = return_node->input(i)->cast<CNodePtr>();
      if (CheckPrimitiveType(input_cnode, prim::kPrimTupleGetItem)) {
        input_cnode = input_cnode->input(1)->cast<CNodePtr>();
      }
      auto input_prim = GetValueNode<PrimitivePtr>(input_cnode->input(0));
      if (input_prim->GetAttr(kInferDone) == nullptr || !GetValue<bool>(input_prim->GetAttr(kInferDone))) {
        infer_done = false;
      }
    }
  }
  return_node->set_inputs(origin_inputs);
  if (utils::isa<abstract::AbstractTuplePtr>(cnode->abstract())) {
    cnode->set_abstract(std::make_shared<abstract::AbstractTuple>(abstract_list));
  } else {
    if (abstract_list.size() != 1) {
      MS_LOG(ERROR) << "cnode output is invalid.";
    }
    cnode->set_abstract(abstract_list.front());
  }
  auto prim = GetValueNode<PrimitivePtr>(cnode->input(0));
  prim->AddAttr(kInferDone, MakeValue<bool>(infer_done));
}

bool UnifyFormatPass::BasicProcess(const FuncGraphPtr &func_graph, bool main_graph) {
  MS_ASSERT(func_graph != nullptr);
  auto graph_name = GetValue<std::string>(func_graph->get_attr("graph_name"));
  auto manager = Manage(func_graph, true);
  if (manager == nullptr) {
    MS_LOG(ERROR) << "manager is nullptr.";
    return false;
  }
  auto node_list = TopoSort(func_graph->get_return());
  int status;
  for (auto &node : node_list) {
    if (!utils::isa<CNodePtr>(node)) {
      continue;
    }
    auto cnode = node->cast<CNodePtr>();
    if (IsSpecialType(cnode)) {
      continue;
    }
    if (main_graph && !need_reset_) {
      status = HandleGraphInput(func_graph, cnode);
      if (status != lite::RET_OK && status != lite::RET_NO_CHANGE) {
        return false;
      }
    }
    if (CheckPrimitiveType(node, prim::kPrimIf) || CheckPrimitiveType(node, prim::kPrimWhile)) {
      auto origin_inputs = cnode->inputs();
      for (size_t i = 3; i < cnode->size(); ++i) {
        if (sub_inputs_map_.find(graph_name) != sub_inputs_map_.end() &&
            sub_inputs_map_[graph_name].find(cnode->input(i)) != sub_inputs_map_[graph_name].end()) {
          cnode->set_input(i, sub_inputs_map_[graph_name][cnode->input(i)]);
        }
      }
      auto sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(1));
      if (sub_func_graph == nullptr) {
        lite::ReturnCode::GetSingleReturnCode()->UpdateReturnCode(lite::RET_NULL_PTR);
        return false;
      }
      SetSubGraphInput(cnode, sub_func_graph);
      (void)BasicProcess(sub_func_graph, false);
      SetSubGraphOutput(cnode, sub_func_graph);
      sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(2));
      if (sub_func_graph == nullptr) {
        lite::ReturnCode::GetSingleReturnCode()->UpdateReturnCode(lite::RET_NULL_PTR);
        return false;
      }
      SetSubGraphInput(cnode, sub_func_graph);
      (void)BasicProcess(sub_func_graph, false);
      SetSubGraphOutput(cnode, sub_func_graph);
      SetSubGraphAbstract(cnode, sub_func_graph);
      cnode->set_inputs(origin_inputs);
      continue;
    }
    status = HandleGraphNode(func_graph, cnode);
    if (status != lite::RET_OK && status != lite::RET_NO_CHANGE) {
      return false;
    }
  }
  return true;
}

bool UnifyFormatPass::DecreaseTransposeForSingleOp(const FuncGraphPtr &func_graph) {
  MS_ASSERT(func_graph != nullptr);
  auto graph_name = GetValue<std::string>(func_graph->get_attr("graph_name"));
  auto manager = Manage(func_graph, true);
  if (manager == nullptr) {
    MS_LOG(ERROR) << "manager is nullptr.";
    return false;
  }
  auto node_list = TopoSort(func_graph->get_return());
  int status;
  for (auto &node : node_list) {
    if (!utils::isa<CNodePtr>(node)) {
      continue;
    }
    auto cnode = node->cast<CNodePtr>();
    if (IsSpecialType(cnode)) {
      continue;
    }
    if (CheckPrimitiveType(node, prim::kPrimIf) || CheckPrimitiveType(node, prim::kPrimWhile)) {
      auto origin_inputs = cnode->inputs();
      for (size_t i = 3; i < cnode->size(); ++i) {
        if (sub_inputs_map_.find(graph_name) != sub_inputs_map_.end() &&
            sub_inputs_map_[graph_name].find(cnode->input(i)) != sub_inputs_map_[graph_name].end()) {
          cnode->set_input(i, sub_inputs_map_[graph_name][cnode->input(i)]);
        }
      }
      auto sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(1));
      if (sub_func_graph == nullptr) {
        lite::ReturnCode::GetSingleReturnCode()->UpdateReturnCode(lite::RET_NULL_PTR);
        return false;
      }
      SetSubGraphInput(cnode, sub_func_graph);
      (void)DecreaseTransposeForSingleOp(sub_func_graph);
      SetSubGraphOutput(cnode, sub_func_graph);
      sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(2));
      if (sub_func_graph == nullptr) {
        lite::ReturnCode::GetSingleReturnCode()->UpdateReturnCode(lite::RET_NULL_PTR);
        return false;
      }
      SetSubGraphInput(cnode, sub_func_graph);
      (void)DecreaseTransposeForSingleOp(sub_func_graph);
      SetSubGraphOutput(cnode, sub_func_graph);
      SetSubGraphAbstract(cnode, sub_func_graph);
      cnode->set_inputs(origin_inputs);
      continue;
    }
    auto prim = GetValueNode<PrimitivePtr>(cnode->input(0));
    MS_ASSERT(prim != nullptr);
    if (!lite::IsContain(GetDynamicFormatOpList(), prim->name())) {
      continue;
    }
    TransTypePair trans_insert_info;
    status = InsertPreTransNode(func_graph, cnode, &trans_insert_info);
    if (status == lite::RET_NO_CHANGE) {
      continue;
    } else if (status != lite::RET_OK) {
      MS_LOG(ERROR) << "insert pre node failed.";
      return false;
    }
    auto after_perm = trans_insert_info.post_ == kNHWC2NCHW ? NH2NC : NC2NH;
    if (InsertPostTransNode(func_graph, cnode, after_perm) != lite::RET_OK) {
      MS_LOG(ERROR) << "insert post node failed." << cnode->fullname_with_scope();
      return false;
    }
  }
  return true;
}

bool UnifyFormatPass::ResetFuncGraph(const FuncGraphPtr &func_graph) {
  MS_ASSERT(func_graph != nullptr);
  auto manager = Manage(func_graph, true);
  if (manager == nullptr) {
    MS_LOG(ERROR) << "manager is nullptr.";
    return false;
  }
  auto node_list = TopoSort(func_graph->get_return());
  for (auto &node : node_list) {
    if (!utils::isa<CNodePtr>(node)) {
      continue;
    }
    auto cnode = node->cast<CNodePtr>();
    auto prim = GetValueNode<PrimitivePtr>(cnode->input(0));
    if (prim->GetAttr(kInferDone) != nullptr) {
      prim->EraseAttr(kInferDone);
    }
    if (prim->GetAttr(kTransDone) != nullptr) {
      prim->EraseAttr(kTransDone);
    }
    if (pre_insert_trans_.find(cnode) != pre_insert_trans_.end()) {
      manager->Replace(node, cnode->input(1));
    }
    if (post_insert_trans_.find(cnode) != post_insert_trans_.end()) {
      auto cnode_abstract = cnode->abstract();
      if (!utils::isa<abstract::AbstractTensorPtr>(cnode_abstract)) {
        MS_LOG(ERROR) << "abstract is not abstract tensor.";
        return false;
      }
      auto cnode_abstract_tensor = cnode_abstract->cast<abstract::AbstractTensorPtr>();
      if (!utils::isa<abstract::ShapePtr>(cnode_abstract_tensor->BuildShape())) {
        MS_LOG(ERROR) << "shape of abstract tensor should be ShapePtr.";
        return false;
      }
      auto shape_ptr = utils::cast<abstract::ShapePtr>(cnode_abstract_tensor->BuildShape());
      auto input_abstract = GetCNodeInputAbstract(cnode, 1);
      if (!utils::isa<abstract::AbstractTensorPtr>(input_abstract)) {
        MS_LOG(ERROR) << "abstract is not abstract tensor.";
        return false;
      }
      auto input_abstract_tensor = input_abstract->cast<abstract::AbstractTensorPtr>();
      input_abstract_tensor->set_shape(shape_ptr);
      manager->Replace(node, cnode->input(1));
    }
    if (CheckPrimitiveType(node, prim::kPrimIf) || CheckPrimitiveType(node, prim::kPrimWhile)) {
      auto sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(1));
      if (sub_func_graph == nullptr) {
        return false;
      }
      (void)ResetFuncGraph(sub_func_graph);
      sub_func_graph = GetValueNode<FuncGraphPtr>(cnode->input(2));
      if (sub_func_graph == nullptr) {
        return false;
      }
      (void)ResetFuncGraph(sub_func_graph);
    }
  }
  return true;
}

bool UnifyFormatPass::RunOnlyForShape(const FuncGraphPtr &func_graph) {
  MS_ASSERT(func_graph != nullptr);
  need_reset_ = true;
  // insert transpose for some ops whose format must be NHWC, which is depend on framework.
  // In this process, transpose op cannot be fused to restore the original graph.
  if (!BasicProcess(func_graph, true)) {
    MS_LOG(ERROR) << "run framework transpose unify failed.";
    return false;
  }
  // delete insert transpose op and update op output shape.
  if (!ResetFuncGraph(func_graph)) {
    MS_LOG(ERROR) << "reset func_graph failed.";
    return false;
  }
  return true;
}

bool UnifyFormatPass::Run(const FuncGraphPtr &func_graph) {
  MS_ASSERT(func_graph != nullptr);
  auto node_list = TopoSort(func_graph->get_return());
  for (auto &node : node_list) {
    auto prim = GetValueNode<PrimitivePtr>(node);
    if (prim == nullptr) {
      continue;
    }
    if (prim->GetAttr(kTransDone) != nullptr) {
      return true;
    }
  }
  // insert transpose for some ops whose format must be NHWC, which is depend on framework.
  // In this process, tranpose can be fused, which the original graph may not be able to restored.
  if (!BasicProcess(func_graph, true)) {
    MS_LOG(ERROR) << "run framework transpose unify failed.";
    return false;
  }
  // if input's format of a certain op can be NHWC, can try transform this op to decrease the number of transpose op.
  if (!DecreaseTransposeForSingleOp(func_graph)) {
    MS_LOG(ERROR) << "run local trans insert optimizer failed.";
    return false;
  }
  return true;
}
}  // namespace opt
}  // namespace mindspore
