/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#include "tools/converter/legacy_optimizer/graph/dtype_trans_pass.h"
#include <string>
#include "tools/common/converter_op_utils.h"
#include "tools/common/node_util.h"
#include "src/common/common.h"
#include "src/common/utils.h"

namespace mindspore {
namespace lite {
#define kMinInputNum 1
#define kOutputNum 1

STATUS DTypeTransPass::Run(schema::MetaGraphT *graph) {
  MS_ASSERT(graph != nullptr);

  auto status = DoModelInputDTypeTrans(graph);
  if (status != RET_OK) {
    MS_LOG(ERROR) << "DoModelInputDTypeTrans error: " << status;
    return status;
  }

  status = DoModelOutputDTypeTrans(graph);
  if (status != RET_OK) {
    MS_LOG(ERROR) << "DoModelOutputDTypeTrans error: " << status;
    return status;
  }

  status = DoNodeInoutDTypeTrans(graph);
  if (status != RET_OK) {
    MS_LOG(ERROR) << "DoNodeInoutDTypeTrans error: " << status;
    return status;
  }
  return RET_OK;
}

STATUS DTypeTransPass::DoModelInputDTypeTrans(schema::MetaGraphT *graph) {
  MS_ASSERT(graph != nullptr);
  // modify inputTensor first
  auto &graphInIdxes = graph->inputIndex;
  for (auto graphInIdx : graphInIdxes) {
    MS_ASSERT(graph->allTensors.size() > graphInIdx);
    auto &graphInTensor = graph->allTensors.at(graphInIdx);
    graphInTensor->dataType = TypeId::kNumberTypeInt8;
  }

  if (this->inputDataDType == TypeId::kNumberTypeInt8) {
    return RET_OK;
  }
  if (this->inputDataDType != TypeId::kNumberTypeFloat && this->inputDataDType != TypeId::kNumberTypeUInt8) {
    MS_LOG(ERROR) << "Invalid inputDataType: " << this->inputDataDType;
    return RET_ERROR;
  }
  // insert fp2int8 node
  for (auto graphInIdx : graphInIdxes) {
    MS_ASSERT(graphInIdx < graph->allTensors.size());
    auto &tensor = graph->allTensors.at(graphInIdx);
    if (tensor->dims.size() != kNHWCDimNumber) {
      continue;
    }

    for (auto iter = graph->nodes.begin(); iter != graph->nodes.end(); iter++) {
      auto &node = *iter;
      auto nodeName = node->name;
      for (size_t inputIndexIdx = 0; inputIndexIdx < node->inputIndex.size(); inputIndexIdx++) {
        if (node->inputIndex.at(inputIndexIdx) == graphInIdx) {
          STATUS status = RET_OK;

          // insert dtype cast node between input tensor and input node
          if (inputDataDType == TypeId::kNumberTypeFloat) {
            iter = InsertDTypeTransNode(graph, iter, kBefore, inputIndexIdx, kFP32ToInt8, &status);
          } else {
            iter = InsertDTypeTransNode(graph, iter, kBefore, inputIndexIdx, kUInt8ToInt8, &status);
          }

          if (status != RET_OK) {
            MS_LOG(ERROR) << "InsertDTypeTransNode before " << nodeName.c_str() << " failed";
            return status;
          }
        }
      }
    }
  }
  return RET_OK;
}

STATUS DTypeTransPass::DoModelOutputDTypeTrans(schema::MetaGraphT *graph) {
  MS_ASSERT(graph != nullptr);
  if (outputDataDType == TypeId::kNumberTypeInt8) {
    return RET_OK;
  }
  MS_ASSERT(inputDataDType == TypeId::kNumberTypeFloat);
  auto &graphOutIdxes = graph->outputIndex;
  for (auto graphOutIdx : graphOutIdxes) {
    for (auto iter = graph->nodes.begin(); iter != graph->nodes.end(); iter++) {
      auto &node = *iter;
      auto nodeName = node->name;
      MS_ASSERT(node != nullptr);
      for (size_t outputIndexIdx = 0; outputIndexIdx < node->outputIndex.size(); outputIndexIdx++) {
        if (node->outputIndex.at(outputIndexIdx) == graphOutIdx) {
          // insert transNode
          STATUS status = RET_OK;
          if (inputDataDType == TypeId::kNumberTypeFloat) {
            iter = InsertDTypeTransNode(graph, iter, kAfter, outputIndexIdx, kInt8ToFP32, &status);
          } else {
            iter = InsertDTypeTransNode(graph, iter, kAfter, outputIndexIdx, kInt8ToUInt8, &status);
          }
          if (status != RET_OK) {
            MS_LOG(ERROR) << "InsertDTypeTransNode after " << nodeName.c_str() << " failed";
            return status;
          }
          break;
        }
      }
    }
  }
  return RET_OK;
}

STATUS DTypeTransPass::DoNodeInoutDTypeTrans(schema::MetaGraphT *graph) {
  MS_ASSERT(graph != nullptr);
  // insert transNode before and after existNode
  for (auto iter = graph->nodes.begin(); iter != graph->nodes.end(); iter++) {
    if (IsContain(GetUint8OpList(), GetCNodeTType(**iter)) && (*iter)->quantType == QuantType_AwareTraining) {
      continue;
    }
    auto &node = *iter;
    if (GetCNodeTType(**iter) == PrimitiveType_QuantDTypeCast) {
      continue;
    }
    bool needInsertPost = true;
    if (GetCNodeTType(**iter) == PrimitiveType_Shape) {
      needInsertPost = false;
    }
    auto nodeName = node->name;
    if (node->inputIndex.size() < kMinInputNum) {
      MS_LOG(ERROR) << "Op " << nodeName.c_str() << " should have " << kMinInputNum << " input tensor at least";
      return RET_ERROR;
    }
    STATUS status;
    // insert pre
    for (size_t i = 0; i < (*iter)->inputIndex.size(); i++) {
      MS_ASSERT(graph->allTensors.size() > (*iter)->inputIndex.at(i));
      auto &preTensor = graph->allTensors.at((*iter)->inputIndex.at(i));
      auto &graphInIdxes = graph->inputIndex;
      if (preTensor->nodeType == NodeType_ValueNode && !IsContain(graphInIdxes, (*iter)->inputIndex.at(i))) {
        continue;
      }
      iter = InsertDTypeTransNode(graph, iter, kBefore, i, kInt8ToFP32, &status);
      if (status != RET_OK) {
        MS_LOG(ERROR) << "InsertInt8ToFloat32Node before " << nodeName.c_str() << " failed";
        return RET_ERROR;
      }
    }

    if (needInsertPost) {
      for (size_t i = 0; i < (*iter)->outputIndex.size(); i++) {
        iter = InsertDTypeTransNode(graph, iter, kAfter, i, kFP32ToInt8, &status);
        if (status != RET_OK) {
          MS_LOG(ERROR) << "InsertFloat32ToUint8Node after " << nodeName.c_str() << " failed";
          return RET_ERROR;
        }
      }
    }
    (*iter)->quantType = QuantType_QUANT_NONE;
  }

  return RET_OK;
}

NodeIter DTypeTransPass::InsertDTypeTransNode(schema::MetaGraphT *graph, NodeIter existNodeIter, InsertPlace place,
                                              size_t inoutIdx, DTypeTransNodeType nodeType, STATUS *errorCode) {
  MS_ASSERT((*existNodeIter) != nullptr);
  auto existNodeName = (*existNodeIter)->name;
  std::string tileName;
  if (place == kBefore) {
    tileName = existNodeName + "_pre";
  } else {
    tileName = existNodeName + "_post";
  }
  auto transNode = std::unique_ptr<CNodeT>(new (std::nothrow) CNodeT);
  if (transNode == nullptr) {
    MS_LOG(ERROR) << "new TransNode failed";
    *errorCode = RET_ERROR;
    return graph->nodes.end();
  }
  auto quantDTypeCastParam = new (std::nothrow) QuantDTypeCastT;
  if (quantDTypeCastParam == nullptr) {
    MS_LOG(ERROR) << "new quantDTypeCastParam failed";
    *errorCode = RET_ERROR;
    return graph->nodes.end();
  }
  transNode->primitive = std::make_unique<schema::PrimitiveT>();
  transNode->primitive->value.value = quantDTypeCastParam;
  transNode->primitive->value.type = PrimitiveType_QuantDTypeCast;
  transNode->quantType = QuantType_AwareTraining;
  if (nodeType == kInt8ToFP32) {
    quantDTypeCastParam->srcT = TypeId::kNumberTypeInt8;
    quantDTypeCastParam->dstT = TypeId::kNumberTypeFloat32;
    transNode->name = "int8toft32_" + tileName + std::to_string(id++);
  } else if (nodeType == kFP32ToInt8) {
    quantDTypeCastParam->srcT = TypeId::kNumberTypeFloat32;
    quantDTypeCastParam->dstT = TypeId::kNumberTypeInt8;
    transNode->name = "ft32toint8_" + tileName + std::to_string(id++);
  } else if (nodeType == kUInt8ToInt8) {
    quantDTypeCastParam->srcT = TypeId::kNumberTypeUInt8;
    quantDTypeCastParam->dstT = TypeId::kNumberTypeInt8;
    transNode->name = "uint8toint8_" + tileName + std::to_string(id++);
  } else if (nodeType == kInt8ToUInt8) {
    quantDTypeCastParam->srcT = TypeId::kNumberTypeInt8;
    quantDTypeCastParam->dstT = TypeId::kNumberTypeUInt8;
    transNode->name = "int8touint8_" + tileName + std::to_string(id++);
  }
  transNode->primitive->value.value = quantDTypeCastParam;
  return InsertNode(graph, existNodeIter, place, inoutIdx, std::move(transNode), errorCode, castOpCopyer);
}

void DTypeTransPass::SetInputDataDType(TypeId dataType) { this->inputDataDType = dataType; }

void DTypeTransPass::SetOutputDataDType(TypeId dataType) { this->outputDataDType = dataType; }

}  // namespace lite
}  // namespace mindspore
