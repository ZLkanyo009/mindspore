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

#ifndef MINDSPORE_LITE_TOOLS_OPTIMIZER_GRAPH_TRANSPOSE_STRATEGY_H_
#define MINDSPORE_LITE_TOOLS_OPTIMIZER_GRAPH_TRANSPOSE_STRATEGY_H_

#include <vector>
#include <memory>
#include <string>
#include "schema/inner/model_generated.h"
#include "tools/converter/converter_flags.h"
#include "tools/optimizer/common/format_utils.h"
#include "tools/optimizer/graph/node_infershape.h"

using mindspore::lite::converter::FmkType;
namespace mindspore {
namespace opt {
class TransposeStrategy {
 public:
  TransposeStrategy() = default;
  ~TransposeStrategy() = default;
  void Init(FmkType fmk_type, bool train_flag) {
    fmk_type_ = fmk_type;
    train_flag_ = train_flag;
    node_infer_shape_.Init(fmk_type, train_flag);
  }
  AnfNodePtr TransposePairFuseWhenInsert(const FuncGraphPtr &func_graph, const CNodePtr &code,
                                         const std::vector<int> &perm, bool before, size_t index);
  AnfNodePtr TransposeDependOnShape(const FuncGraphPtr &func_graph, const CNodePtr &cnode, const std::vector<int> &perm,
                                    bool before, size_t index);
  bool CanFusionIfInsert(const FuncGraphPtr &func_graph, const CNodePtr &cnode, TransTypePair *trans_info,
                         TransTypePair *trans_insert_info);
  STATUS ChangeOpAxis(const FuncGraphPtr &func_graph, const CNodePtr &cnode);

 private:
  STATUS TransposeInsertDependOnShape(const FuncGraphPtr &func_graph, const CNodePtr &cnode, bool before, size_t index);
  bool IsInOutCanFuison(const FuncGraphPtr &func_graph, const std::vector<AnfNodePtr> &nodes, size_t *trans_count,
                        FormatTransNodeType *trans_type);
  void DecidePreAndPostTransType(TransTypePair *trans_info, TransTypePair *trans_insert_info);
  STATUS ChangeOpSlice(const FuncGraphPtr &func_graph, const CNodePtr &cnode);
  STATUS ChangeOpStrideSlice(const FuncGraphPtr &func_graph, const CNodePtr &cnode);
  void TransformAttrByAxes(const FuncGraphPtr &func_graph, const CNodePtr &cnode, size_t input_index,
                           const std::vector<int> &axes);
  std::vector<int> TransformOpAxesAttr(const std::vector<int> &origin_axes);
  FmkType fmk_type_{lite::converter::FmkType_MS};
  bool train_flag_{false};
  NodeInferShape node_infer_shape_;
};
}  // namespace opt
}  // namespace mindspore

#endif  // MINDSPORE_LITE_TOOLS_OPTIMIZER_GRAPH_TRANSPOSE_STRATEGY_H_
