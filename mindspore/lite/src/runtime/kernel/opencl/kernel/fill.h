/**
 * Copyright 2019 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_FILL_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_FILL_H_

#include <vector>
#include "nnacl/base/fill_base.h"
#include "src/runtime/kernel/opencl/opencl_kernel.h"

namespace mindspore::kernel {
class FillOpenCLKernel : public OpenCLKernel {
 public:
  using OpenCLKernel::OpenCLKernel;

  ~FillOpenCLKernel() override = default;

  int Prepare() override;

  int CheckSpecs() override;
  void SetConstArgs() override;
  void SetGlobalLocal() override;

  int Run() override;

 private:
  int RunFill();
  int RunShape();
  float default_{0.0f};
};

}  // namespace mindspore::kernel
#endif
