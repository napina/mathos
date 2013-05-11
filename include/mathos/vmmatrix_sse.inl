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
#ifndef mathos_vmmatrix_sse_inl
#define mathos_vmmatrix_sse_inl

namespace mathos {
namespace vm {

__forceinline vmmat identityM()
{
    static const __m128 identity0 = {1.0f, 0.0f, 0.0f, 0.0f};

    vmmat m;
    m.row0 = identity0;
    m.row1 = _mm_shuffle_ps(identity0, identity0, _MM_SHUFFLE(2,1,0,3));
    m.row2 = _mm_shuffle_ps(identity0, identity0, _MM_SHUFFLE(1,0,3,2));
    m.row3 = _mm_shuffle_ps(identity0, identity0, _MM_SHUFFLE(0,3,2,1));
    return m;
}

__forceinline vmmat makeTranslationM(vmvecParam v)
{
    static const __m128 identity0 = {1.0f, 0.0f, 0.0f, 0.0f};
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmmat m;
    m.row0 = identity0;
    m.row1 = _mm_shuffle_ps(identity0, identity0, _MM_SHUFFLE(2,1,0,3));
    m.row2 = _mm_shuffle_ps(identity0, identity0, _MM_SHUFFLE(1,0,3,2));
    m.row3 = _mm_or_ps(_mm_and_ps(v, mask3.v), identity3);
    return m;
}

__forceinline vmmat makeScaleM(vmvecParam s)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmmat m;
    const __m128 s0 = _mm_and_ps(s, mask3.v);
    m.row0 = _mm_shuffle_ps(s0, s0, _MM_SHUFFLE(3,3,3,0));
    m.row1 = _mm_shuffle_ps(s0, s0, _MM_SHUFFLE(3,3,1,3));
    m.row2 = _mm_shuffle_ps(s0, s0, _MM_SHUFFLE(3,2,3,3));
    m.row3 = identity3;
    return m;
}

__forceinline vmmat makeXRotationM(float const angle)
{
    static const __m128 identity0 = {1.0f, 0.0f, 0.0f, 0.0f};
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const __m128 negateY = {1.0f, -1.0f, 1.0f, 1.0f};

    float sin;
    float cos;
    sinCos(angle, sin, cos);

    const __m128 s = _mm_set_ss(sin);
    const __m128 c = _mm_set_ss(cos);
    const __m128 _0cs0 = _mm_shuffle_ps(c, s, _MM_SHUFFLE(3,0,0,3));
    vmmat m;
    m.row0 = identity0;
    m.row1 = _0cs0;
    m.row2 = _mm_mul_ps(_mm_shuffle_ps(_0cs0, _0cs0, _MM_SHUFFLE(3,1,2,0)), negateY);
    m.row3 = identity3;
    return m;
}

__forceinline vmmat makeYRotationM(float const angle)
{
    static const __m128 identity1 = {0.0f, 1.0f, 0.0f, 0.0f};
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const __m128 negateZ = {1.0f, 1.0f, -1.0f, 1.0f};

    float sin;
    float cos;
    sinCos(angle, sin, cos);

    const __m128 s = _mm_set_ss(sin);
    const __m128 c = _mm_set_ss(cos);
    const __m128 s0c0 = _mm_shuffle_ps(s,c,_MM_SHUFFLE(3,0,3,0));
    vmmat m;
    m.row2 = s0c0;
    m.row0 = _mm_mul_ps(_mm_shuffle_ps(s0c0, s0c0, _MM_SHUFFLE(3,0,1,2)), negateZ);
    m.row1 = identity1;
    m.row3 = identity3;
    return m;
}

__forceinline vmmat makeZRotationM(float const angle)
{
    static const __m128 identity2 = {0.0f, 0.0f, 1.0f, 0.0f};
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const __m128 negateX = {-1.0f, 1.0f, 1.0f, 1.0f};

    float sin;
    float cos;
    sinCos(angle, sin, cos);

    const __m128 s = _mm_set_ss(sin);
    const __m128 c = _mm_set_ss(cos);
    const __m128 cs00 = _mm_unpacklo_ps(c,s);
    vmmat m;
    m.row0 = cs00;
    m.row1 = _mm_mul_ps(_mm_shuffle_ps(cs00, cs00, _MM_SHUFFLE(3,2,0,1)), negateX);
    m.row2 = identity2;
    m.row3 = identity3;
    return m;
}

__forceinline vmmat makeAxisAngleM(vmvecParam axis, float const angle)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    __m128 N0, N1;
    __m128 V0, V1, V2;
    __m128 R0, R1, R2;
    __m128 C0, C1, C2;

    float sinAngle;
    float cosAngle;
    mathos::sinCos(angle, sinAngle, cosAngle);

    C2 = _mm_set_ps1(1.0f - cosAngle);
    C1 = _mm_set_ps1(cosAngle);
    C0 = _mm_set_ps1(sinAngle);

    N0 = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(3,0,2,1));
    N1 = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(3,1,0,2));

    V0 = _mm_mul_ps(C2, N0);
    V0 = _mm_mul_ps(V0, N1);

    R0 = _mm_mul_ps(C2, axis);
    R0 = _mm_mul_ps(R0, axis);
    R0 = _mm_add_ps(R0, C1);

    R1 = _mm_mul_ps(C0, axis);
    R1 = _mm_add_ps(R1, V0);
    R2 = _mm_mul_ps(C0, axis);
    R2 = _mm_sub_ps(V0,R2);

    V0 = _mm_and_ps(R0, mask3.v);
    V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2,1,2,0));
    V1 = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0,3,2,1));
    V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0,0,1,1));
    V2 = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2,0,2,0));

    R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1,0,3,0));
    R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1,3,2,0));

    vmmat M;
    M.row0 = R2;
    R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3,2,3,1));
    R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1,3,0,2));
    M.row1 = R2;
    V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3,2,1,0));
    M.row2 = V2;
    M.row3 = identity3;
    return M;
}

__forceinline vmmat toMatrix(vmvecFastParam q)
{
    vmmat M;
    __m128 Q0, Q1;
    __m128 V0, V1, V2;
    __m128 R0, R1, R2;
    static const __m128 constant1110 = {1.0f, 1.0f, 1.0f, 0.0f};
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    Q0 = _mm_add_ps(q, q);
    Q1 = _mm_mul_ps(q, Q0);

    V0 = _mm_shuffle_ps(Q1, Q1, _MM_SHUFFLE(3,0,0,1));
    V0 = _mm_and_ps(V0, mask3.v);
    V1 = _mm_shuffle_ps(Q1, Q1, _MM_SHUFFLE(3,1,2,2));
    V1 = _mm_and_ps(V1, mask3.v);
    R0 = _mm_sub_ps(constant1110, V0);
    R0 = _mm_sub_ps(R0, V1);

    V0 = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3,1,0,0));
    V1 = _mm_shuffle_ps(Q0, Q0, _MM_SHUFFLE(3,2,1,2));
    V0 = _mm_mul_ps(V0, V1);

    V1 = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3,3,3,3));
    V2 = _mm_shuffle_ps(Q0, Q0, _MM_SHUFFLE(3,0,2,1));
    V1 = _mm_mul_ps(V1, V2);

    R1 = _mm_add_ps(V0, V1);
    R2 = _mm_sub_ps(V0, V1);

    V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1,0,2,1));
    V0 = _mm_shuffle_ps(V0, V0, _MM_SHUFFLE(1,3,2,0));
    V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2,2,0,0));
    V1 = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(2,0,2,0));

    Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1,0,3,0));
    Q1 = _mm_shuffle_ps(Q1, Q1, _MM_SHUFFLE(1,3,2,0));
    M.row0 = Q1;
    Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3,2,3,1));
    Q1 = _mm_shuffle_ps(Q1, Q1, _MM_SHUFFLE(1,3,0,2));
    M.row1 = Q1;
    Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3,2,1,0));
    M.row2 = Q1;
    M.row3 = identity3;
    return M;
}

__forceinline vmmat toMatrix(vmtransParam t)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmmat m = toMatrix(t.quat);
    vmmat r;
    const __m128 s = splatW(t.posScale);
    r.row0 = _mm_mul_ps(m.row0, s);
    r.row1 = _mm_mul_ps(m.row1, s);
    r.row2 = _mm_mul_ps(m.row2, s);
    r.row3 = _mm_or_ps(_mm_and_ps(t.posScale, mask3.v), identity3);
    return r;
}

__forceinline vmmat makeLookAtM(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmmat r;
    r.row2 = normalize3(sub3(at, eye));
    r.row0 = normalize3(cross3(up, r.row2));
    r.row1 = cross3(r.row2, r.row0);
    r.row3 = _mm_or_ps(_mm_and_ps(eye, mask3.v), identity3);
    return r;
}

__forceinline vmmat makeLookAtRotationM(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};

    vmmat r;
    r.row2 = normalize3(sub3(at, eye));
    r.row0 = normalize3(cross3(up, r.row2));
    r.row1 = cross3(r.row2, r.row0);
    r.row3 = identity3;
    return r;
}

__forceinline vmmat transposeM(vmmatParam m)
{
    const __m128 tmp0 = _mm_shuffle_ps(m.row0, m.row1, 0x44);
    const __m128 tmp2 = _mm_shuffle_ps(m.row0, m.row1, 0xEE);
    const __m128 tmp1 = _mm_shuffle_ps(m.row2, m.row3, 0x44);
    const __m128 tmp3 = _mm_shuffle_ps(m.row2, m.row3, 0xEE);
    vmmat r;
    r.row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
    r.row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
    r.row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
    r.row3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
    return r;
}

__forceinline vmmat orthonormalizeM(vmmatParam m)
{
    // TODO
    return m;
}

__forceinline vmmat mulM(vmmatParam m1, vmmatParam m2)
{
    vmmat m;
    __m128 q, t;
    q = _mm_mul_ps(m2.row0, splatX(m1.row0));
    t = _mm_mul_ps(m2.row1, splatY(m1.row0));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row2, splatZ(m1.row0));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row3, splatW(m1.row0));
    q = _mm_add_ps(q, t);
    m.row0 = q;
    q = _mm_mul_ps(m2.row0, splatX(m1.row1));
    t = _mm_mul_ps(m2.row1, splatY(m1.row1));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row2, splatZ(m1.row1));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row3, splatW(m1.row1));
    q = _mm_add_ps(q, t);
    m.row1 = q;
    q = _mm_mul_ps(m2.row0, splatX(m1.row2));
    t = _mm_mul_ps(m2.row1, splatY(m1.row2));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row2, splatZ(m1.row2));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row3, splatW(m1.row2));
    q = _mm_add_ps(q, t);
    m.row2 = q;
    q = _mm_mul_ps(m2.row0, splatX(m1.row3));
    t = _mm_mul_ps(m2.row1, splatY(m1.row3));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row2, splatZ(m1.row3));
    q = _mm_add_ps(q, t);
    t = _mm_mul_ps(m2.row3, splatW(m1.row3));
    q = _mm_add_ps(q, t);
    m.row3 = q;
    return m;
}

__forceinline vmmat mulTransposeM(vmmatParam m1, vmmatParam m2)
{
    return transposeM(mulM(m1, m2));
}

__forceinline vmmat translateM(vmmatParam m, vmvecParam v)
{
    // TODO should w be ignored?
    vmmat r;
    __m128 t = _mm_mul_ps(v, splatW(m.row0));
    r.row0 = _mm_add_ps(m.row0, t);
    //r.row0.w = m.row0.w;
    t = _mm_mul_ps(v, splatW(m.row1));
    r.row1 = _mm_add_ps(m.row1, t);
    //r.row1.w = m.row1.w;
    t = _mm_mul_ps(v, splatW(m.row2));
    r.row2 = _mm_add_ps(m.row2, t);
    //r.row2.w = m.row2.w;
    t = _mm_mul_ps(v, splatW(m.row3));
    r.row3 = _mm_add_ps(m.row3, t);
    //r.row3.w = m.row3.w;
    return r;
}

__forceinline vmmat scaleM(vmmatParam m, vmvecParam s)
{
    // TODO should w be ignored?
    vmmat r;
    r.row0 = _mm_mul_ps(m.row0, s);
    //r.row0.w = m.row0.w;
    r.row1 = _mm_mul_ps(m.row1, s);
    //r.row1.w = m.row1.w;
    r.row2 = _mm_mul_ps(m.row2, s);
    //r.row2.w = m.row2.w;
    r.row3 = _mm_mul_ps(m.row3, s);
    //r.row3.w = m.row3.w;
    return r;
}

__forceinline vmmat invertM(vmmatParam m)
{
    vmmat MT = transposeM(m);
    __m128 V00 = _mm_shuffle_ps(MT.row2, MT.row2, _MM_SHUFFLE(1,1,0,0));
    __m128 V10 = _mm_shuffle_ps(MT.row3, MT.row3, _MM_SHUFFLE(3,2,3,2));
    __m128 V01 = _mm_shuffle_ps(MT.row0, MT.row0, _MM_SHUFFLE(1,1,0,0));
    __m128 V11 = _mm_shuffle_ps(MT.row1, MT.row1, _MM_SHUFFLE(3,2,3,2));
    __m128 V02 = _mm_shuffle_ps(MT.row2, MT.row0, _MM_SHUFFLE(2,0,2,0));
    __m128 V12 = _mm_shuffle_ps(MT.row3, MT.row1, _MM_SHUFFLE(3,1,3,1));

    __m128 D0 = _mm_mul_ps(V00,V10);
    __m128 D1 = _mm_mul_ps(V01,V11);
    __m128 D2 = _mm_mul_ps(V02,V12);

    V00 = _mm_shuffle_ps(MT.row2, MT.row2, _MM_SHUFFLE(3,2,3,2));
    V10 = _mm_shuffle_ps(MT.row3, MT.row3, _MM_SHUFFLE(1,1,0,0));
    V01 = _mm_shuffle_ps(MT.row0, MT.row0, _MM_SHUFFLE(3,2,3,2));
    V11 = _mm_shuffle_ps(MT.row1, MT.row1, _MM_SHUFFLE(1,1,0,0));
    V02 = _mm_shuffle_ps(MT.row2, MT.row0, _MM_SHUFFLE(3,1,3,1));
    V12 = _mm_shuffle_ps(MT.row3, MT.row1, _MM_SHUFFLE(2,0,2,0));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    D0 = _mm_sub_ps(D0, V00);
    D1 = _mm_sub_ps(D1, V01);
    D2 = _mm_sub_ps(D2, V02);
    V11 = _mm_shuffle_ps(D0,      D2,      _MM_SHUFFLE(1,1,3,1));
    V00 = _mm_shuffle_ps(MT.row1, MT.row1, _MM_SHUFFLE(1,0,2,1));
    V10 = _mm_shuffle_ps(V11,     D0,      _MM_SHUFFLE(0,3,0,2));
    V01 = _mm_shuffle_ps(MT.row0, MT.row0, _MM_SHUFFLE(0,1,0,2));
    V11 = _mm_shuffle_ps(V11,     D0,      _MM_SHUFFLE(2,1,2,1));
    __m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3,3,3,1));
    V02 = _mm_shuffle_ps(MT.row3, MT.row3, _MM_SHUFFLE(1,0,2,1));
    V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0,3,0,2));
    __m128 V03 = _mm_shuffle_ps(MT.row2, MT.row2,_MM_SHUFFLE(0,1,0,2));
    V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2,1,2,1));

    __m128 C0 = _mm_mul_ps(V00, V10);
    __m128 C2 = _mm_mul_ps(V01, V11);
    __m128 C4 = _mm_mul_ps(V02, V12);
    __m128 C6 = _mm_mul_ps(V03, V13);

    V11 = _mm_shuffle_ps(D0,      D2,      _MM_SHUFFLE(0,0,1,0));
    V00 = _mm_shuffle_ps(MT.row1, MT.row1, _MM_SHUFFLE(2,1,3,2));
    V10 = _mm_shuffle_ps(D0,      V11,     _MM_SHUFFLE(2,1,0,3));
    V01 = _mm_shuffle_ps(MT.row0, MT.row0, _MM_SHUFFLE(1,3,2,3));
    V11 = _mm_shuffle_ps(D0,      V11,     _MM_SHUFFLE(0,2,1,2));
    V13 = _mm_shuffle_ps(D1,      D2,      _MM_SHUFFLE(2,2,1,0));
    V02 = _mm_shuffle_ps(MT.row3, MT.row3, _MM_SHUFFLE(2,1,3,2));
    V12 = _mm_shuffle_ps(D1,      V13,     _MM_SHUFFLE(2,1,0,3));
    V03 = _mm_shuffle_ps(MT.row2, MT.row2, _MM_SHUFFLE(1,3,2,3));
    V13 = _mm_shuffle_ps(D1,      V13,     _MM_SHUFFLE(0,2,1,2));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    V03 = _mm_mul_ps(V03, V13);
    C0 = _mm_sub_ps(C0, V00);
    C2 = _mm_sub_ps(C2, V01);
    C4 = _mm_sub_ps(C4, V02);
    C6 = _mm_sub_ps(C6, V03);

    V00 = _mm_shuffle_ps(MT.row1, MT.row1, _MM_SHUFFLE(0,3,0,3));
    V10 = _mm_shuffle_ps(D0,      D2,      _MM_SHUFFLE(1,0,2,2));
    V10 = _mm_shuffle_ps(V10,     V10,     _MM_SHUFFLE(0,2,3,0));
    V01 = _mm_shuffle_ps(MT.row0, MT.row0, _MM_SHUFFLE(2,0,3,1));
    V11 = _mm_shuffle_ps(D0,      D2,      _MM_SHUFFLE(1,0,3,0));
    V11 = _mm_shuffle_ps(V11,     V11,     _MM_SHUFFLE(2,1,0,3));
    V02 = _mm_shuffle_ps(MT.row3, MT.row3, _MM_SHUFFLE(0,3,0,3));
    V12 = _mm_shuffle_ps(D1,      D2,      _MM_SHUFFLE(3,2,2,2));
    V12 = _mm_shuffle_ps(V12,     V12,     _MM_SHUFFLE(0,2,3,0));
    V03 = _mm_shuffle_ps(MT.row2, MT.row2, _MM_SHUFFLE(2,0,3,1));
    V13 = _mm_shuffle_ps(D1,      D2,      _MM_SHUFFLE(3,2,3,0));
    V13 = _mm_shuffle_ps(V13,     V13,     _MM_SHUFFLE(2,1,0,3));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    V03 = _mm_mul_ps(V03, V13);
    __m128 C1 = _mm_sub_ps(C0, V00);
    C0 = _mm_add_ps(C0, V00);
    __m128 C3 = _mm_add_ps(C2, V01);
    C2 = _mm_sub_ps(C2, V01);
    __m128 C5 = _mm_sub_ps(C4, V02);
    C4 = _mm_add_ps(C4, V02);
    __m128 C7 = _mm_add_ps(C6, V03);
    C6 = _mm_sub_ps(C6, V03);

    C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3,1,2,0));
    C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3,1,2,0));
    C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3,1,2,0));
    C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3,1,2,0));
    C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3,1,2,0));
    C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3,1,2,0));
    C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3,1,2,0));
    C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3,1,2,0));
    // Get the determinate
    __m128 vTemp = dot4(C0, MT.row0);
    //*pDeterminant = vTemp;
    vTemp = reciprocal4(vTemp);
    vmmat mResult;
    mResult.row0 = _mm_mul_ps(C0, vTemp);
    mResult.row1 = _mm_mul_ps(C2, vTemp);
    mResult.row2 = _mm_mul_ps(C4, vTemp);
    mResult.row3 = _mm_mul_ps(C6, vTemp);
    return mResult;
}

__forceinline vmmat invertRotationTranslationM(vmmatParam m)
{
    return m;
    /*
    // 9 mult, 6 add, 3 neg
    vmmat r;
    // transpose of rotation
    r.row0.x = m.row0.x;
    r.row0.y = m.row1.x;
    r.row0.z = m.row2.x;
    r.row0.w = 0.0f;
    r.row1.x = m.row0.y;
    r.row1.y = m.row1.y;
    r.row1.z = m.row2.y;
    r.row1.w = 0.0f;
    r.row2.x = m.row0.z;
    r.row2.y = m.row1.z;
    r.row2.z = m.row2.z;
    r.row2.w = 0.0f;
    // translation
    r.row3.x = Negate(Add3(Multiply(m.row0.x, m.row3.x), Multiply(m.row0.y, m.row3.y), Multiply(m.row0.z, m.row3.z)));
    r.row3.y = Negate(Add3(Multiply(m.row1.x, m.row3.x), Multiply(m.row1.y, m.row3.y), Multiply(m.row1.z, m.row3.z)));
    r.row3.z = Negate(Add3(Multiply(m.row2.x, m.row3.x), Multiply(m.row2.y, m.row3.y), Multiply(m.row2.z, m.row3.z)));
    r.row3.w = 1.0f;
    return r;*/
}

} // end of vm
} // end of mathos

#endif
