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

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_RANDOM_STANDARD_NORMAL_BASE_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_RANDOM_STANDARD_NORMAL_BASE_H_

#include <vector>
#include "src/lite_kernel.h"
#include "nnacl/random_standard_normal_parameter.h"

using mindspore::lite::InnerContext;

namespace mindspore::kernel {
class RandomStandardNormalCPUKernel : public LiteKernel {
 public:
  RandomStandardNormalCPUKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                                const std::vector<lite::Tensor *> &outputs, const InnerContext *ctx)
      : LiteKernel(parameter, inputs, outputs, ctx) {
    param_ = reinterpret_cast<RandomStandardNormalParam *>(parameter);
  }
  ~RandomStandardNormalCPUKernel() override = default;

  int Init() override;
  int ReSize() override;
  int Run() override;

 protected:
  RandomStandardNormalParam *param_ = nullptr;
};
}  // namespace mindspore::kernel

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_RANDOM_STANDARD_NORMAL_BASE_H_