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
#ifndef MINDSPORE_NNACL_INT8_SLICE_INT8_H_
#define MINDSPORE_NNACL_INT8_SLICE_INT8_H_

#include <math.h>
#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/slice_parameter.h"
#include "nnacl/int8/fixed_point.h"

#ifdef __cplusplus
extern "C" {
#endif
int SliceInt8NoParallel(const int8_t *input, int8_t *output, SliceParameter *param);
int SliceInt8(const int8_t *input, int8_t *output, SliceParameter *param, int thread_id);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_NNACL_INT8_SLICE_INT8_H_
