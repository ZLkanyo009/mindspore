/**
 * Copyright 2020-2021 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_GROUP_CONVOLUTION_CREATOR_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_GROUP_CONVOLUTION_CREATOR_H_

#include <utility>
#include <vector>
#include "src/lite_kernel.h"
#include "nnacl/conv_parameter.h"

namespace mindspore::kernel {
struct TensorInfo {
  std::vector<int> shape_;
  schema::Format format_;
  TypeId data_type_;
  lite::Tensor::Category tensor_type_;
  bool is_in_;
};

class GroupConvCreator {
 public:
  GroupConvCreator(std::vector<lite::Tensor *> inputs, std::vector<lite::Tensor *> outputs, OpParameter *op_parameter,
                   const lite::InnerContext *ctx, bool is_quant)
      : origin_inputs_(std::move(inputs)),
        origin_outputs_(std::move(outputs)),
        context_(ctx),
        infered_(op_parameter->infer_flag_),
        is_quant_(is_quant) {
    conv_param_ = reinterpret_cast<ConvParameter *>(op_parameter);
  }

  ~GroupConvCreator() = default;

 public:
  void SetShapeOfTensors();
  int CreatGroupConv();
  std::vector<kernel::LiteKernel *> get_group_conv() { return group_convs_; }

 protected:
  void set_input_shape(const std::vector<int> &shape) { input_shape_ = shape; }
  void set_output_shape(const std::vector<int> &shape) { output_shape_ = shape; }
  void set_filter_shape(const std::vector<int> &shape) { filter_shape_ = shape; }
  void set_bias_shape(const std::vector<int> &shape) { bias_shape_ = shape; }
  void CopyQuantParam(std::vector<lite::Tensor *> *tensors);
  bool CheckIfValidPoint(void *ptr);
  int NewInputTensor(std::vector<lite::Tensor *> *tensors);
  int NewConstTensor(std::vector<lite::Tensor *> *tensors, int group_id);
  int NewOutputTensor(std::vector<lite::Tensor *> *tensors, lite::Tensor *output);

 private:
  std::vector<lite::Tensor *> origin_inputs_;
  std::vector<lite::Tensor *> origin_outputs_;
  std::vector<kernel::LiteKernel *> group_convs_;
  std::vector<int> input_shape_;
  std::vector<int> output_shape_;
  std::vector<int> filter_shape_;
  std::vector<int> bias_shape_;
  const lite::InnerContext *context_;
  ConvParameter *conv_param_;
  bool infered_;
  bool is_quant_;
};

LiteKernel *CpuGroupConvFp32KernelCreator(const std::vector<lite::Tensor *> &inputs,
                                          const std::vector<lite::Tensor *> &outputs, OpParameter *op_parameter,
                                          const lite::InnerContext *ctx);

LiteKernel *CpuGroupConvInt8KernelCreator(const std::vector<lite::Tensor *> &inputs,
                                          const std::vector<lite::Tensor *> &outputs, OpParameter *op_parameter,
                                          const lite::InnerContext *ctx, int group);

}  // namespace mindspore::kernel

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_GROUP_CONVOLUTION_CREATOR_H_