/**
 * Copyright 2019-2021 Huawei Technologies Co., Ltd
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
#include "nnacl/fp32/elu_fp32.h"
#include "src/ops/populate/populate_register.h"
using mindspore::schema::PrimitiveType_Elu;

namespace mindspore {
namespace lite {
namespace {
OpParameter *PopulateEluParameter(const void *prim) {
  EluParameter *elu_parameter = reinterpret_cast<EluParameter *>(malloc(sizeof(EluParameter)));
  if (elu_parameter == nullptr) {
    MS_LOG(ERROR) << "malloc EluParameter failed.";
    return nullptr;
  }
  memset(elu_parameter, 0, sizeof(EluParameter));
  auto primitive = static_cast<const schema::Primitive *>(prim);
  elu_parameter->op_parameter_.type_ = primitive->value_type();
  auto param = primitive->value_as_Elu();
  elu_parameter->alpha_ = param->alpha();
  return reinterpret_cast<OpParameter *>(elu_parameter);
}
}  // namespace
REG_POPULATE(PrimitiveType_Elu, PopulateEluParameter, SCHEMA_CUR)
}  // namespace lite
}  // namespace mindspore
