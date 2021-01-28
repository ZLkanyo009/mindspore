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

#include "tools/converter/parser/onnx/onnx_conv_parser.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include "ops/fusion/conv2d_fusion.h"

namespace mindspore::lite {
ops::PrimitiveC *OnnxConvParser::Parse(const onnx::GraphProto &onnx_graph, const onnx::NodeProto &onnx_node) {
  auto prim = std::make_unique<ops::Conv2DFusion>();

  prim->set_pad({0, 0, 0, 0});
  mindspore::Format format = mindspore::Format::NCHW;
  mindspore::PadMode pad_mode = mindspore::PadMode::PAD;

  int64_t channel_out = 1;
  int64_t channel_in = 1;
  int64_t group = 1;
  std::vector<int64_t> kernels;
  std::vector<int64_t> strides;
  std::vector<int64_t> dilation;
  std::vector<int64_t> pads;
  // set opdef each attr params
  for (const auto &onnx_node_attr : onnx_node.attribute()) {
    if (onnx_node_attr.name() == "group") {
      group = onnx_node_attr.i();
    } else if (onnx_node_attr.name() == "dilations") {
      if (onnx_node_attr.ints().size() != 2) {
        MS_LOG(ERROR) << "dilations size " << onnx_node_attr.ints().size() << " is not 2";
        return nullptr;
      }
      dilation.push_back(onnx_node_attr.ints(0));
      dilation.push_back(onnx_node_attr.ints(1));
    } else if (onnx_node_attr.name() == "kernels") {
      if (onnx_node_attr.ints().size() != 2) {
        MS_LOG(ERROR) << "kernel_shape size " << onnx_node_attr.ints().size() << " is not 2";
        return nullptr;
      }
      kernels.push_back(onnx_node_attr.ints(0));
      kernels.push_back(onnx_node_attr.ints(1));
      prim->set_kernel_size(kernels);
    } else if (onnx_node_attr.name() == "kernel_shape") {
      if (onnx_node_attr.ints().size() != 2) {
        MS_LOG(ERROR) << "kernel_shape size " << onnx_node_attr.ints().size() << " is not 2";
        return nullptr;
      }
      kernels.push_back(onnx_node_attr.ints(0));
      kernels.push_back(onnx_node_attr.ints(1));
      prim->set_kernel_size(kernels);
    } else if (onnx_node_attr.name() == "auto_pad") {
      if (onnx_node_attr.s() == "SAME_UPPER") {
        pad_mode = mindspore::PadMode::SAME;
      } else if (onnx_node_attr.s() == "VALID") {
        pad_mode = mindspore::PadMode::VALID;
      } else if (onnx_node_attr.s() == "NOTSET") {
        pad_mode = mindspore::PadMode::PAD;
      } else if (onnx_node_attr.s() == "SAME_LOWER") {
        MS_LOG(ERROR) << "unsupported padMode";
        return nullptr;
      }
    } else if (onnx_node_attr.name() == "pads") {
      if (onnx_node_attr.ints().size() != 4) {
        MS_LOG(ERROR) << "pads size " << onnx_node_attr.ints().size() << " is not 4";
        return nullptr;
      }
      pads.push_back(onnx_node_attr.ints(0));
      pads.push_back(onnx_node_attr.ints(2));
      pads.push_back(onnx_node_attr.ints(1));
      pads.push_back(onnx_node_attr.ints(3));
      prim->set_pad_list(pads);
    } else if (onnx_node_attr.name() == "strides") {
      if (onnx_node_attr.ints().size() != 2) {
        MS_LOG(ERROR) << "strides size " << onnx_node_attr.ints().size() << " is not 2";
        return nullptr;
      }
      strides.push_back(onnx_node_attr.ints(0));
      strides.push_back(onnx_node_attr.ints(1));
      prim->set_stride(strides);
    } else if (onnx_node_attr.name() == "order") {
      if (onnx_node_attr.s() == "NHWC") {
        format = mindspore::Format::NHWC;
      } else {
        MS_LOG(ERROR) << "Unsupported format: " << onnx_node_attr.s();
        return nullptr;
      }
    }
  }
  if (dilation.empty()) {
    dilation = {1, 1};
  }
  prim->set_dilation(dilation);

  if (pads.empty()) {
    pads = {0, 0, 0, 0};
  }
  prim->set_pad_list(pads);

  prim->set_format(format);
  prim->set_pad_mode(pad_mode);
  prim->set_group(group);

  // get channel_out and channel_in
  const auto &onnx_conv_weight = onnx_node.input(1);
  if (onnx_node.op_type() == "Conv") {
    auto node_iter =
      std::find_if(onnx_graph.initializer().begin(), onnx_graph.initializer().end(),
                   [onnx_conv_weight](const onnx::TensorProto &proto) { return proto.name() == onnx_conv_weight; });
    if (node_iter == onnx_graph.initializer().end()) {
      MS_LOG(WARNING) << "not find node: " << onnx_conv_weight;
    } else {
      std::vector<int> weight_shape;
      auto size = (*node_iter).dims_size();
      weight_shape.reserve(size);
      for (int i = 0; i < size; ++i) {
        weight_shape.emplace_back((*node_iter).dims(i));
      }
      channel_out = weight_shape[0];
      channel_in = weight_shape[1] * group;
    }
  } else {
    auto node_iter =
      std::find_if(onnx_graph.node().begin(), onnx_graph.node().end(),
                   [onnx_conv_weight](const onnx::NodeProto &proto) { return proto.output(0) == onnx_conv_weight; });
    if (node_iter == onnx_graph.node().end()) {
      MS_LOG(ERROR) << "can not find node: " << onnx_conv_weight;
      return nullptr;
    }
    std::vector<int> dims;
    auto iter = std::find_if((*node_iter).attribute().begin(), (*node_iter).attribute().end(),
                             [](const onnx::AttributeProto &attr) { return attr.name() == "shape"; });
    if (iter != (*node_iter).attribute().end()) {
      if (iter->ints().begin() == nullptr || iter->ints().end() == nullptr) {
        MS_LOG(ERROR) << "dims insert failed";
        return nullptr;
      }
      dims.insert(dims.begin(), iter->ints().begin(), iter->ints().end());
    }
    channel_out = dims.at(0);
    channel_in = dims.at(3) * group;
  }
  prim->set_in_channel(channel_in);
  prim->set_out_channel(channel_out);

  // parse activationType
  if (onnx_node.op_type() == "ConvRelu" || onnx_node.op_type() == "Int8ConvRelu") {
    prim->set_activation_type(mindspore::ActivationType::RELU);
  } else {
    prim->set_activation_type(mindspore::ActivationType::NO_ACTIVATION);
  }

  if (group == channel_in && channel_in == channel_out) {
    prim->AddAttr(ops::kIsDepthWise, MakeValue<bool>(true));
  }

  return prim.release();
}

OnnxNodeRegistrar g_onnxConvParser("Conv", new OnnxConvParser());
OnnxNodeRegistrar g_onnxInt8ConvParser("Int8Conv", new OnnxConvParser());
OnnxNodeRegistrar g_onnxConvReluParser("ConvRelu", new OnnxConvParser());
OnnxNodeRegistrar g_onnxInt8ConvReluParser("Int8ConvRelu", new OnnxConvParser());
}  // namespace mindspore::lite
