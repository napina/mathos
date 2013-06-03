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
#ifndef mathos_vmvector3_sse_inl
#define mathos_vmvector3_sse_inl

namespace mathos {
namespace vm {

__forceinline vmvec add3(vmvecFastParam v1, vmvecFastParam v2)
{
    return add4(v1, v2);
}

__forceinline vmvec sub3(vmvecFastParam v1, vmvecFastParam v2)
{
    return sub4(v1, v2);
}

__forceinline vmvec cross3(vmvecFastParam v1, vmvecFastParam v2)
{
    const __m128 a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3,0,2,1));
    const __m128 b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3,1,0,2));
    const __m128 c = _mm_mul_ps(a, b);
    const __m128 d = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3,0,2,1));
    const __m128 e = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,1,0,2));
    const __m128 f = _mm_mul_ps(d, e);
    return _mm_sub_ps(c, f);
}

__forceinline vmvec dot3(vmvecFastParam v1, vmvecFastParam v2)
{
    const __m128 m   = _mm_mul_ps(v1, v2);
    const __m128 yz  = _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,1,2,1));
    const __m128 xy  = _mm_add_ss(m, yz);
    const __m128 z   = _mm_shuffle_ps(yz, yz, _MM_SHUFFLE(1,1,1,1));
    const __m128 xyz = _mm_add_ss(xy, z);
    return _mm_shuffle_ps(xyz, xyz, _MM_SHUFFLE(0,0,0,0));
}

__forceinline vmvec length3(vmvecFastParam v)
{
    return sqrt4(dot3(v, v));
}

__forceinline vmvec lengthEst3(vmvecFastParam v)
{
    return sqrtEst4(dot3(v, v));
}

__forceinline vmvec normalize3(vmvecFastParam v)
{
    static const __m128 half = { 0.5f, 0.5f, 0.5f, 0.5f };
    static const __m128 three = { 3.0f, 3.0f, 3.0f, 3.0f };

    const __m128 lenSq = dot3(v, v);
    const __m128 r = _mm_rsqrt_ss(lenSq);
    const __m128 a = _mm_sub_ss(three, _mm_mul_ss(_mm_mul_ss(lenSq, r), r));
    const __m128 b = _mm_mul_ss(half, r);
    const __m128 rq = _mm_mul_ss(a, b);
    return _mm_mul_ps(v, splatX(rq));
}

__forceinline vmvec normalizeEst3(vmvecFastParam v)
{
    return _mm_mul_ps(v, _mm_rsqrt_ps(dot3(v, v)));
}

__forceinline vmvec reflect3(vmvecFastParam v, vmvecFastParam normal)
{
    __m128 r = dot3(v, normal);
    r = _mm_add_ps(r, r);
    r = _mm_mul_ps(r, normal);
    return _mm_sub_ps(v, r);
}

__forceinline vmvec refract3(vmvecFastParam v, vmvecFastParam normal, float const refraction)
{
    const __m128 index = _mm_set_ps1(refraction);
    return refract3(v, normal, index);
}

__forceinline vmvec refract3(vmvecFastParam v, vmvecFastParam normal, vmvecFastParam refraction)
{
    // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
    // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
    const __m128 IDotN = dot3(v, normal);
    // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
    __m128 R = _mm_mul_ps(IDotN, IDotN);
    R = _mm_sub_ps(splatOne(),R);
    R = _mm_mul_ps(R, refraction);
    R = _mm_mul_ps(R, refraction);
    R = _mm_sub_ps(splatOne(),R);

    __m128 vResult = _mm_cmple_ps(R, splatZero());
    if (_mm_movemask_ps(vResult)==0x0f)
    {
        // Total internal reflection
        vResult = splatZero();
    }
    else
    {
        // R = RefractionIndex * IDotN + sqrt(R)
        R = _mm_sqrt_ps(R);
        vResult = _mm_mul_ps(refraction,IDotN);
        R = _mm_add_ps(R,vResult);
        // Result = RefractionIndex * Incident - Normal * R
        vResult = _mm_mul_ps(refraction, v);
        R = _mm_mul_ps(R,normal);
        vResult = _mm_sub_ps(vResult,R);
    }
    return vResult;
}

__forceinline vmvec transform3(vmvecFastParam v, vmvecFastParam quat)
{
    static const vmmask mask = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};

    const __m128 a = _mm_and_ps(v, mask.v);
    const __m128 q = conjugateQ(quat);
    const __m128 r = mulQ(q, a);
    return mulQ(r, quat);
}

__forceinline vmvec transform3(vmvecFastParam v, vmmatParam m)
{
    const __m128 x   = _mm_mul_ps(m.row0, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0)));
    const __m128 y   = _mm_mul_ps(m.row1, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1)));
    const __m128 xy  = _mm_add_ps(x, y);
    const __m128 z   = _mm_mul_ps(m.row2, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,2,2,2)));
    const __m128 xyz = _mm_add_ps(xy, z);
    return _mm_add_ps(xyz, m.row3);
}

__forceinline vmvec transform3(vmvecFastParam v, vmtransParam t)
{
    const __m128 p = _mm_mul_ps(v, splatW(t.posScale));
    const __m128 r = transform3(p, t.quat);
    return _mm_add_ps(r, t.posScale);
}

__forceinline vmvec transformNormal(vmvecFastParam v, vmvecFastParam quat)
{
    return transform3(v, quat);
}

__forceinline vmvec transformNormal(vmvecFastParam v, vmmatParam m)
{
    const __m128 x  = _mm_mul_ps(m.row0, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0)));
    const __m128 y  = _mm_mul_ps(m.row1, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1)));
    const __m128 xy = _mm_add_ps(x, y);
    const __m128 z  = _mm_mul_ps(m.row2, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,2,2,2)));
    return _mm_add_ps(xy, z);
}

__forceinline vmvec transformNormal(vmvecFastParam v, vmtransParam t)
{
    return transform3(v, t.quat);
}

} // end of vm
} // end of mathos

#endif
