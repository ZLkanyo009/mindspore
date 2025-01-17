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
#ifndef MINDSPORE_LITE_SRC_REG_KERNELS_H
#define MINDSPORE_LITE_SRC_REG_KERNELS_H

namespace mindspore {
namespace kernel {

#ifdef MS_COMPILE_IOS
// base
extern void _kCPUkNumberTypeInt32PrimitiveType_StridedSlice();
extern void _kCPUkNumberTypeFloat32PrimitiveType_StridedSlice();
extern void _kCPUkNumberTypeFloat16PrimitiveType_StridedSlice();
extern void _kCPUkNumberTypeInt8PrimitiveType_StridedSlice();
extern void _kCPUkNumberTypeUInt8PrimitiveType_QuantDTypeCast();
extern void _kCPUkNumberTypeInt8PrimitiveType_QuantDTypeCast();
extern void _kCPUkNumberTypeFloat32PrimitiveType_QuantDTypeCast();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Assert();
extern void _kCPUkNumberTypeBoolPrimitiveType_Assert();
extern void _kCPUkNumberTypeInt32PrimitiveType_TensorListReserve();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TensorListReserve();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TensorListReserve();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TensorListStack();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TensorListStack();
extern void _kCPUkNumberTypeInt32PrimitiveType_TensorListStack();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ArgMaxFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ArgMinFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_ArgMaxFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_ArgMinFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TensorListGetItem();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TensorListGetItem();
extern void _kCPUkNumberTypeInt32PrimitiveType_TensorListGetItem();
extern void _kCPUkNumberTypeInt32PrimitiveType_RandomStandardNormal();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Split();
extern void _kCPUkNumberTypeInt32PrimitiveType_Split();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Split();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Switch();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Switch();
extern void _kCPUkNumberTypeBoolPrimitiveType_Switch();
extern void _kCPUkNumberTypeInt32PrimitiveType_Switch();
extern void _kCPUkNumberTypeFloat32PrimitiveType_PriorBox();
extern void _kCPUkNumberTypeInt8PrimitiveType_PriorBox();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ConstantOfShape();
extern void _kCPUkNumberTypeInt32PrimitiveType_ConstantOfShape();
extern void _kCPUkNumberTypeInt64PrimitiveType_ConstantOfShape();
extern void _kCPUkNumberTypeInt32PrimitiveType_SliceFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SliceFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TensorListSetItem();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TensorListSetItem();
extern void _kCPUkNumberTypeInt32PrimitiveType_TensorListSetItem();
extern void _kCPUkNumberTypeInt32PrimitiveType_Reshape();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Reshape();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Reshape();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Flatten();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Flatten();
extern void _kCPUkNumberTypeFloat32PrimitiveType_FlattenGrad();
extern void _kCPUkNumberTypeInt32PrimitiveType_ExpandDims();
extern void _kCPUkNumberTypeFloat16PrimitiveType_ExpandDims();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ExpandDims();
extern void _kCPUkNumberTypeInt8PrimitiveType_ExpandDims();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Squeeze();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Squeeze();
extern void _kCPUkNumberTypeInt32PrimitiveType_Squeeze();
extern void _kCPUkNumberTypeBoolPrimitiveType_Squeeze();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Unsqueeze();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Unsqueeze();
extern void _kCPUkNumberTypeInt32PrimitiveType_Unsqueeze();
extern void _kCPUkNumberTypeInt64PrimitiveType_Unsqueeze();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TileFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_TileFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TileFusion();
extern void _kCPUkNumberTypeBoolPrimitiveType_Select();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TensorListFromTensor();
extern void _kCPUkNumberTypeInt32PrimitiveType_TensorListFromTensor();
extern void _kCPUkNumberTypeFloat16PrimitiveType_TensorListFromTensor();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Stack();
extern void _kCPUkNumberTypeInt32PrimitiveType_Stack();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Merge();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Merge();
extern void _kCPUkNumberTypeBoolPrimitiveType_Merge();
extern void _kCPUkNumberTypeInt32PrimitiveType_Merge();

// fp16
#ifdef ENABLE_FP16
extern void _kCPUkNumberTypeFloat16PrimitiveType_SliceFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Concat();
extern void _kCPUkNumberTypeFloat16PrimitiveType_BiasAdd();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Crop();
extern void _kCPUkNumberTypeFloat16PrimitiveType_ScaleFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_PowFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Stack();
extern void _kCPUkNumberTypeFloat16PrimitiveType_GRU();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Softmax();
extern void _kCPUkNumberTypeFloat16PrimitiveType_MatMul();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Transpose();
extern void _kCPUkNumberTypeFloat16PrimitiveType_NotEqual();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Equal();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Less();
extern void _kCPUkNumberTypeFloat16PrimitiveType_LessEqual();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Greater();
extern void _kCPUkNumberTypeFloat16PrimitiveType_GreaterEqual();
extern void _kCPUkNumberTypeFloat16PrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_FullConnection();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Gather();
extern void _kCPUkNumberTypeFloat16PrimitiveType_LSTM();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Activation();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Cast();
extern void _kCPUkNumberTypeInt64PrimitiveType_Cast();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Abs();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Cos();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Log();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Square();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Sqrt();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Rsqrt();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Sin();
extern void _kCPUkNumberTypeFloat16PrimitiveType_LogicalNot();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Floor();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Ceil();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Round();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Neg();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Reciprocal();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Erf();
extern void _kCPUkNumberTypeFloat16PrimitiveType_AvgPoolFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_MaxPoolFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_QuantDTypeCast();
extern void _kCPUkNumberTypeFloat16PrimitiveType_PadFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_BatchNorm();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Conv2dTransposeFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_MulFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_AddFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_SubFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_DivFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_FloorMod();
extern void _kCPUkNumberTypeFloat16PrimitiveType_FloorDiv();
extern void _kCPUkNumberTypeFloat16PrimitiveType_LogicalAnd();
extern void _kCPUkNumberTypeFloat16PrimitiveType_LogicalOr();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Maximum();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Minimum();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Eltwise();
extern void _kCPUkNumberTypeFloat16PrimitiveType_SquaredDifference();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Conv2DFusion();
extern void _kCPUkNumberTypeFloat16PrimitiveType_InstanceNorm();
#endif

//  fp32
extern void _kCPUkNumberTypeFloat32PrimitiveType_ROIPooling();
extern void _kCPUkNumberTypeFloat32PrimitiveType_PadFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Activation();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ReverseSequence();
extern void _kCPUkNumberTypeFloat32PrimitiveType_AdderFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_DetectionPostProcess();
extern void _kCPUkNumberTypeFloat32PrimitiveType_FullConnection();
extern void _kCPUkNumberTypeFloat32PrimitiveType_GatherNd();
extern void _kCPUkNumberTypeInt32PrimitiveType_GatherNd();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Rank();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Elu();
extern void _kCPUkNumberTypeFloat32PrimitiveType_NonMaxSuppression();
extern void _kCPUkNumberTypeFloat32PrimitiveType_BatchNorm();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeIntPrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeBoolPrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SparseToDense();
extern void _kCPUkNumberTypeInt32PrimitiveType_SparseToDense();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Gather();
extern void _kCPUkNumberTypeInt32PrimitiveType_Gather();
extern void _kCPUkNumberTypeFloat32PrimitiveType_DepthToSpace();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Unique();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ExpFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ScatterNd();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SpaceToDepth();
extern void _kCPUkNumberTypeBoolPrimitiveType_NonZero();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Resize();
extern void _kCPUkNumberTypeFloat32PrimitiveType_PReLUFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_MulFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_MulFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_AddFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_AddFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SubFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_SubFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_DivFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_RealDiv();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Mod();
extern void _kCPUkNumberTypeInt32PrimitiveType_Mod();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LogicalAnd();
extern void _kCPUkNumberTypeBoolPrimitiveType_LogicalAnd();
extern void _kCPUkNumberTypeInt32PrimitiveType_LogicalAnd();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LogicalOr();
extern void _kCPUkNumberTypeBoolPrimitiveType_LogicalOr();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Maximum();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Minimum();
extern void _kCPUkNumberTypeInt32PrimitiveType_Maximum();
extern void _kCPUkNumberTypeInt32PrimitiveType_Minimum();
extern void _kCPUkNumberTypeFloat32PrimitiveType_FloorDiv();
extern void _kCPUkNumberTypeFloat32PrimitiveType_FloorMod();
extern void _kCPUkNumberTypeInt32PrimitiveType_FloorDiv();
extern void _kCPUkNumberTypeInt32PrimitiveType_FloorMod();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SquaredDifference();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Eltwise();
extern void _kCPUkNumberTypeInt32PrimitiveType_DivFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_AvgPoolFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_MaxPoolFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_EmbeddingLookupFusion();
extern void _kCPUkNumberTypeInt32PrimitiveType_Size();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Size();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Transpose();
extern void _kCPUkNumberTypeInt32PrimitiveType_Transpose();
extern void _kCPUkNumberTypeFloat32PrimitiveType_PowFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Abs();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Cos();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Log();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Square();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Sqrt();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Rsqrt();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Sin();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LogicalNot();
extern void _kCPUkNumberTypeBoolPrimitiveType_LogicalNot();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Floor();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Ceil();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Round();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Neg();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Reciprocal();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Erf();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ReverseV2();
extern void _kCPUkNumberTypeInt32PrimitiveType_ReverseV2();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Unstack();
extern void _kCPUkNumberTypeInt32PrimitiveType_InvertPermutation();
extern void _kCPUkNumberTypeFloat32PrimitiveType_InvertPermutation();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SpaceToBatch();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SpaceToBatchND();
extern void _kCPUkNumberTypeFloat32PrimitiveType_AddN();
extern void _kCPUkNumberTypeFloat32PrimitiveType_MatMul();
extern void _kCPUkNumberTypeFloat32PrimitiveType_FusedBatchNorm();
extern void _kCPUkNumberTypeFloat32PrimitiveType_BiasAdd();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LRN();
extern void _kCPUkNumberTypeFloat32PrimitiveType_TopKFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_GRU();
extern void _kCPUkNumberTypeInt32PrimitiveType_Shape();
extern void _kCPUkNumberTypeFloat16PrimitiveType_Shape();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Shape();
extern void _kCPUkNumberTypeInt8PrimitiveType_Shape();
extern void _kCPUkNumberTypeFloat32PrimitiveType_L2NormalizeFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ScaleFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_BatchToSpace();
extern void _kCPUkNumberTypeFloat32PrimitiveType_BatchToSpaceND();
extern void _kCPUkNumberTypeInt32PrimitiveType_Where();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Where();
extern void _kCPUkNumberTypeBoolPrimitiveType_Where();
extern void _kCPUkNumberTypeFloat32PrimitiveType_SkipGram();
extern void _kCPUkNumberTypeFloat32PrimitiveType_BroadcastTo();
extern void _kCPUkNumberTypeFloat32PrimitiveType_CropAndResize();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Conv2dTransposeFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Conv2DFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LayerNormFusion();
extern void _kCPUkNumberTypeFloat32PrimitiveType_ZerosLike();
extern void _kCPUkNumberTypeFloat32PrimitiveType_InstanceNorm();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Cast();
extern void _kCPUkNumberTypeUInt8PrimitiveType_Cast();
extern void _kCPUkNumberTypeInt8PrimitiveType_Cast();
extern void _kCPUkNumberTypeInt32PrimitiveType_Cast();
extern void _kCPUkNumberTypeInt64PrimitiveType_Cast();
extern void _kCPUkNumberTypeBoolPrimitiveType_Cast();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LshProjection();
extern void _kCPUkNumberTypeInt32PrimitiveType_Fill();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Fill();
extern void _kCPUkNumberTypeInt32PrimitiveType_OneHot();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LSTM();
extern void _kCPUkNumberTypeInt32PrimitiveType_Crop();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Crop();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Equal();
extern void _kCPUkNumberTypeInt32PrimitiveType_Equal();
extern void _kCPUkNumberTypeFloat32PrimitiveType_NotEqual();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Less();
extern void _kCPUkNumberTypeInt32PrimitiveType_Less();
extern void _kCPUkNumberTypeFloat32PrimitiveType_LessEqual();
extern void _kCPUkNumberTypeInt32PrimitiveType_LessEqual();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Greater();
extern void _kCPUkNumberTypeFloat32PrimitiveType_GreaterEqual();
extern void _kCPUkNumberTypeInt32PrimitiveType_GreaterEqual();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Range();
extern void _kCPUkNumberTypeFloatPrimitiveType_Range();
extern void _kCPUkNumberTypeInt32PrimitiveType_Range();
extern void _kCPUkNumberTypeIntPrimitiveType_Range();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Softmax();
extern void _kCPUkNumberTypeInt32PrimitiveType_Concat();
extern void _kCPUkNumberTypeFloat32PrimitiveType_Concat();

// int8
extern void _kCPUkNumberTypeInt8PrimitiveType_PowFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_ReduceFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_TopKFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_GatherNd();
extern void _kCPUkNumberTypeInt8PrimitiveType_PadFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_L2NormalizeFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_MulFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_Unsqueeze();
extern void _kCPUkNumberTypeInt8PrimitiveType_Conv2DFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_AvgPoolFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_MaxPoolFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_BatchNorm();
extern void _kCPUkNumberTypeInt8PrimitiveType_FusedBatchNorm();
extern void _kCPUkNumberTypeInt8PrimitiveType_Equal();
extern void _kCPUkNumberTypeInt8PrimitiveType_NotEqual();
extern void _kCPUkNumberTypeInt8PrimitiveType_Less();
extern void _kCPUkNumberTypeInt8PrimitiveType_LessEqual();
extern void _kCPUkNumberTypeInt8PrimitiveType_Greater();
extern void _kCPUkNumberTypeInt8PrimitiveType_GreaterEqual();
extern void _kCPUkNumberTypeInt8PrimitiveType_Eltwise();
extern void _kCPUkNumberTypeInt8PrimitiveType_BatchToSpace();
extern void _kCPUkNumberTypeInt8PrimitiveType_BatchToSpaceND();
extern void _kCPUkNumberTypeInt8PrimitiveType_DepthToSpace();
extern void _kCPUkNumberTypeInt8PrimitiveType_Activation();
extern void _kCPUkNumberTypeInt8PrimitiveType_Round();
extern void _kCPUkNumberTypeInt8PrimitiveType_Floor();
extern void _kCPUkNumberTypeInt8PrimitiveType_Ceil();
extern void _kCPUkNumberTypeInt8PrimitiveType_Abs();
extern void _kCPUkNumberTypeInt8PrimitiveType_Sin();
extern void _kCPUkNumberTypeInt8PrimitiveType_Cos();
extern void _kCPUkNumberTypeInt8PrimitiveType_Log();
extern void _kCPUkNumberTypeInt8PrimitiveType_Sqrt();
extern void _kCPUkNumberTypeInt8PrimitiveType_Rsqrt();
extern void _kCPUkNumberTypeInt8PrimitiveType_Square();
extern void _kCPUkNumberTypeInt8PrimitiveType_LogicalNot();
extern void _kCPUkNumberTypeInt8PrimitiveType_Reciprocal();
extern void _kCPUkNumberTypeInt8PrimitiveType_SliceFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_Reshape();
extern void _kCPUkNumberTypeInt8PrimitiveType_DivFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_MatMul();
extern void _kCPUkNumberTypeInt8PrimitiveType_Concat();
extern void _kCPUkNumberTypeInt8PrimitiveType_Crop();
extern void _kCPUkNumberTypeInt8PrimitiveType_SpaceToBatch();
extern void _kCPUkNumberTypeInt8PrimitiveType_SpaceToBatchND();
extern void _kCPUkNumberTypeInt8PrimitiveType_Split();
extern void _kCPUkNumberTypeInt8PrimitiveType_BiasAdd();
extern void _kCPUkNumberTypeInt8PrimitiveType_Resize();
extern void _kCPUkNumberTypeInt8PrimitiveType_Gather();
extern void _kCPUkNumberTypeInt8PrimitiveType_AddFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_Squeeze();
extern void _kCPUkNumberTypeInt8PrimitiveType_ScaleFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_Softmax();
extern void _kCPUkNumberTypeInt8PrimitiveType_ArgMaxFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_ArgMinFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_FullConnection();
extern void _kCPUkNumberTypeInt8PrimitiveType_Transpose();
extern void _kCPUkNumberTypeInt8PrimitiveType_Conv2dTransposeFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_DetectionPostProcess();
extern void _kCPUkNumberTypeInt8PrimitiveType_LeakyRelu();
extern void _kCPUkNumberTypeInt8PrimitiveType_LayerNormFusion();
extern void _kCPUkNumberTypeInt8PrimitiveType_SubFusion();

void IosRegisterKernels() {
  // base
  _kCPUkNumberTypeInt32PrimitiveType_StridedSlice();
  _kCPUkNumberTypeFloat32PrimitiveType_StridedSlice();
  _kCPUkNumberTypeFloat16PrimitiveType_StridedSlice();
  _kCPUkNumberTypeInt8PrimitiveType_StridedSlice();
  _kCPUkNumberTypeUInt8PrimitiveType_QuantDTypeCast();
  _kCPUkNumberTypeInt8PrimitiveType_QuantDTypeCast();
  _kCPUkNumberTypeFloat32PrimitiveType_QuantDTypeCast();
  _kCPUkNumberTypeFloat32PrimitiveType_Assert();
  _kCPUkNumberTypeBoolPrimitiveType_Assert();
  _kCPUkNumberTypeInt32PrimitiveType_TensorListReserve();
  _kCPUkNumberTypeFloat16PrimitiveType_TensorListReserve();
  _kCPUkNumberTypeFloat32PrimitiveType_TensorListReserve();
  _kCPUkNumberTypeFloat32PrimitiveType_TensorListStack();
  _kCPUkNumberTypeFloat16PrimitiveType_TensorListStack();
  _kCPUkNumberTypeInt32PrimitiveType_TensorListStack();
  _kCPUkNumberTypeFloat32PrimitiveType_ArgMaxFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_ArgMinFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_ArgMaxFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_ArgMinFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_TensorListGetItem();
  _kCPUkNumberTypeFloat16PrimitiveType_TensorListGetItem();
  _kCPUkNumberTypeInt32PrimitiveType_TensorListGetItem();
  _kCPUkNumberTypeInt32PrimitiveType_RandomStandardNormal();
  _kCPUkNumberTypeFloat16PrimitiveType_Split();
  _kCPUkNumberTypeInt32PrimitiveType_Split();
  _kCPUkNumberTypeFloat32PrimitiveType_Split();
  _kCPUkNumberTypeFloat32PrimitiveType_Switch();
  _kCPUkNumberTypeFloat16PrimitiveType_Switch();
  _kCPUkNumberTypeBoolPrimitiveType_Switch();
  _kCPUkNumberTypeInt32PrimitiveType_Switch();
  _kCPUkNumberTypeFloat32PrimitiveType_PriorBox();
  _kCPUkNumberTypeInt8PrimitiveType_PriorBox();
  _kCPUkNumberTypeFloat32PrimitiveType_ConstantOfShape();
  _kCPUkNumberTypeInt32PrimitiveType_ConstantOfShape();
  _kCPUkNumberTypeInt64PrimitiveType_ConstantOfShape();
  _kCPUkNumberTypeInt32PrimitiveType_SliceFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_SliceFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_TensorListSetItem();
  _kCPUkNumberTypeFloat16PrimitiveType_TensorListSetItem();
  _kCPUkNumberTypeInt32PrimitiveType_TensorListSetItem();
  _kCPUkNumberTypeInt32PrimitiveType_Reshape();
  _kCPUkNumberTypeFloat32PrimitiveType_Reshape();
  _kCPUkNumberTypeFloat16PrimitiveType_Reshape();
  _kCPUkNumberTypeFloat16PrimitiveType_Flatten();
  _kCPUkNumberTypeFloat32PrimitiveType_Flatten();
  _kCPUkNumberTypeFloat32PrimitiveType_FlattenGrad();
  _kCPUkNumberTypeInt32PrimitiveType_ExpandDims();
  _kCPUkNumberTypeFloat16PrimitiveType_ExpandDims();
  _kCPUkNumberTypeFloat32PrimitiveType_ExpandDims();
  _kCPUkNumberTypeInt8PrimitiveType_ExpandDims();
  _kCPUkNumberTypeFloat32PrimitiveType_Squeeze();
  _kCPUkNumberTypeFloat16PrimitiveType_Squeeze();
  _kCPUkNumberTypeInt32PrimitiveType_Squeeze();
  _kCPUkNumberTypeBoolPrimitiveType_Squeeze();
  _kCPUkNumberTypeFloat16PrimitiveType_Unsqueeze();
  _kCPUkNumberTypeFloat32PrimitiveType_Unsqueeze();
  _kCPUkNumberTypeInt32PrimitiveType_Unsqueeze();
  _kCPUkNumberTypeInt64PrimitiveType_Unsqueeze();
  _kCPUkNumberTypeFloat32PrimitiveType_TileFusion();
  _kCPUkNumberTypeInt32PrimitiveType_TileFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_TileFusion();
  _kCPUkNumberTypeBoolPrimitiveType_Select();
  _kCPUkNumberTypeFloat32PrimitiveType_TensorListFromTensor();
  _kCPUkNumberTypeInt32PrimitiveType_TensorListFromTensor();
  _kCPUkNumberTypeFloat16PrimitiveType_TensorListFromTensor();
  _kCPUkNumberTypeFloat32PrimitiveType_Stack();
  _kCPUkNumberTypeInt32PrimitiveType_Stack();
  _kCPUkNumberTypeFloat32PrimitiveType_Merge();
  _kCPUkNumberTypeFloat16PrimitiveType_Merge();
  _kCPUkNumberTypeBoolPrimitiveType_Merge();
  _kCPUkNumberTypeInt32PrimitiveType_Merge();

// fp16
#ifdef ENABLE_FP16
  _kCPUkNumberTypeFloat16PrimitiveType_SliceFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_Concat();
  _kCPUkNumberTypeFloat16PrimitiveType_BiasAdd();
  _kCPUkNumberTypeFloat16PrimitiveType_Crop();
  _kCPUkNumberTypeFloat16PrimitiveType_ScaleFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_PowFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_Stack();
  _kCPUkNumberTypeFloat16PrimitiveType_GRU();
  _kCPUkNumberTypeFloat16PrimitiveType_Softmax();
  _kCPUkNumberTypeFloat16PrimitiveType_MatMul();
  _kCPUkNumberTypeFloat16PrimitiveType_Transpose();
  _kCPUkNumberTypeFloat16PrimitiveType_NotEqual();
  _kCPUkNumberTypeFloat16PrimitiveType_Equal();
  _kCPUkNumberTypeFloat16PrimitiveType_Less();
  _kCPUkNumberTypeFloat16PrimitiveType_LessEqual();
  _kCPUkNumberTypeFloat16PrimitiveType_Greater();
  _kCPUkNumberTypeFloat16PrimitiveType_GreaterEqual();
  _kCPUkNumberTypeFloat16PrimitiveType_ReduceFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_FullConnection();
  _kCPUkNumberTypeFloat16PrimitiveType_Gather();
  _kCPUkNumberTypeFloat16PrimitiveType_LSTM();
  _kCPUkNumberTypeFloat16PrimitiveType_Activation();
  _kCPUkNumberTypeFloat16PrimitiveType_Cast();
  _kCPUkNumberTypeInt64PrimitiveType_Cast();
  _kCPUkNumberTypeFloat16PrimitiveType_Abs();
  _kCPUkNumberTypeFloat16PrimitiveType_Cos();
  _kCPUkNumberTypeFloat16PrimitiveType_Log();
  _kCPUkNumberTypeFloat16PrimitiveType_Square();
  _kCPUkNumberTypeFloat16PrimitiveType_Sqrt();
  _kCPUkNumberTypeFloat16PrimitiveType_Rsqrt();
  _kCPUkNumberTypeFloat16PrimitiveType_Sin();
  _kCPUkNumberTypeFloat16PrimitiveType_LogicalNot();
  _kCPUkNumberTypeFloat16PrimitiveType_Floor();
  _kCPUkNumberTypeFloat16PrimitiveType_Ceil();
  _kCPUkNumberTypeFloat16PrimitiveType_Round();
  _kCPUkNumberTypeFloat16PrimitiveType_Neg();
  _kCPUkNumberTypeFloat16PrimitiveType_Reciprocal();
  _kCPUkNumberTypeFloat16PrimitiveType_Erf();
  _kCPUkNumberTypeFloat16PrimitiveType_AvgPoolFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_MaxPoolFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_QuantDTypeCast();
  _kCPUkNumberTypeFloat16PrimitiveType_PadFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_BatchNorm();
  _kCPUkNumberTypeFloat16PrimitiveType_Conv2dTransposeFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_MulFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_AddFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_SubFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_DivFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_FloorMod();
  _kCPUkNumberTypeFloat16PrimitiveType_FloorDiv();
  _kCPUkNumberTypeFloat16PrimitiveType_LogicalAnd();
  _kCPUkNumberTypeFloat16PrimitiveType_LogicalOr();
  _kCPUkNumberTypeFloat16PrimitiveType_Maximum();
  _kCPUkNumberTypeFloat16PrimitiveType_Minimum();
  _kCPUkNumberTypeFloat16PrimitiveType_Eltwise();
  _kCPUkNumberTypeFloat16PrimitiveType_SquaredDifference();
  _kCPUkNumberTypeFloat16PrimitiveType_Conv2DFusion();
  _kCPUkNumberTypeFloat16PrimitiveType_InstanceNorm();
#endif

  // fp32
  _kCPUkNumberTypeFloat32PrimitiveType_ROIPooling();
  _kCPUkNumberTypeFloat32PrimitiveType_PadFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_Activation();
  _kCPUkNumberTypeFloat32PrimitiveType_ReverseSequence();
  _kCPUkNumberTypeFloat32PrimitiveType_AdderFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_DetectionPostProcess();
  _kCPUkNumberTypeFloat32PrimitiveType_FullConnection();
  _kCPUkNumberTypeFloat32PrimitiveType_GatherNd();
  _kCPUkNumberTypeInt32PrimitiveType_GatherNd();
  _kCPUkNumberTypeFloat32PrimitiveType_Rank();
  _kCPUkNumberTypeFloat32PrimitiveType_Elu();
  _kCPUkNumberTypeFloat32PrimitiveType_NonMaxSuppression();
  _kCPUkNumberTypeFloat32PrimitiveType_BatchNorm();
  _kCPUkNumberTypeFloat32PrimitiveType_ReduceFusion();
  _kCPUkNumberTypeIntPrimitiveType_ReduceFusion();
  _kCPUkNumberTypeInt32PrimitiveType_ReduceFusion();
  _kCPUkNumberTypeBoolPrimitiveType_ReduceFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_SparseToDense();
  _kCPUkNumberTypeInt32PrimitiveType_SparseToDense();
  _kCPUkNumberTypeFloat32PrimitiveType_Gather();
  _kCPUkNumberTypeInt32PrimitiveType_Gather();
  _kCPUkNumberTypeFloat32PrimitiveType_DepthToSpace();
  _kCPUkNumberTypeFloat32PrimitiveType_Unique();
  _kCPUkNumberTypeFloat32PrimitiveType_ExpFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_ScatterNd();
  _kCPUkNumberTypeFloat32PrimitiveType_SpaceToDepth();
  _kCPUkNumberTypeBoolPrimitiveType_NonZero();
  _kCPUkNumberTypeFloat32PrimitiveType_Resize();
  _kCPUkNumberTypeFloat32PrimitiveType_PReLUFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_MulFusion();
  _kCPUkNumberTypeInt32PrimitiveType_MulFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_AddFusion();
  _kCPUkNumberTypeInt32PrimitiveType_AddFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_SubFusion();
  _kCPUkNumberTypeInt32PrimitiveType_SubFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_DivFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_RealDiv();
  _kCPUkNumberTypeFloat32PrimitiveType_Mod();
  _kCPUkNumberTypeInt32PrimitiveType_Mod();
  _kCPUkNumberTypeFloat32PrimitiveType_LogicalAnd();
  _kCPUkNumberTypeBoolPrimitiveType_LogicalAnd();
  _kCPUkNumberTypeInt32PrimitiveType_LogicalAnd();
  _kCPUkNumberTypeFloat32PrimitiveType_LogicalOr();
  _kCPUkNumberTypeBoolPrimitiveType_LogicalOr();
  _kCPUkNumberTypeFloat32PrimitiveType_Maximum();
  _kCPUkNumberTypeFloat32PrimitiveType_Minimum();
  _kCPUkNumberTypeInt32PrimitiveType_Maximum();
  _kCPUkNumberTypeInt32PrimitiveType_Minimum();
  _kCPUkNumberTypeFloat32PrimitiveType_FloorDiv();
  _kCPUkNumberTypeFloat32PrimitiveType_FloorMod();
  _kCPUkNumberTypeInt32PrimitiveType_FloorDiv();
  _kCPUkNumberTypeInt32PrimitiveType_FloorMod();
  _kCPUkNumberTypeFloat32PrimitiveType_SquaredDifference();
  _kCPUkNumberTypeFloat32PrimitiveType_Eltwise();
  _kCPUkNumberTypeInt32PrimitiveType_DivFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_AvgPoolFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_MaxPoolFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_EmbeddingLookupFusion();
  _kCPUkNumberTypeInt32PrimitiveType_Size();
  _kCPUkNumberTypeFloat32PrimitiveType_Size();
  _kCPUkNumberTypeFloat32PrimitiveType_Transpose();
  _kCPUkNumberTypeInt32PrimitiveType_Transpose();
  _kCPUkNumberTypeFloat32PrimitiveType_PowFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_Abs();
  _kCPUkNumberTypeFloat32PrimitiveType_Cos();
  _kCPUkNumberTypeFloat32PrimitiveType_Log();
  _kCPUkNumberTypeFloat32PrimitiveType_Square();
  _kCPUkNumberTypeFloat32PrimitiveType_Sqrt();
  _kCPUkNumberTypeFloat32PrimitiveType_Rsqrt();
  _kCPUkNumberTypeFloat32PrimitiveType_Sin();
  _kCPUkNumberTypeFloat32PrimitiveType_LogicalNot();
  _kCPUkNumberTypeBoolPrimitiveType_LogicalNot();
  _kCPUkNumberTypeFloat32PrimitiveType_Floor();
  _kCPUkNumberTypeFloat32PrimitiveType_Ceil();
  _kCPUkNumberTypeFloat32PrimitiveType_Round();
  _kCPUkNumberTypeFloat32PrimitiveType_Neg();
  _kCPUkNumberTypeFloat32PrimitiveType_Reciprocal();
  _kCPUkNumberTypeFloat32PrimitiveType_Erf();
  _kCPUkNumberTypeFloat32PrimitiveType_ReverseV2();
  _kCPUkNumberTypeInt32PrimitiveType_ReverseV2();
  _kCPUkNumberTypeFloat32PrimitiveType_Unstack();
  _kCPUkNumberTypeInt32PrimitiveType_InvertPermutation();
  _kCPUkNumberTypeFloat32PrimitiveType_InvertPermutation();
  _kCPUkNumberTypeFloat32PrimitiveType_SpaceToBatch();
  _kCPUkNumberTypeFloat32PrimitiveType_SpaceToBatchND();
  _kCPUkNumberTypeFloat32PrimitiveType_AddN();
  _kCPUkNumberTypeFloat32PrimitiveType_MatMul();
  _kCPUkNumberTypeFloat32PrimitiveType_FusedBatchNorm();
  _kCPUkNumberTypeFloat32PrimitiveType_BiasAdd();
  _kCPUkNumberTypeFloat32PrimitiveType_LRN();
  _kCPUkNumberTypeFloat32PrimitiveType_TopKFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_GRU();
  _kCPUkNumberTypeInt32PrimitiveType_Shape();
  _kCPUkNumberTypeFloat16PrimitiveType_Shape();
  _kCPUkNumberTypeFloat32PrimitiveType_Shape();
  _kCPUkNumberTypeInt8PrimitiveType_Shape();
  _kCPUkNumberTypeFloat32PrimitiveType_L2NormalizeFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_ScaleFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_BatchToSpace();
  _kCPUkNumberTypeFloat32PrimitiveType_BatchToSpaceND();
  _kCPUkNumberTypeInt32PrimitiveType_Where();
  _kCPUkNumberTypeFloat32PrimitiveType_Where();
  _kCPUkNumberTypeBoolPrimitiveType_Where();
  _kCPUkNumberTypeFloat32PrimitiveType_SkipGram();
  _kCPUkNumberTypeFloat32PrimitiveType_BroadcastTo();
  _kCPUkNumberTypeFloat32PrimitiveType_CropAndResize();
  _kCPUkNumberTypeFloat32PrimitiveType_Conv2dTransposeFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_Conv2DFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_LayerNormFusion();
  _kCPUkNumberTypeFloat32PrimitiveType_ZerosLike();
  _kCPUkNumberTypeFloat32PrimitiveType_InstanceNorm();
  _kCPUkNumberTypeFloat32PrimitiveType_Cast();
  _kCPUkNumberTypeUInt8PrimitiveType_Cast();
  _kCPUkNumberTypeInt8PrimitiveType_Cast();
  _kCPUkNumberTypeInt32PrimitiveType_Cast();
  _kCPUkNumberTypeInt64PrimitiveType_Cast();
  _kCPUkNumberTypeBoolPrimitiveType_Cast();
  _kCPUkNumberTypeFloat32PrimitiveType_LshProjection();
  _kCPUkNumberTypeInt32PrimitiveType_Fill();
  _kCPUkNumberTypeFloat32PrimitiveType_Fill();
  _kCPUkNumberTypeInt32PrimitiveType_OneHot();
  _kCPUkNumberTypeFloat32PrimitiveType_LSTM();
  _kCPUkNumberTypeInt32PrimitiveType_Crop();
  _kCPUkNumberTypeFloat32PrimitiveType_Crop();
  _kCPUkNumberTypeFloat32PrimitiveType_Equal();
  _kCPUkNumberTypeInt32PrimitiveType_Equal();
  _kCPUkNumberTypeFloat32PrimitiveType_NotEqual();
  _kCPUkNumberTypeFloat32PrimitiveType_Less();
  _kCPUkNumberTypeInt32PrimitiveType_Less();
  _kCPUkNumberTypeFloat32PrimitiveType_LessEqual();
  _kCPUkNumberTypeInt32PrimitiveType_LessEqual();
  _kCPUkNumberTypeFloat32PrimitiveType_Greater();
  _kCPUkNumberTypeFloat32PrimitiveType_GreaterEqual();
  _kCPUkNumberTypeInt32PrimitiveType_GreaterEqual();
  _kCPUkNumberTypeFloat32PrimitiveType_Range();
  _kCPUkNumberTypeFloatPrimitiveType_Range();
  _kCPUkNumberTypeInt32PrimitiveType_Range();
  _kCPUkNumberTypeIntPrimitiveType_Range();
  _kCPUkNumberTypeFloat32PrimitiveType_Softmax();
  _kCPUkNumberTypeInt32PrimitiveType_Concat();
  _kCPUkNumberTypeFloat32PrimitiveType_Concat();

  // int8
  _kCPUkNumberTypeInt8PrimitiveType_PowFusion();
  _kCPUkNumberTypeInt8PrimitiveType_ReduceFusion();
  _kCPUkNumberTypeInt8PrimitiveType_TopKFusion();
  _kCPUkNumberTypeInt8PrimitiveType_GatherNd();
  _kCPUkNumberTypeInt8PrimitiveType_PadFusion();
  _kCPUkNumberTypeInt8PrimitiveType_L2NormalizeFusion();
  _kCPUkNumberTypeInt8PrimitiveType_MulFusion();
  _kCPUkNumberTypeInt8PrimitiveType_Unsqueeze();
  _kCPUkNumberTypeInt8PrimitiveType_Conv2DFusion();
  _kCPUkNumberTypeInt8PrimitiveType_AvgPoolFusion();
  _kCPUkNumberTypeInt8PrimitiveType_MaxPoolFusion();
  _kCPUkNumberTypeInt8PrimitiveType_BatchNorm();
  _kCPUkNumberTypeInt8PrimitiveType_FusedBatchNorm();
  _kCPUkNumberTypeInt8PrimitiveType_Equal();
  _kCPUkNumberTypeInt8PrimitiveType_NotEqual();
  _kCPUkNumberTypeInt8PrimitiveType_Less();
  _kCPUkNumberTypeInt8PrimitiveType_LessEqual();
  _kCPUkNumberTypeInt8PrimitiveType_Greater();
  _kCPUkNumberTypeInt8PrimitiveType_GreaterEqual();
  _kCPUkNumberTypeInt8PrimitiveType_Eltwise();
  _kCPUkNumberTypeInt8PrimitiveType_BatchToSpace();
  _kCPUkNumberTypeInt8PrimitiveType_BatchToSpaceND();
  _kCPUkNumberTypeInt8PrimitiveType_DepthToSpace();
  _kCPUkNumberTypeInt8PrimitiveType_Activation();
  _kCPUkNumberTypeInt8PrimitiveType_Round();
  _kCPUkNumberTypeInt8PrimitiveType_Floor();
  _kCPUkNumberTypeInt8PrimitiveType_Ceil();
  _kCPUkNumberTypeInt8PrimitiveType_Abs();
  _kCPUkNumberTypeInt8PrimitiveType_Sin();
  _kCPUkNumberTypeInt8PrimitiveType_Cos();
  _kCPUkNumberTypeInt8PrimitiveType_Log();
  _kCPUkNumberTypeInt8PrimitiveType_Sqrt();
  _kCPUkNumberTypeInt8PrimitiveType_Rsqrt();
  _kCPUkNumberTypeInt8PrimitiveType_Square();
  _kCPUkNumberTypeInt8PrimitiveType_LogicalNot();
  _kCPUkNumberTypeInt8PrimitiveType_Reciprocal();
  _kCPUkNumberTypeInt8PrimitiveType_SliceFusion();
  _kCPUkNumberTypeInt8PrimitiveType_Reshape();
  _kCPUkNumberTypeInt8PrimitiveType_DivFusion();
  _kCPUkNumberTypeInt8PrimitiveType_MatMul();
  _kCPUkNumberTypeInt8PrimitiveType_Concat();
  _kCPUkNumberTypeInt8PrimitiveType_Crop();
  _kCPUkNumberTypeInt8PrimitiveType_SpaceToBatch();
  _kCPUkNumberTypeInt8PrimitiveType_SpaceToBatchND();
  _kCPUkNumberTypeInt8PrimitiveType_Split();
  _kCPUkNumberTypeInt8PrimitiveType_BiasAdd();
  _kCPUkNumberTypeInt8PrimitiveType_Resize();
  _kCPUkNumberTypeInt8PrimitiveType_Gather();
  _kCPUkNumberTypeInt8PrimitiveType_AddFusion();
  _kCPUkNumberTypeInt8PrimitiveType_Squeeze();
  _kCPUkNumberTypeInt8PrimitiveType_ScaleFusion();
  _kCPUkNumberTypeInt8PrimitiveType_Softmax();
  _kCPUkNumberTypeInt8PrimitiveType_ArgMaxFusion();
  _kCPUkNumberTypeInt8PrimitiveType_ArgMinFusion();
  _kCPUkNumberTypeInt8PrimitiveType_FullConnection();
  _kCPUkNumberTypeInt8PrimitiveType_Transpose();
  _kCPUkNumberTypeInt8PrimitiveType_Conv2dTransposeFusion();
  _kCPUkNumberTypeInt8PrimitiveType_DetectionPostProcess();
  _kCPUkNumberTypeInt8PrimitiveType_LeakyRelu();
  _kCPUkNumberTypeInt8PrimitiveType_LayerNormFusion();
  _kCPUkNumberTypeInt8PrimitiveType_SubFusion();
}

#endif

}  // namespace kernel
}  // namespace mindspore

#endif  // MINDSPORE_LITE_SRC_REG_KERNELS_H
