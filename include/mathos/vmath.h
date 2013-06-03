/*=============================================================================

Copyright (c) 2010 Ville Ruusutie

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

#include "mathos/vmtypes.h"
//-----------------------------------------------------------------------------

namespace mathos {
namespace vm {

// Load
vmvec load2(void const* source);
vmvec load2A(void const* source);
vmvec load3(void const* source);
vmvec load3A(void const* source);
vmvec load4(void const* source);
vmvec load4A(void const* source);
vmmat loadM(void const* source);
vmtrans loadT(void const* source);
//-----------------------------------------------------------------------------

// Store
void store2(void* destination, vmvecFastParam v);
void store2A(void* destination, vmvecFastParam v);
void store3(void* destination, vmvecFastParam v);
void store3A(void* destination, vmvecFastParam v);
void store4(void* destination, vmvecFastParam v);
void store4A(void* destination, vmvecFastParam v);
void storeM(void* destination, vmmatParam m);
void storeT(void* destination, vmtransParam t);
float extractX(vmvecFastParam v);
float extractY(vmvecFastParam v);
float extractZ(vmvecFastParam v);
float extractW(vmvecFastParam v);
uint32_t extractXInt(vmvecFastParam v);
uint32_t extractYInt(vmvecFastParam v);
uint32_t extractZInt(vmvecFastParam v);
uint32_t extractWInt(vmvecFastParam v);
//-----------------------------------------------------------------------------

// Setters
vmvec make(float x, float y, float z, float w);
vmvec makeInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
vmvec replicate(float f);
vmvec splatZero();
vmvec splatOne();
vmvec splatSignMask();
vmvec splatMask(uint32_t mask);
vmvec splatX(vmvecFastParam v);
vmvec splatY(vmvecFastParam v);
vmvec splatZ(vmvecFastParam v);
vmvec splatW(vmvecFastParam v);
vmvec select(vmvecFastParam a, vmvecFastParam b, vmvecFastParam control);
vmvec permute(vmvecFastParam a, vmvecFastParam b, uint32_t e0, uint32_t e1, uint32_t e2, uint32_t e3);
vmvec permute(vmvecFastParam a, vmvecFastParam b, vmvecFastParam control);
vmvec permuteXYZA(vmvecFastParam a, vmvecFastParam b);
vmvec mergeXY(vmvecFastParam a, vmvecFastParam b);
vmvec mergeZW(vmvecFastParam a, vmvecFastParam b);

//vmvec vm_insert(vmvecFastParam v1, vmvecFastParam v2, uint32_t selectX, uint32_t selectY, uint32_t selectZ, uint32_t selectW);
//vmvec vm_shiftLeft(vmvecFastParam v1, vmvecFastParam v2, uint32_t elements);
//vmvec vm_rotateLeft(vmvecFastParam v, uint32_t elements);
//vmvec vm_rotateRight(vmvecFastParam v, uint32_t elements);
//vmvec vm_swizzle(vmvecFastParam v, uint32_t e0, uint32_t e1, uint32_t e2, uint32_t e3);
//-----------------------------------------------------------------------------

// Conditional
vmvec isNaN(vmvecFastParam v);
vmvec isEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec isNotEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec isLess(vmvecFastParam v1, vmvecFastParam v2);
vmvec isLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec isGreater(vmvecFastParam v1, vmvecFastParam v2);
vmvec isGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
vmvec isInBounds(vmvecFastParam v, vmvecFastParam bounds);
uint32_t allEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32_t allLess(vmvecFastParam v1, vmvecFastParam v2);
uint32_t allLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32_t allGreater(vmvecFastParam v1, vmvecFastParam v2);
uint32_t allGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32_t allSame(vmvecFastParam v);
uint32_t anyEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32_t anyLess(vmvecFastParam v1, vmvecFastParam v2);
uint32_t anyLessOrEqual(vmvecFastParam v1, vmvecFastParam v2);
uint32_t anyGreater(vmvecFastParam v1, vmvecFastParam v2);
uint32_t anyGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2);
//-----------------------------------------------------------------------------

// Clipping
vmvec abs4(vmvecFastParam v);
vmvec neg4(vmvecFastParam v1);
vmvec min4(vmvecFastParam v1, vmvecFastParam v2);
vmvec max4(vmvecFastParam v1, vmvecFastParam v2);
vmvec fract4(vmvecFastParam v);
vmvec round4(vmvecFastParam v);
vmvec floor4(vmvecFastParam v);
vmvec ceil4(vmvecFastParam v);
vmvec truncate4(vmvecFastParam v);
vmvec clamp4(vmvecFastParam v, vmvecFastParam min, vmvecFastParam max);
vmvec saturate4(vmvecFastParam v);
//-----------------------------------------------------------------------------

// Operations
vmvec add4(vmvecFastParam v1, vmvecFastParam v2);
vmvec sub4(vmvecFastParam v1, vmvecFastParam v2);
vmvec mul4(vmvecFastParam v1, vmvecFastParam v2);
vmvec mulAdd4(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3);
vmvec negMulSub4(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3);
vmvec div4(vmvecFastParam v1, vmvecFastParam v2);
vmvec divEst4(vmvecFastParam v1, vmvecFastParam v2);
vmvec sqrt4(vmvecFastParam v);
vmvec sqrtEst4(vmvecFastParam v);
vmvec reciprocalSqrt4(vmvecFastParam v);
vmvec reciprocalSqrtEst4(vmvecFastParam v);
vmvec reciprocal4(vmvecFastParam v);
vmvec reciprocalEst4(vmvecFastParam v);
vmvec sin4(vmvecFastParam v);
vmvec sinZeroHalfPI4(vmvecFastParam v);
vmvec cos4(vmvecFastParam v);
vmvec atanPositive4(vmvecFastParam y, vmvecFastParam x);
//-----------------------------------------------------------------------------

// Vector3
vmvec add3(vmvecFastParam v1, vmvecFastParam v2);
vmvec sub3(vmvecFastParam v1, vmvecFastParam v2);
vmvec dot3(vmvecFastParam v1, vmvecFastParam v2);
vmvec cross3(vmvecFastParam v1, vmvecFastParam v2);
vmvec length3(vmvecFastParam v);
vmvec lengthEst3(vmvecFastParam v);
vmvec normalize3(vmvecFastParam v);
vmvec normalizeEst3(vmvecFastParam v);
vmvec reflect3(vmvecFastParam v, vmvecFastParam normal);
vmvec refract3(vmvecFastParam v, vmvecFastParam normal, vmvecFastParam refraction);
vmvec transform3(vmvecFastParam v, vmvecFastParam q);
vmvec transform3(vmvecFastParam v, vmmatParam m);
vmvec transform3(vmvecFastParam v, vmtransParam t);
vmvec transformNormal(vmvecFastParam n, vmmatParam m);
vmvec transformNormal(vmvecFastParam n, vmtransParam t);
//-----------------------------------------------------------------------------

// Vector4
vmvec dot4(vmvecFastParam v1, vmvecFastParam v2);
vmvec length4(vmvecFastParam v);
vmvec lengthEst4(vmvecFastParam v);
vmvec normalize4(vmvecFastParam v);
vmvec normalizeEst4(vmvecFastParam v);
vmvec transform4(vmvecFastParam v, vmvecFastParam q);
vmvec transform4(vmvecFastParam v, vmmatParam m);
vmvec transform4(vmvecFastParam v, vmtransParam t);
vmvec lerp4(vmvecFastParam a, vmvecFastParam b, vmvecFastParam t);
//-----------------------------------------------------------------------------

// Matrix
vmmat identityM();
vmmat toMatrix(vmvecFastParam q);
vmmat toMatrix(vmtransParam t);
vmmat makeTranslationM(vmvecParam v);
vmmat makeScaleM(vmvecParam s);
vmmat makeXRotationM(float const xAngle);
vmmat makeYRotationM(float const yAngle);
vmmat makeZRotationM(float const zAngle);
vmmat makeAxisAngleM(vmvecParam axis, float const angle);
vmmat makeLookAtM(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);
vmmat makeLookAtRotationM(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);

vmmat orthonormalizeM(vmmatParam m);
vmmat transposeM(vmmatParam m);
vmmat mulM(vmmatParam a, vmmatParam b);
vmmat mulTransposeM(vmmatParam a, vmmatParam b);
vmmat translateM(vmmatParam m, vmvecParam v);
vmmat scaleM(vmmatParam m, vmvecParam s);
vmmat invertM(vmmatParam m);
vmmat invertRotationTranslationM(vmmatParam m);
//-----------------------------------------------------------------------------

// Quaternion
vmvec identityQ();
vmvec toQuaternion(vmmatParam m);
vmvec makeXRotationQ(float const xAngle);
vmvec makeYRotationQ(float const yAngle);
vmvec makeZRotationQ(float const zAngle);
vmvec makeAxisAngleQ(vmvecFastParam axis, float const angle);
vmvec makeLookAtQ(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);

vmvec addQ(vmvecFastParam a, vmvecFastParam b);
vmvec subQ(vmvecFastParam a, vmvecFastParam b);
vmvec mulQ(vmvecFastParam a, vmvecFastParam b);
vmvec conjugateQ(vmvecFastParam q);
vmvec normalizeQ(vmvecFastParam q);
vmvec normalizeEstQ(vmvecFastParam q);
vmvec rotateQ(vmvecFastParam q, vmvecFastParam axis, float const angle);
vmvec lerpQ(vmvecFastParam a, vmvecFastParam b, vmvecFastParam t);
vmvec slerpQ(vmvecFastParam a, vmvecFastParam b, vmvecFastParam t);
vmvec extractXAxisQ(vmvecFastParam q);
vmvec extractYAxisQ(vmvecFastParam q);
vmvec extractZAxisQ(vmvecFastParam q);
//-----------------------------------------------------------------------------

// Transform
vmtrans identityT();
vmtrans toTransform(vmmatParam m);
vmtrans makeLookAtT(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up);

vmtrans translateT(vmtransParam t, vmvecParam v);
vmtrans scaleT(vmtransParam t, vmvecParam s);
vmtrans rotateT(vmtransParam t, vmvecParam q);
vmtrans invertT(vmtransParam t);
vmtrans combineT(vmtransParam t1, vmtransParam t2);
vmtrans normalizeT(vmtransParam t);
//-----------------------------------------------------------------------------

// Random
vmvec randomSeed();
vmvec random01(vmvec& seed);
vmvec randomSigned(vmvec& seed);
vmvec randomNormal(vmvec& seed);
vmvec randomQuat(vmvec& seed);
//-----------------------------------------------------------------------------

} // end of vm
} // end of mathos

//#include "mathos/floatmath.h"
#if defined(VMATH_SSE)
#include "mathos/vmconvert_sse.inl"
#include "mathos/vmvector_sse.inl"
#include "mathos/vmvector3_sse.inl"
#include "mathos/vmvector4_sse.inl"
#include "mathos/vmmatrix_sse.inl"
#include "mathos/vmquaternion_sse.inl"
#include "mathos/vmtransform_sse.inl"
#endif

#if 0//defined(VMATH_ARM_NEON)
#include "mathos/vmconvert_armneon.inl"
#include "mathos/vmvector_armneon.inl"
#include "mathos/vmvector3_armneon.inl"
#include "mathos/vmvector4_armneon.inl"
#include "mathos/vmmatrix_armneon.inl"
#include "mathos/vmquaternion_armneon.inl"
#include "mathos/vmtransform_armneon.inl"
#endif

#endif
