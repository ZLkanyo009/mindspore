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

#include "ops/gather_d.h"
#include <memory>
#include <set>
#include "ops/op_utils.h"
#include "utils/check_convert_utils.h"
#include "abstract/primitive_infer_map.h"

namespace mindspore {
namespace ops {
// gather_d
namespace {
abstract::ShapePtr GatherDInferShape(const PrimitivePtr &primitive, const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  auto prim_name = primitive->name();
  // check
  auto x_shape = CheckAndConvertUtils::ConvertShapePtrToShape("x_shape", input_args[0]->BuildShape(), prim_name);
  auto index_shape = CheckAndConvertUtils::ConvertShapePtrToShape("dim_shape", input_args[2]->BuildShape(), prim_name);
  int64_t x_rank = x_shape.size();
  CheckAndConvertUtils::Check("x_rank", x_rank, kEqual, "index_rank", index_shape.size(), prim_name);
  auto dim_v = GetValue<int64_t>(input_args[1]->BuildValue());
  CheckAndConvertUtils::Check("dim value", dim_v, kGreaterEqual, "negative index_rank", -x_rank, prim_name);
  CheckAndConvertUtils::Check("dim value", dim_v, kLessThan, "index_rank", x_rank, prim_name);

  if (dim_v < 0) {
    dim_v = dim_v + x_rank;
  }
  for (int i = 0; i < x_rank; ++i) {
    if (i == dim_v) continue;
    MS_LOG(INFO) << "Check " << i << "th x shape";
    CheckAndConvertUtils::Check("x shape", x_shape[i], kEqual, "index_rank", index_shape[i], prim_name);
  }
  return std::make_shared<abstract::Shape>(index_shape);
}

TypePtr GatherDInferType(const PrimitivePtr &prim, const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(prim);
  auto prim_name = prim->name();
  // check
  std::set<TypePtr> valid_x_type = {kTensorType};
  auto x_type =
    CheckAndConvertUtils::CheckTensorTypeValid("x_type", input_args[0]->BuildType(), valid_x_type, prim_name);
  std::set<TypePtr> valid_index_types = {kInt32, kInt64};
  CheckAndConvertUtils::CheckTensorTypeValid("index_type", input_args[2]->BuildType(), valid_index_types, prim_name);
  std::set<TypePtr> valid_dim_type = {kInt32, kInt64};
  CheckAndConvertUtils::CheckSubClass("dim_type", input_args[1]->BuildType(), valid_dim_type, prim_name);
  return x_type;
}
}  // namespace
AbstractBasePtr GatherDInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                             const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  auto abs = std::make_shared<abstract::AbstractTensor>(GatherDInferType(primitive, input_args),
                                                        GatherDInferShape(primitive, input_args));
  return abs;
}
REGISTER_PRIMITIVE_EVAL_IMPL(GatherD, prim::kPrimGatherD, GatherDInfer, nullptr, true);
}  // namespace ops
}  // namespace mindspore
