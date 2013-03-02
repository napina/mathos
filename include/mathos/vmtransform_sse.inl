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
#ifndef mathos_vmtransform_sse_inl
#define mathos_vmtransform_sse_inl

namespace mathos {
namespace vm {

__forceinline vmtrans identityT()
{
    vmtrans identity = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    return identity;
}

__forceinline vmtrans makeLookAtT(vmvecFastParam eye, vmvecFastParam at, vmvecFastParam up)
{
    static const __m128 identity3 = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmtrans r;
    r.posScale = _mm_or_ps(_mm_and_ps(eye, mask3.v), identity3);
    r.quat = makeLookAtQ(eye, at, up);
    return r;
}

__forceinline vmtrans translateT(vmtransParam t, vmvecParam v)
{
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    vmtrans r;
    r.posScale = _mm_add_ps(t.posScale, _mm_and_ps(v, mask3.v));
    r.quat = t.quat;
    return r;
}

__forceinline vmtrans scaleT(vmtransParam t, float const s)
{
    vmtrans r;
    r.posScale = _mm_mul_ps(t.posScale, _mm_set_ps(s, 1.0f, 1.0f, 1.0f));
    r.quat = t.quat;
    return r;
}

__forceinline vmtrans scaleT(vmtransParam t, vmvecParam v)
{
    static const __m128 one3 = {1.0f, 1.0f, 1.0f, 0.0f};
    static const vmmask maskW = {0, 0, 0, 0xffffffff};

    vmtrans r;
    const __m128 s = _mm_or_ps(one3, _mm_and_ps(v, maskW.v));
    r.posScale = _mm_mul_ps(t.posScale, s);
    r.quat = t.quat;
    return r;
}

__forceinline vmtrans rotateT(vmtransParam t, vmvecParam q)
{
    vmtrans r;
    r.posScale = t.posScale;
    r.quat = mulQ(q, t.quat);
    return r;
}

__forceinline vmtrans invertT(vmtransParam t)
{
    static const __m128 negate3 = {-1.0f, -1.0f, -1.0f, 1.0f};
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    const __m128 invQuat = conjugateQ(t.quat);
    const __m128 invPos = transform3(t.posScale, invQuat);
    const __m128 scale = _mm_mul_ps(reciprocal4(splatW(t.posScale)), negate3);
    const __m128 p = _mm_or_ps(_mm_and_ps(mask3.v, invPos), _mm_andnot_ps(mask3.v, negate3));
    vmtrans r;
    r.posScale = _mm_mul_ps(p, scale);
    r.quat = invQuat;
    return r;
}

__forceinline vmtrans combineT(vmtransParam t1, vmtransParam t2)
{
    static const vmmask mask3 = {0xffffffff, 0xffffffff, 0xffffffff, 0};

    __m128 p = _mm_mul_ps(t2.posScale, splatW(t1.posScale));
    __m128 q = transform3(p, t1.quat);
    __m128 np = _mm_add_ps(q, t1.posScale);
    vmtrans r;
    r.posScale = _mm_or_ps(_mm_and_ps(mask3.v, np), _mm_andnot_ps(mask3.v, p));
    r.quat = mulQ(t2.quat, t1.quat);
    return r;
}

__forceinline vmtrans normalizeT(vmtransParam t)
{
    vmtrans r;
    r.posScale = t.posScale;
    r.quat = normalizeQ(t.quat);
    return r;
}

} // end of vm
} // end of mathos

#endif
