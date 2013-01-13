/*=============================================================================

Copyright (c) 2010-2013 Ville Ruusutie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

=============================================================================*/

#pragma once
#ifndef mathos_vmath_h
#define mathos_vmath_h

#include "mathos/platform.h"
#include "mathos/vmtypes.h"

namespace mathos {

// Load
vmvec vmLoadInt(void const* source);
vmvec vmLoadFloat(void const* source);
vmvec vmLoadVector2(void const* source);
vmvec vmLoadVector2A(void const* source);
vmvec vmLoadVector3(void const* source);
vmvec vmLoadVector3A(void const* source);
vmvec vmLoadVector4(void const* source);
vmvec vmLoadVector4A(void const* source);
vmmat vmLoadMatrix4(void const* source);
vmmat vmLoadMatrix4A(void const* source);
vmmat vmLoadMatrix4FromQuaternion(void const* source);
vmmat vmLoadMatrix4FromQuaternionA(void const* source);
vmtrans vmLoadTransform(void const* source);
vmtrans vmLoadTransformA(void const* source);

// Store
void vmStoreInt(void* destination, vmvecFastParam v);
void vmStoreFloat(void* destination, vmvecFastParam v);
void vmStoreVector2(void* destination, vmvecFastParam v);
void vmStoreVector2A(void* destination, vmvecFastParam v);
void vmStoreVector3(void* destination, vmvecFastParam v);
void vmStoreVector3A(void* destination, vmvecFastParam v);
void vmStoreVector4(void* destination, vmvecFastParam v);
void vmStoreVector4A(void* destination, vmvecFastParam v);
void vmStoreMatrix4(void* destination, vmmatParam m);
void vmStoreMatrix4A(void* destination, vmmatParam m);
void vmStoreTransform(void* destination, vmtransParam t);
void vmStoreTransformA(void* destination, vmtransParam t);

// Setters
vmvec vmVectorSet(float x, float y, float z, float w);
vmvec vmVectorSetInt(uint32 x, uint32 y, uint32 z, uint32 w);
vmvec vmVectorZero();
vmvec vmVectorSplatOne();
vmvec vmVectorSplatSignMask();
vmvec vmVectorSplatMask(uint32 mask);
vmvec vmVectorSplatX(vmvecFastParam v);
vmvec vmVectorSplatY(vmvecFastParam v);
vmvec vmVectorSplatZ(vmvecFastParam v);
vmvec vmVectorSplatW(vmvecFastParam v);
vmvec vmVectorReplicate(float f);
vmvec vmVectorSelect(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam control);
vmvec vmVectorPermute(vmvecFastParam v1, vmvecFastParam v2, uint32 e0, uint32 e1, uint32 e2, uint32 e3);
vmvec vmVectorPermuteV(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam control);
vmvec vmVectorMergeXY(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorMergeZW(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorDefaultRandomSeed();

// Getters
float vmVectorGetX(vmvecFastParam v);
float vmVectorGetY(vmvecFastParam v);
float vmVectorGetZ(vmvecFastParam v);
float vmVectorGetW(vmvecFastParam v);
uint32 vmVectorGetXInt(vmvecFastParam v);
uint32 vmVectorGetYInt(vmvecFastParam v);
uint32 vmVectorGetZInt(vmvecFastParam v);
uint32 vmVectorGetWInt(vmvecFastParam v);

// Macros
// vmvec vmVectorvmVectorInsert(vmvecFastParam v1, vmvecFastParam v2, uint32 selectX, uint32 selectY, uint32 selectZ, uint32 selectW);
// vmvec vmVectorShiftLeft(vmvecFastParam v1, vmvecFastParam v2, uint32 elements)
// vmvec vmVectorRotateLeft(vmvecFastParam v, uint32 elements)
// vmvec vmVectorRotateRight(vmvecFastParam v, uint32 elements)
// vmvec vmVectorSwizzle(vmvecFastParam v, uint32 e0, uint32 e1, uint32 e2, uint32 e3)
// vmvec vmVectorPermuteXYZA(vmvecFastParam v1, vmvecFastParam v2)

// Clipping
vmvec vmVectorAbs(vmvecFastParam v);
vmvec vmVectorNegate(vmvecFastParam v1);
vmvec vmVectorMin(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorMax(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorFract(vmvecFastParam v);
vmvec vmVectorRound(vmvecFastParam v);
vmvec vmVectorFloor(vmvecFastParam v);
vmvec vmVectorCeil(vmvecFastParam v);
vmvec vmVectorTruncate(vmvecFastParam v);
vmvec vmVectorClamp(vmvecFastParam v, vmvecFastParam min, vmvecFastParam max);
vmvec vmVectorSaturate(vmvecFastParam v);

// Operations
vmvec vmVectorAdd(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorSubtract(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorMultiply(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorMultiplyAdd(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3);
vmvec vmVectorNegativeMultiplySubtract(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3);
vmvec vmVectorDivide(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorDivideEst(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorSqrt(vmvecFastParam v);
vmvec vmVectorSqrtEst(vmvecFastParam v);
vmvec vmVectorReciprocalSqrt(vmvecFastParam v);
vmvec vmVectorReciprocalSqrtEst(vmvecFastParam v);
vmvec vmVectorReciprocal(vmvecFastParam v);
vmvec vmVectorReciprocalEst(vmvecFastParam v);
vmvec vmVectorSin(vmvecFastParam v);
vmvec vmVectorSinZeroHalfPI(vmvecFastParam v);
vmvec vmVectorCos(vmvecFastParam v);
vmvec vmVectorATanPositive(vmvecFastParam y, vmvecFastParam x);
vmvec vmVectorRandom01(vmvec& seed);
vmvec vmVectorRandomSigned(vmvec& seed);

// Conditional
vmvec vmVectorIsNaN(vmvecFastParam v);
vmvec vmVectorEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorNotEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorLess(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorGreater(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVectorInBounds(vmvecFastParam v, vmvecFastParam bounds);
uint32 vmVectorAllEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAllLess(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAllLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAllGreater(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAllGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAnyEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAnyLess(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAnyLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAnyGreater(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAnyGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32 vmVectorAllSame(vmvecFastParam v);

// Vector3
vmvec vmVector3Add(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVector3Subtract(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVector3Cross(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVector3Dot(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVector3Length(vmvecFastParam v);
vmvec vmVector3LengthEst(vmvecFastParam v);
vmvec vmVector3Normalize(vmvecFastParam v);
vmvec vmVector3NormalizeEst(vmvecFastParam v);
vmvec vmVector3Reflect(vmvecFastParam v, vmvecFastParam normal);
vmvec vmVector3Refract(vmvecFastParam v, vmvecFastParam normal, float const refraction);
vmvec vmVector3RefractV(vmvecFastParam v, vmvecFastParam normal, vmvecFastParam refraction);
vmvec vmVector3TransformQ(vmvecFastParam v, vmvecFastParam q);
vmvec vmVector3TransformM(vmvecFastParam v, vmmatParam m);
vmvec vmVector3TransformT(vmvecFastParam v, vmtransParam t);
vmvec vmVector3TransformNormalM(vmvecFastParam n, vmmatParam m);
vmvec vmVector3TransformNormalT(vmvecFastParam n, vmtransParam t);
vmvec vmVector3Random01(vmvec& seed);
vmvec vmVector3RandomSigned(vmvec& seed);
vmvec vmVector3RandomNormal(vmvec& seed);

// Vector4
vmvec vmVector4Dot(vmvecFastParam v1, vmvecFastParam v2);
vmvec vmVector4Length(vmvecFastParam v);
vmvec vmVector4LengthEst(vmvecFastParam v);
vmvec vmVector4Normalize(vmvecFastParam v);
vmvec vmVector4NormalizeEst(vmvecFastParam v);
vmvec vmVector4TransformQ(vmvecFastParam v, vmvecFastParam q);
vmvec vmVector4TransformM(vmvecFastParam v, vmmatParam m);
vmvec vmVector4TransformT(vmvecFastParam v, vmtransParam t);

// Matrix
vmmat vmMatrixIdentity();
vmmat vmMatrixFromTranslation(vmvecParam v);
vmmat vmMatrixFromScale(vmvecParam s);
vmmat vmMatrixFromRotationX(float const xAngle);
vmmat vmMatrixFromRotationY(float const yAngle);
vmmat vmMatrixFromRotationZ(float const zAngle);
vmmat vmMatrixFromAxisAngle(vmvecParam axis, float const angle);
vmmat vmMatrixFromQuaternion(vmvecFastParam q);
vmmat vmMatrixFromTransform(vmtransParam t);
vmmat vmMatrixFromLookAt(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);
vmmat vmMatrixFromLookAtRotation(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);
vmmat vmMatrixOrthonormalize(vmmatParam m);
vmmat vmMatrixTranspose(vmmatParam m);
vmmat vmMatrixMultiply(vmmatParam m1, vmmatParam m2);
vmmat vmMatrixMultiplyTranspose(vmmatParam m1, vmmatParam m2);
vmmat vmMatrixTranslate(vmmatParam m, vmvecParam v);
vmmat vmMatrixScale(vmmatParam m, vmvecParam s);
vmmat vmMatrixInvert(vmmatParam m);
vmmat vmMatrixInvertRotationTranslation(vmmatParam m);
vmmat vmMatrixLookAt(vmmatParam m, vmvecParam at, vmvecParam up);

// Quaternion
vmvec vmQuaternionIdentity();
vmvec vmQuaternionFromMatrix(vmmatParam m);
vmvec vmQuaternionFromRotationX(float const xAngle);
vmvec vmQuaternionFromRotationY(float const yAngle);
vmvec vmQuaternionFromRotationZ(float const zAngle);
vmvec vmQuaternionFromAxisAngle(vmvecFastParam axis, float const angle);
vmvec vmQuaternionFromLookAt(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);
vmvec vmQuaternionConjugate(vmvecFastParam q);
vmvec vmQuaternionNormalize(vmvecFastParam q);
vmvec vmQuaternionNormalizeEst(vmvecFastParam q);
vmvec vmQuaternionAdd(vmvecFastParam q1, vmvecFastParam q2);
vmvec vmQuaternionSub(vmvecFastParam q1, vmvecFastParam q2);
vmvec vmQuaternionMultiply(vmvecFastParam q1, vmvecFastParam q2);
vmvec vmQuaternionRotate(vmvecFastParam q, vmvecFastParam axis, float const angle);
vmvec vmQuaternionLerp(vmvecFastParam q1, vmvecFastParam q2, float const t);
vmvec vmQuaternionSlerp(vmvecFastParam q1, vmvecFastParam q2, float const t);
vmvec vmQuaternionSlerpV(vmvecFastParam q1, vmvecFastParam q2, vmvecFastParam t);
vmvec vmQuaternionRandom(vmvec& seed);
vmvec vmQuaternionGetXAxis(vmvecFastParam q);
vmvec vmQuaternionGetYAxis(vmvecFastParam q);
vmvec vmQuaternionGetZAxis(vmvecFastParam q);

// Transform
vmtrans vmTransformIdentity();
vmtrans vmTransformFromLookAt(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);
vmtrans vmTransformTranslate(vmtransParam t, vmvecParam v);
vmtrans vmTransformScale(vmtransParam t, float const s);
vmtrans vmTransformScaleV(vmtransParam t, vmvecParam v);
vmtrans vmTransformRotate(vmtransParam t, vmvecParam q);
vmtrans vmTransformInvert(vmtransParam t);
vmtrans vmTransformCombine(vmtransParam t1, vmtransParam t2);
vmtrans vmTransformLookAt(vmtransParam t, vmvecFastParam at, vmvecFastParam up);
vmtrans vmTransformNormalize(vmtransParam t);

} // end of mathos

#include "mathos/floatmath.h"
#if defined(VMATH_SSE)
#include "mathos/vmconvert_sse.inl"
#include "mathos/vmvector_sse.inl"
#include "mathos/vmvector3_sse.inl"
#include "mathos/vmvector4_sse.inl"
#include "mathos/vmmatrix_sse.inl"
#include "mathos/vmquaternion_sse.inl"
#include "mathos/vmtransform_sse.inl"
#elif defined(VMATH_ARM_VFP)
#include "mathos/vmmatrix_armvfp.inl"
#elif defined(VMATH_ARM_NEON)
#include "mathos/vmconvert_armneon.inl"
#include "mathos/vmvector_armneon.inl"
#include "mathos/vmvector3_armneon.inl"
#include "mathos/vmvector4_armneon.inl"
#include "mathos/vmmatrix_armneon.inl"
#include "mathos/vmquaternion_armneon.inl"
#include "mathos/vmtransform_armneon.inl"
#endif

#include "mathos/vmconvert.inl"
#include "mathos/vmvector.inl"
#include "mathos/vmvector3.inl"
#include "mathos/vmvector4.inl"
#include "mathos/vmmatrix.inl"
#include "mathos/vmquaternion.inl"
#include "mathos/vmtransform.inl"

#endif
