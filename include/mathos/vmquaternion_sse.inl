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
#ifndef mathos_vmquaternion_sse_inl
#define mathos_vmquaternion_sse_inl

namespace mathos {
namespace vm {

__forceinline vmvec identityQ()
{
    static const __m128 identity = {0.0f, 0.0f, 0.0f, 1.0f};
    return identity;
}

__forceinline vmvec toQuaternion(vmmatParam m)
{
    static const __m128 One = {1.0f, 1.0f, 1.0f, 1.0f};
    static const __m128 OneHalf = {0.5f, 0.5f, 0.5f, 0.5f};
    static const __m128 OneQuarter = {0.25f, 0.25f, 0.25f, 0.25f};
    static const __m128 NegOne = {-1.0f, -1.0f, -1.0f, -1.0f};
    static const __m128 SignPNNP = {1.0f, -1.0f, -1.0f, 1.0f};
    static const __m128 SignNPNP = {-1.0f, 1.0f, -1.0f, 1.0f};
    static const __m128 SignNNPP = {-1.0f, -1.0f, 1.0f, 1.0f};
    static const __m128 SignPNPP = {1.0f, -1.0f, 1.0f, 1.0f};
    static const __m128 SignPPNP = {1.0f, 1.0f, -1.0f, 1.0f};
    static const __m128 SignNPPP = {-1.0f, 1.0f, 1.0f, 1.0f};
    static const __m128 Epsilon = {1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f};
    static const vmmask Permute0X0X0Y0W = {VM_PERMUTE_0X, VM_PERMUTE_0X, VM_PERMUTE_0Y, VM_PERMUTE_0W};
    static const vmmask Permute0Y0Z0Z1W = {VM_PERMUTE_0Y, VM_PERMUTE_0Z, VM_PERMUTE_0Z, VM_PERMUTE_1W};
    static const vmmask SplatX = {VM_PERMUTE_0X, VM_PERMUTE_0X, VM_PERMUTE_0X, VM_PERMUTE_0X};
    static const vmmask SplatY = {VM_PERMUTE_0Y, VM_PERMUTE_0Y, VM_PERMUTE_0Y, VM_PERMUTE_0Y};
    static const vmmask SplatZ = {VM_PERMUTE_0Z, VM_PERMUTE_0Z, VM_PERMUTE_0Z, VM_PERMUTE_0Z};
    static const vmmask SplatW = {VM_PERMUTE_0W, VM_PERMUTE_0W, VM_PERMUTE_0W, VM_PERMUTE_0W};
    static const vmmask PermuteC = {VM_PERMUTE_0X, VM_PERMUTE_0Z, VM_PERMUTE_1X, VM_PERMUTE_1Y};
    static const vmmask PermuteA = {VM_PERMUTE_0Y, VM_PERMUTE_1Y, VM_PERMUTE_1Z, VM_PERMUTE_0W};
    static const vmmask PermuteB = {VM_PERMUTE_1X, VM_PERMUTE_1W, VM_PERMUTE_0Z, VM_PERMUTE_0W};
    static const vmmask Permute0 = {VM_PERMUTE_0X, VM_PERMUTE_1X, VM_PERMUTE_1Z, VM_PERMUTE_1Y};
    static const vmmask Permute1 = {VM_PERMUTE_1X, VM_PERMUTE_0Y, VM_PERMUTE_1Y, VM_PERMUTE_1Z};
    static const vmmask Permute2 = {VM_PERMUTE_1Z, VM_PERMUTE_1Y, VM_PERMUTE_0Z, VM_PERMUTE_1X};
    static const vmmask Permute3 = {VM_PERMUTE_1Y, VM_PERMUTE_1Z, VM_PERMUTE_1X, VM_PERMUTE_0W};

    __m128 Q0, Q1, Q2;
    __m128 M00, M11, M22;
    __m128 CQ0, CQ1, C;
    __m128 CX, CY, CZ, CW;
    __m128 SQ1, Scale;
    __m128 Rsq, Sqrt, VEqualsNaN;
    __m128 A, B, P;
    __m128 PermuteSplat, PermuteSplatT;
    __m128 SignB, SignBT;
    __m128 PermuteControl, PermuteControlT;
    __m128 Result;    

    M00 = splatX(m.row0);
    M11 = splatY(m.row1);
    M22 = splatZ(m.row2);

    Q0 = mul4(SignPNNP, M00);
    Q0 = mulAdd4(SignNPNP, M11, Q0);
    Q0 = mulAdd4(SignNNPP, M22, Q0);

    Q1 = add4(Q0, One);

    Rsq = reciprocalSqrt4(Q1);
    VEqualsNaN = isNaN(Rsq);
    Sqrt = mul4(Q1, Rsq);
    Q1 = select(Sqrt, Q1, VEqualsNaN);

    Q1 = mul4(Q1, OneHalf);

    SQ1 = mul4(Rsq, OneHalf);

    //CQ0 = permute(Q0, Q0, Permute0X0X0Y0W.v);
    CQ0 = _mm_shuffle_ps(Q0, Q0, _MM_SHUFFLE(3,1,0,0));
    //CQ1 = permute(Q0, g_XMEpsilon, Permute0Y0Z0Z1W.v);
    CQ1 = _mm_shuffle_ps(Q0, Epsilon, _MM_SHUFFLE(3,3,2,2));
    CQ1 = _mm_shuffle_ps(Q0, CQ1, _MM_SHUFFLE(3,0,2,1));
    C = isGreaterOrEqual(CQ0, CQ1);

    CX = splatX(C);
    CY = splatY(C);
    CZ = splatZ(C);
    CW = splatW(C);

    PermuteSplat = select(SplatZ.v, SplatY.v, CZ);
    SignB = select(SignNPPP, SignPPNP, CZ);
    PermuteControl = select(Permute2.v, Permute1.v, CZ);

    PermuteSplat = select(PermuteSplat, SplatZ.v, CX);
    SignB = select(SignB, SignNPPP, CX);
    PermuteControl = select(PermuteControl, Permute2.v, CX);

    PermuteSplatT = select(PermuteSplat,SplatX.v, CY);
    SignBT = select(SignB, SignPNPP, CY);
    PermuteControlT = select(PermuteControl,Permute0.v, CY);

    PermuteSplat = select(PermuteSplat, PermuteSplatT, CX);
    SignB = select(SignB, SignBT, CX);
    PermuteControl = select(PermuteControl, PermuteControlT, CX);

    PermuteSplat = select(PermuteSplat,SplatW.v, CW);
    SignB = select(SignB, NegOne, CW);
    PermuteControl = select(PermuteControl,Permute3.v, CW);

    Scale = permute(SQ1, SQ1, PermuteSplat);

    P = permute(m.row1, m.row2,PermuteC.v);  // {M10, M12, M20, M21}
    A = permute(m.row0, P, PermuteA.v);      // {M01, M12, M20, M03}
    B = permute(m.row0, P, PermuteB.v);      // {M10, M21, M02, M03}

    Q2 = mulAdd4(SignB, B, A);
    Q2 = mul4(Q2, Scale);

    Result = permute(Q1, Q2, PermuteControl);

    return Result;
}

__forceinline vmvec makeXRotationQ(float const angle)
{
    float sinValue;
    float cosValue;
    sinCos(angle * 0.5f, sinValue, cosValue);
    return make(sinValue, 0.0f, 0.0f, cosValue);
}

__forceinline vmvec makeYRotationQ(float const angle)
{
    float sinValue;
    float cosValue;
    sinCos(angle * 0.5f, sinValue, cosValue);
    return make(0.0f, sinValue, 0.0f, cosValue);
}

__forceinline vmvec makeZRotationQ(float const angle)
{
    float sinValue;
    float cosValue;
    sinCos(angle * 0.5f, sinValue, cosValue);
    return make(0.0f, 0.0f, sinValue, cosValue);
}

__forceinline vmvec makeAxisAngleQ(vmvecFastParam axis, float const angle)
{
    float sinValue;
    float cosValue;
    sinCos(angle * 0.5f, sinValue, cosValue);
    __m128 r = _mm_mul_ps(axis, _mm_set_ps(0, sinValue, sinValue, sinValue));
    return _mm_add_ps(r, _mm_set_ps(cosValue, 0.0f, 0.0f, 0.0f));
}

__forceinline vmvec makeLookAtQ(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up)
{
    static const __m128 one  = {1.0f, 1.0f, 1.0f, 1.0f};
    static const __m128 half = {0.5f, 0.5f, 0.5f, 0.5f};

    __m128 z = normalize3(sub3(at, eye));
    __m128 x = normalize3(cross3(up, z));
    __m128 y = cross3(z, x);

    //  dot = 1.0f + x.x + y.y + z.z
    __m128 xx = _mm_shuffle_ps(x, x, _MM_SHUFFLE(0,0,0,0));
    __m128 yy = _mm_shuffle_ps(y, y, _MM_SHUFFLE(1,1,1,1));
    __m128 zz = _mm_shuffle_ps(z, z, _MM_SHUFFLE(2,2,2,2));
    __m128 dot = _mm_add_ps(xx, yy);
    dot = _mm_add_ps(dot, zz);
    dot = _mm_add_ps(dot, one);

    // s = 0.5f / sqrt(dot)
    // w = 0.5 * sqrt(dot)
    __m128 s = reciprocalSqrt4(dot);
    __m128 w = _mm_mul_ps(half, reciprocal4(s));
    s = _mm_mul_ps(half, s);

    // a = y.z, x.y, z.x, z.x
    // b = z.y, y.x, x.z, x.z
    __m128 t = _mm_shuffle_ps(x, y, _MM_SHUFFLE(2,0,2,1)); // xy, xz, yx, yz
    __m128 a = _mm_shuffle_ps(t, z, _MM_SHUFFLE(0,0,0,3)); // yz, xy, zx, zx
    __m128 b = _mm_shuffle_ps(z, t, _MM_SHUFFLE(2,1,1,1)); // zy, zy, xz, yx
    b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2,2,3,0));        // zy, yx, xz, xz

    // rx = (y.z - z.y) * s
    // ry = (z.x - x.z) * s
    // rz = (x.y - y.x) * s
    // rw = w
    __m128 r = _mm_mul_ps(_mm_sub_ps(a, b), s);
    __m128 rw = _mm_shuffle_ps(r, w, _MM_SHUFFLE(0,0,1,1));
    return _mm_shuffle_ps(r, rw, _MM_SHUFFLE(3,0,3,0));
}

__forceinline vmvec addQ(vmvecFastParam q1, vmvecFastParam q2)
{
    return add4(q1, q2);
}

__forceinline vmvec subQ(vmvecFastParam q1, vmvecFastParam q2)
{
    return sub4(q1, q2);
}

__forceinline vmvec mulQ(vmvecFastParam q1, vmvecFastParam q2)
{
    static const __m128 ControlWZYX = { 1.0f,-1.0f, 1.0f,-1.0f};
    static const __m128 ControlZWXY = { 1.0f, 1.0f,-1.0f,-1.0f};
    static const __m128 ControlYXWZ = {-1.0f, 1.0f, 1.0f,-1.0f};

    // Copy to SSE registers and use as few as possible for x86
    __m128 Q2X = q2;
    __m128 Q2Y = q2;
    __m128 Q2Z = q2;
    __m128 vResult = q2;
    // Splat with one instruction
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(3,3,3,3));
    Q2X = _mm_shuffle_ps(Q2X,Q2X,_MM_SHUFFLE(0,0,0,0));
    Q2Y = _mm_shuffle_ps(Q2Y,Q2Y,_MM_SHUFFLE(1,1,1,1));
    Q2Z = _mm_shuffle_ps(Q2Z,Q2Z,_MM_SHUFFLE(2,2,2,2));
    // Retire Q1 and perform Q1*Q2W
    vResult = _mm_mul_ps(vResult,q1);
    __m128 Q1Shuffle = q1;
    // Shuffle the copies of Q1
    Q1Shuffle = _mm_shuffle_ps(Q1Shuffle,Q1Shuffle,_MM_SHUFFLE(0,1,2,3));
    // Mul by Q1WZYX
    Q2X = _mm_mul_ps(Q2X,Q1Shuffle);
    Q1Shuffle = _mm_shuffle_ps(Q1Shuffle,Q1Shuffle,_MM_SHUFFLE(2,3,0,1));
    // Flip the signs on y and z
    Q2X = _mm_mul_ps(Q2X,ControlWZYX);
    // Mul by Q1ZWXY
    Q2Y = _mm_mul_ps(Q2Y,Q1Shuffle);
    Q1Shuffle = _mm_shuffle_ps(Q1Shuffle,Q1Shuffle,_MM_SHUFFLE(0,1,2,3));
    // Flip the signs on z and w
    Q2Y = _mm_mul_ps(Q2Y,ControlZWXY);
    // Mul by Q1YXWZ
    Q2Z = _mm_mul_ps(Q2Z,Q1Shuffle);
    vResult = _mm_add_ps(vResult,Q2X);
    // Flip the signs on x and w
    Q2Z = _mm_mul_ps(Q2Z,ControlYXWZ);
    Q2Y = _mm_add_ps(Q2Y,Q2Z);
    vResult = _mm_add_ps(vResult,Q2Y);
    return vResult;
}

__forceinline vmvec conjugateQ(vmvecFastParam q)
{
    static const __m128 NegativeOne3 = {-1.0f, -1.0f, -1.0f, 1.0f};
    return _mm_mul_ps(q, NegativeOne3);
}

__forceinline vmvec normalizeQ(vmvecFastParam q)
{
    return normalize4(q);
}

__forceinline vmvec normalizeEstQ(vmvecFastParam q)
{
    return normalizeEst4(q);
}

__forceinline vmvec rotateQ(vmvecFastParam q, vmvecFastParam axis, float const angle)
{
    __m128 r;
    const float h = angle * 0.5f;
    __m128 negQ_yzxz = neg4(_mm_shuffle_ps(q, q, _MM_SHUFFLE(1,2,0,2)));
    __m128 a0 = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(2,0,1,2));
    __m128 t0 = _mm_mul_ps(a0, negQ_yzxz);
    __m128 negQ_wy_xy = _mm_shuffle_ps(q, negQ_yzxz, _MM_SHUFFLE(3,1,0,1));
    __m128 q0 = _mm_shuffle_ps(q, negQ_wy_xy, _MM_SHUFFLE(2,0,1,3));
    a0 = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(1,2,0,1));
    t0 = _mm_add_ps(t0, _mm_mul_ps(a0, q0));
    q0 = _mm_shuffle_ps(q, negQ_wy_xy, _MM_SHUFFLE(3,3,0,2));
    a0 = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(0,1,2,0));
    t0 = _mm_add_ps(t0, _mm_mul_ps(a0, q0));
    r = _mm_add_ps(q, _mm_mul_ps(t0, _mm_set_ps1(h)));
    return r;
}

__forceinline vmvec lerpQ(vmvecFastParam from, vmvecFastParam to, vmvecFastParam t)
{
    static const __m128 one  = {1.0f, 1.0f, 1.0f, 1.0f};
    static const __m128 zero = {0.0f, 0.0f, 0.0f, 0.0f};

    __m128 cosom = dot4(from, to);
    __m128 tneg = _mm_sub_ps(zero, t);
    __m128 scale0 = _mm_sub_ps(one, t);
    __m128 cmp = _mm_cmpge_ps(cosom, zero);
    __m128 scale1 = _mm_or_ps(_mm_and_ps(cmp, t), _mm_andnot_ps(cmp, tneg));
    __m128 v = _mm_add_ps(_mm_mul_ps(scale0, from), _mm_mul_ps(scale1, to));
    __m128 s = reciprocalSqrt4(dot4(v, v));
    return _mm_mul_ps(v, s);
}

__forceinline vmvec slerpQ(vmvecFastParam q1, vmvecFastParam q2, vmvecFastParam t)
{
    // Result = Q0 * sin((1.0 - t) * Omega) / sin(Omega) + Q1 * sin(t * Omega) / sin(Omega)
    __m128 Omega;
    __m128 CosOmega;
    __m128 SinOmega;
    __m128 V01;
    __m128 S0;
    __m128 S1;
    __m128 Sign;
    __m128 Control;
    __m128 Result;
    __m128 Zero;
    static const __m128 one = {1.0f, 1.0f, 1.0f, 1.0f};
    static const __m128 negOne = {-1.0f, -1.0f, -1.0f, -1.0f};
    static const __m128 identity0 = {1.0f, 0.0f, 0.0f, 0.0f};
    static const __m128 OneMinusEpsilon = {1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f};
    static const vmmask SignMask2 = {0x80000000,0x00000000,0x00000000,0x00000000};
    static const vmmask MaskXY = {0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000};

    CosOmega = dot4(q1, q2);

    Zero = splatZero();
    Control = isLess(CosOmega, Zero);
    Sign = select(one, negOne, Control);

    CosOmega = _mm_mul_ps(CosOmega, Sign);

    Control = isLess(CosOmega, OneMinusEpsilon);

    SinOmega = _mm_mul_ps(CosOmega,CosOmega);
    SinOmega = _mm_sub_ps(one, SinOmega);
    SinOmega = _mm_sqrt_ps(SinOmega);

    Omega = atanPositive4(SinOmega, CosOmega);

    V01 = _mm_shuffle_ps(t, t, _MM_SHUFFLE(2,3,0,1));
    V01 = _mm_and_ps(V01, MaskXY.v);
    V01 = _mm_xor_ps(V01, SignMask2.v);
    V01 = _mm_add_ps(identity0, V01);

    S0 = _mm_mul_ps(V01, Omega);
    S0 = sinZeroHalfPI4(S0);
    S0 = _mm_div_ps(S0, SinOmega);

    S0 = select(V01, S0, Control);

    S1 = splatY(S0);
    S0 = splatX(S0);

    S1 = _mm_mul_ps(S1, Sign);
    Result = _mm_mul_ps(q1, S0);
    S1 = _mm_mul_ps(S1, q2);
    Result = _mm_add_ps(Result,S1);
    return Result;
}

__forceinline vmvec extractXAxis(vmvecFastParam q)
{
    return q;
    /*float y2 = Add(q.y, q.y);
    float z2 = Add(q.z, q.z);
    float y2x = y2 * q.x;
    float y2y = y2 * q.y;
    float y2w = y2 * q.w;
    float z2x = z2 * q.x;
    float z2z = z2 * q.z;
    float z2w = z2 * q.w;
    return vmVectorSet(Sub(1.0f, Add(y2y, z2z)), Add(y2x, z2w), Sub(z2x, y2w), 0.0f);*/
}

__forceinline vmvec extractYAxis(vmvecFastParam q)
{
#if 0
    __m128 q2 = _mm_add_ps(q, q);
    __m128 a = _mm_mul_ps(q2, 
    return vmVectorSet(
                (q2.y * q.x) - (q2.z * q.w),
        1.0f - ((q2.x * q.x) + (q2.z * q.z)),
                (q2.z * q.y) + (q2.x * q.w),
        0.0f);
#endif
    return q;
    /*float x2 = Add(q.x, q.x);
    float y2 = Add(q.y, q.y);
    float z2 = Add(q.z, q.z);
    float x2x = x2 * q.x;
    float x2w = x2 * q.w;
    float y2x = y2 * q.x;
    float z2y = z2 * q.y;
    float z2z = z2 * q.z;
    float z2w = z2 * q.w;
    return vmVectorSet(Sub(y2x, z2w), Sub(1.0f, Add(x2x, z2z)), Add(z2y, x2w), 0.0f);*/
}

__forceinline vmvec extractZAxis(vmvecFastParam q)
{
    return q;
    /*float x2 = Add(q.x, q.x);
    float y2 = Add(q.y, q.y);
    float z2 = Add(q.z, q.z);
    float x2x = x2 * q.x;
    float x2w = x2 * q.w;
    float y2y = y2 * q.y;
    float y2w = z2 * q.w;
    float z2x = z2 * q.x;
    float z2y = z2 * q.y;
    return vmVectorSet(Add(z2x, y2w), Sub(z2y, x2w), Sub(1.0f, Add(x2x, y2y)), 0.0f);*/
}

} // end of vm
} // end of mathos

#endif
