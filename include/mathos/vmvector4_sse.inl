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
#ifndef mathos_vmvector4_sse_inl
#define mathos_vmvector4_sse_inl

namespace mathos {
namespace vm {

__forceinline vmvec dot4(vmvecFastParam v1, vmvecFastParam v2)
{
    const __m128 xyzw = _mm_mul_ps(v1, v2);
    const __m128 wzyx = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(0,1,2,3));
    const __m128 a    = _mm_add_ps(xyzw, wzyx);
    const __m128 b    = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,3,0,1));
    return _mm_add_ps(a, b);
}

__forceinline vmvec length4(vmvecFastParam v)
{
    return sqrt4(dot4(v, v));
}

__forceinline vmvec lengthEst4(vmvecFastParam v)
{
    return _mm_sqrt_ps(dot4(v, v));
}

__forceinline vmvec normalize4(vmvecFastParam v)
{
    const __m128 lenSq = dot4(v, v);
    const __m128 r = _mm_rsqrt_ss(lenSq);
    const __m128 half = { 0.5f, 0.5f, 0.5f, 0.5f };
    const __m128 three = { 3.0f, 3.0f, 3.0f, 3.0f };
    const __m128 a = _mm_sub_ss(three, _mm_mul_ss(_mm_mul_ss(lenSq, r), r));
    const __m128 b = _mm_mul_ss(half, r);
    const __m128 rq = _mm_mul_ss(a, b);
    return _mm_mul_ps(v, splatX(rq));
}

__forceinline vmvec normalizeEst4(vmvecFastParam v)
{
    return _mm_mul_ps(v, _mm_rsqrt_ps(dot4(v, v)));
}

__forceinline vmvec transform4(vmvecFastParam v, vmvecFastParam q)
{
    return transform3(v, q);
}

__forceinline vmvec transform4(vmvecFastParam v, vmmatParam m)
{
    const __m128 x  = _mm_mul_ps(m.row0, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0)));
    const __m128 y  = _mm_mul_ps(m.row1, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1)));
    const __m128 xy = _mm_add_ps(x, y);
    const __m128 z  = _mm_mul_ps(m.row2, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,2,2,2)));
    const __m128 w  = _mm_mul_ps(m.row3, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3)));
    const __m128 zw = _mm_add_ps(z, w);
    return _mm_add_ps(xy, zw);
}

__forceinline vmvec transform4(vmvecFastParam v, vmtransParam t)
{
    return transform3(v, t);
}

__forceinline vmvec lerp4(vmvecFastParam a, vmvecFastParam b, vmvecFastParam t)
{
    // TODO
    // b*t + a*(1-t)
    // a + (b-a)*t
    return negMulSub4(a, t, mul4(b, t));
}

} // end of vm
} // end of mathos

#endif
