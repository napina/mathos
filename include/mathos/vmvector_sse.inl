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
#ifndef mathos_vmvector_sse_inl
#define mathos_vmvector_sse_inl

namespace mathos {
namespace vm {

namespace internal {

template<int Count> struct Shift {};

template<> struct Shift<1>
{
    __forceinline static vmvec Left(vmvecFastParam v1, vmvecFastParam v2)
    {
        const __m128 a = _mm_move_ss(v1, v2);
        return _mm_shuffle_ps(a, a, _MM_SHUFFLE(0,3,2,1));
    }
};

template<> struct Shift<2>
{
    __forceinline static vmvec Left(vmvecFastParam v1, vmvecFastParam v2)
    {
        return _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(1,0,3,2));
    }
};

template<> struct Shift<3>
{
    __forceinline static vmvec Left(vmvecFastParam v1, vmvecFastParam v2)
    {
        const __m128 a = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(0,0,0,3));
        const __m128 b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,2,0));
        return _mm_shuffle_ps(b, v2, _MM_SHUFFLE(0,1,1,2));
    }
};
} // end of internal

#define vm_insert(a, b, selectX, selectY, selectZ, selectW)\
    _mm_shuffle_ps(\
        _mm_shuffle_ps(\
            _mm_shuffle_ps(a, b, _MM_SHUFFLE(1,0,1,0)),\
            _mm_shuffle_ps(a, b, _MM_SHUFFLE(1,0,1,0)),\
            _MM_SHUFFLE(0, 0, ((selectY) ? 3 : 1), ((selectX) ? 2 : 0))),\
        _mm_shuffle_ps(\
            _mm_shuffle_ps(a, b, _MM_SHUFFLE(3,2,3,2)),\
            _mm_shuffle_ps(a, b, _MM_SHUFFLE(3,2,3,2)),\
            _MM_SHUFFLE(((selectW) ? 3 : 1), ((selectZ) ? 2 : 0), 0, 0)),\
        _MM_SHUFFLE(3,2,1,0))

#define vm_shiftLeft(a, b, elements)\
    mathos::vm::internal::Shift<elements>::Left(a, b)

#define vm_rotateLeft(v, elements)\
     _mm_shuffle_ps(v, v, _MM_SHUFFLE((elements+3&3),((elements+2)&3),((elements+1)&3),((elements+0)&3)))

#define vm_rotateRight(v, elements)\
    vm_rotateLeft(v, 4 - elements)

#define vm_swizzle(v, e0, e1, e2, e3)\
    _mm_shuffle_ps(v, v, _MM_SHUFFLE(e3, e2, e1, e0))
//-----------------------------------------------------------------------------

__forceinline vmvec make(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

__forceinline vmvec makeInt(uint32 x, uint32 y, uint32 z, uint32 w)
{
    __m128i V = _mm_set_epi32(w, z, y, x);
    return reinterpret_cast<__m128 *>(&V)[0];
}

__forceinline vmvec splatZero()
{
    return _mm_setzero_ps();
}

__forceinline vmvec splatOne()
{
    static const vmvec r = {1.0f, 1.0f, 1.0f, 1.0f};
    return r;
}

__forceinline vmvec splatSignMask()
{
    static const VMMASK mask = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
    return mask.v;
//    __m128i v = _mm_set1_epi32(0x80000000);
//    return reinterpret_cast<__m128*>(&v)[0];
}

__forceinline vmvec splatMask(uint32 mask)
{
    __m128i v = _mm_set1_epi32(mask);
    return reinterpret_cast<__m128*>(&v)[0];
}

__forceinline vmvec splatX(vmvecFastParam v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
}

__forceinline vmvec splatY(vmvecFastParam v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
}

__forceinline vmvec splatZ(vmvecFastParam v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,2,2,2));
}

__forceinline vmvec splatW(vmvecFastParam v)
{
    return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3));
}

__forceinline vmvec replicate(float f)
{
    return _mm_set_ps1(f);
}

__forceinline vmvec select(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam control)
{
    const __m128 tmp1 = _mm_andnot_ps(control, v1);
    const __m128 tmp2 = _mm_and_ps(v2, control);
    return _mm_or_ps(tmp1, tmp2);
}

__forceinline vmvec permute(vmvecFastParam v1, vmvecFastParam v2, uint32 e0, uint32 e1, uint32 e2, uint32 e3)
{
    // TODO optimize
    vmvec a[2];
    a[0] = v1;
    a[1] = v2;
    float* f = (float*)(&a[0]);
    return make(f[e0 & 7], f[e1 & 7], f[e2 & 7], f[e3 & 7]);
};

__forceinline vmvec permute(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam control)
{
    // TODO optimize
    vmvec a[2];
    a[0] = v1;
    a[1] = v2;
    float* f = (float*)(&a[0]);
    return make(f[control.m128_u32[0] & 7], f[control.m128_u32[1] & 7], f[control.m128_u32[2] & 7], f[control.m128_u32[3] & 7]);
}

__forceinline vmvec permuteXYZA(vmvecFastParam v1, vmvecFastParam v2)
{
    return vm_shiftLeft(vm_rotateRight(v1, 1), v2, 1);
}

__forceinline vmvec mergeXY(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_unpacklo_ps(v1, v2);
}

__forceinline vmvec mergeZW(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_unpackhi_ps(v1, v2);
}
//-----------------------------------------------------------------------------

__forceinline vmvec abs4(vmvecFastParam v)
{
    __m128 r = _mm_setzero_ps();
    r = _mm_sub_ps(r, v);
    return _mm_max_ps(r, v);
}

__forceinline vmvec neg4(vmvecFastParam v)
{
    const __m128 zero = _mm_setzero_ps();
    return _mm_sub_ps(zero, v);
}

__forceinline vmvec min4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_min_ps(v1, v2);
}

__forceinline vmvec max4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_max_ps(v1, v2);
}

__forceinline vmvec fract4(vmvecFastParam v)
{
    return sub4(v, truncate4(v));
}

__forceinline vmvec round4(vmvecFastParam v)
{
    static const VMMASK signMask = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
    static const __m128 half = {0.5f, 0.5f, 0.5f, 0.5f};
    
    const __m128 s = _mm_and_ps(v, signMask.v);
    const __m128 a = _mm_or_ps(half, s);
    const __m128 b = _mm_add_ps(v, a);
    return _mm_cvtepi32_ps(_mm_cvttps_epi32(b));
}

__forceinline vmvec floor4(vmvecFastParam v)
{
    static const VMMASK oneHalfMinusEpsilon = {0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD};

    const __m128 r = _mm_sub_ps(v, oneHalfMinusEpsilon.v);
    const __m128i i = _mm_cvtps_epi32(r);
    return _mm_cvtepi32_ps(i);
}

__forceinline vmvec ceil4(vmvecFastParam v)
{
    static const VMMASK oneHalfMinusEpsilon = {0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD};

    const __m128 r = _mm_add_ps(v, oneHalfMinusEpsilon.v);
    const __m128i i = _mm_cvtps_epi32(r);
    return _mm_cvtepi32_ps(i);
}

__forceinline vmvec truncate4(vmvecFastParam v)
{
    return _mm_cvtepi32_ps(_mm_cvttps_epi32(v));
}

__forceinline vmvec clamp4(vmvecFastParam v, vmvecFastParam min, vmvecFastParam max)
{
    return _mm_min_ps(_mm_max_ps(min, v), max);
}

__forceinline vmvec saturate4(vmvecFastParam v)
{
#if 1
    const __m128 zerof = _mm_sub_ps(v, v);
    return clamp4(v, zerof, splatOne());
#else
    return _mm_min_ps(_mm_max_ps(v, g_XMZero), g_XMOne);
#endif
}
//-----------------------------------------------------------------------------

__forceinline vmvec add4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_add_ps(v1, v2);
}

__forceinline vmvec sub4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_sub_ps(v1, v2);
}

__forceinline vmvec mul4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_mul_ps(v1, v2);
}

__forceinline vmvec mulAdd4(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3)
{
    return _mm_add_ps(_mm_mul_ps(v1, v2), v3);
}

__forceinline vmvec negMulSub4(vmvecFastParam v1, vmvecFastParam v2, vmvecFastParam v3)
{
    return _mm_sub_ps(v3, _mm_mul_ps(v1, v2));
}

__forceinline vmvec div4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_div_ps(v1, v2);
}

__forceinline vmvec divEst4(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_mul_ps(v1, _mm_rcp_ps(v2));
}

__forceinline vmvec sqrt4(vmvecFastParam v)
{
    static const __m128 half = {0.5f, 0.5f, 0.5f, 0.5f};

    // fast estimate with newton-raphson
    //	a = rsqrt(v)
    //	b = rcp(a)
    //	q = b - (b * b - v) * a * 0.5f
    const __m128 a = _mm_rsqrt_ps(v);
    const __m128 b = _mm_rcp_ps(a);
    const __m128 zerof = _mm_xor_ps(v, v);
    const __m128 mask = _mm_cmpeq_ps(v, zerof);
    const __m128 c = _mm_mul_ps(b, b);
    const __m128 d = _mm_mul_ps(a, half); 
    const __m128 e = _mm_sub_ps(c, v);
    const __m128 f = _mm_mul_ps(e, d);
    return select(_mm_sub_ps(b, f), zerof, mask);
}

__forceinline vmvec sqrtEst4(vmvecFastParam v)
{
    return _mm_mul_ps(v, _mm_rsqrt_ps(v));
}

__forceinline vmvec reciprocalSqrt4(vmvecFastParam v)
{
    static const __m128 half = {0.5f, 0.5f, 0.5f, 0.5f};
    static const __m128 three = {3.0f, 3.0f, 3.0f, 3.0f};

    // fast estimate with newton-raphson
    // r = rsqrt(v)
    // q = r * ((3.0f - r * r * x) * 0.5f);
    const __m128 r = _mm_rsqrt_ps(v);
    const __m128 a = _mm_sub_ps(three, _mm_mul_ps( _mm_mul_ps(v, r), r));
    const __m128 b = _mm_mul_ps(half, r);
    return _mm_mul_ps(a, b);
}

__forceinline vmvec reciprocalSqrtEst4(vmvecFastParam v)
{
    return _mm_rsqrt_ps(v);
}

__forceinline vmvec reciprocal4(vmvecFastParam v)
{
    static const __m128 one = {1.0f, 1.0f, 1.0f, 1.0f};

    // fast estimate with newton-raphson
    // r = rcp(v)
    // r = r + r * (1.0f - (r * v));
    const __m128 r = _mm_rcp_ps(v);
    const __m128 a = _mm_sub_ps(one, _mm_mul_ps(r, v));
    const __m128 b = _mm_mul_ps(a, r);
    return			 _mm_add_ps(b, r);
}

__forceinline vmvec reciprocalEst4(vmvecFastParam v)
{
    return _mm_rcp_ps(v);
}

#define _PS_CONST(Name, Val)\
  static const __m128 _ps_##Name = { Val, Val, Val, Val }
#define _PI32_CONST(Name, Val)\
  static const VMMASK _pi32_##Name = { Val, Val, Val, Val }

__forceinline vmvec sin4(vmvecFastParam v)
{
    // Original code made by Julien Pommier. http://gruntthepeon.free.fr/ssemath/
    _PI32_CONST(sign_mask, 0x80000000);
    _PI32_CONST(inv_sign_mask, ~0x80000000);
    _PS_CONST(minus_cephes_DP1, -0.78515625f);
    _PS_CONST(minus_cephes_DP2, -2.4187564849853515625e-4f);
    _PS_CONST(minus_cephes_DP3, -3.77489497744594108e-8f);
    _PS_CONST(sincof_p0, -1.9515295891E-4f);
    _PS_CONST(sincof_p1,  8.3321608736E-3f);
    _PS_CONST(sincof_p2, -1.6666654611E-1f);
    _PS_CONST(coscof_p0,  2.443315711809948E-005f);
    _PS_CONST(coscof_p1, -1.388731625493765E-003f);
    _PS_CONST(coscof_p2,  4.166664568298827E-002f);
    _PS_CONST(cephes_FOPI, 1.27323954473516f);
    _PS_CONST(1  , 1.0f);
    _PS_CONST(0p5, 0.5f);
    _PI32_CONST(1, 1u);
    _PI32_CONST(inv1, ~1u);
    _PI32_CONST(2, 2u);
    _PI32_CONST(4, 4u);

    __m128 xmm1, xmm2 = _mm_setzero_ps(), xmm3, sign_bit, y;

    __m128i emm0, emm2;
    __m128 x = v;
    sign_bit = x;
    /* take the absolute value */
    x = _mm_and_ps(x, _pi32_inv_sign_mask.v);
    /* extract the sign bit (upper one) */
    sign_bit = _mm_and_ps(sign_bit, _pi32_sign_mask.v);
  
    /* scale by 4/Pi */
    y = _mm_mul_ps(x, _ps_cephes_FOPI);

    /* store the integer part of y in mm0 */
    emm2 = _mm_cvttps_epi32(y);
    /* j=(j+1) & (~1) (see the cephes sources) */
    emm2 = _mm_add_epi32(emm2, _pi32_1.vi);
    emm2 = _mm_and_si128(emm2, _pi32_inv1.vi);
    y = _mm_cvtepi32_ps(emm2);
    /* get the swap sign flag */
    emm0 = _mm_and_si128(emm2, _pi32_4.vi);
    emm0 = _mm_slli_epi32(emm0, 29);
    /* get the polynom selection mask 
        there is one polynom for 0 <= x <= Pi/4
        and another one for Pi/4<x<=Pi/2

        Both branches will be computed.
    */
    emm2 = _mm_and_si128(emm2, _pi32_2.vi);
    emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
  
    __m128 swap_sign_bit = _mm_castsi128_ps(emm0);
    __m128 poly_mask = _mm_castsi128_ps(emm2);
    sign_bit = _mm_xor_ps(sign_bit, swap_sign_bit);
  
    /* The magic pass: "Extended precision modular arithmetic" 
        x = ((x - y * DP1) - y * DP2) - y * DP3; */
    xmm1 = _ps_minus_cephes_DP1;
    xmm2 = _ps_minus_cephes_DP2;
    xmm3 = _ps_minus_cephes_DP3;
    xmm1 = _mm_mul_ps(y, xmm1);
    xmm2 = _mm_mul_ps(y, xmm2);
    xmm3 = _mm_mul_ps(y, xmm3);
    x = _mm_add_ps(x, xmm1);
    x = _mm_add_ps(x, xmm2);
    x = _mm_add_ps(x, xmm3);

    /* Evaluate the first polynom  (0 <= x <= Pi/4) */
    y = _ps_coscof_p0;
    __m128 z = _mm_mul_ps(x,x);

    y = _mm_mul_ps(y, z);
    y = _mm_add_ps(y, _ps_coscof_p1);
    y = _mm_mul_ps(y, z);
    y = _mm_add_ps(y, _ps_coscof_p2);
    y = _mm_mul_ps(y, z);
    y = _mm_mul_ps(y, z);
    __m128 tmp = _mm_mul_ps(z, _ps_0p5);
    y = _mm_sub_ps(y, tmp);
    y = _mm_add_ps(y, _ps_1);
  
    // Evaluate the second polynom  (Pi/4 <= x <= 0)
    __m128 y2 = _ps_sincof_p0;
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_add_ps(y2, _ps_sincof_p1);
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_add_ps(y2, _ps_sincof_p2);
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_mul_ps(y2, x);
    y2 = _mm_add_ps(y2, x);

    // select the correct result from the two polynoms
    xmm3 = poly_mask;
    y2 = _mm_and_ps(xmm3, y2); //, xmm3);
    y = _mm_andnot_ps(xmm3, y);
    y = _mm_add_ps(y,y2);
    // update the sign
    y = _mm_xor_ps(y, sign_bit);
    return y;
}

__forceinline vmvec cos4(vmvecFastParam v)
{
    // Original code made by Julien Pommier. http://gruntthepeon.free.fr/ssemath/
    _PI32_CONST(inv_sign_mask, ~0x80000000);
    _PS_CONST(minus_cephes_DP1, -0.78515625f);
    _PS_CONST(minus_cephes_DP2, -2.4187564849853515625e-4f);
    _PS_CONST(minus_cephes_DP3, -3.77489497744594108e-8f);
    _PS_CONST(sincof_p0, -1.9515295891E-4f);
    _PS_CONST(sincof_p1,  8.3321608736E-3f);
    _PS_CONST(sincof_p2, -1.6666654611E-1f);
    _PS_CONST(coscof_p0,  2.443315711809948E-005f);
    _PS_CONST(coscof_p1, -1.388731625493765E-003f);
    _PS_CONST(coscof_p2,  4.166664568298827E-002f);
    _PS_CONST(cephes_FOPI, 1.27323954473516f);
    _PS_CONST(1  , 1.0f);
    _PS_CONST(0p5, 0.5f);
    _PI32_CONST(1, 1u);
    _PI32_CONST(inv1, ~1u);
    _PI32_CONST(2, 2u);
    _PI32_CONST(4, 4u);

    __m128 xmm1, xmm2 = _mm_setzero_ps(), xmm3, y;
    __m128i emm0, emm2;

    /* take the absolute value */
    __m128 x = _mm_and_ps(v, _pi32_inv_sign_mask.v);
  
    /* scale by 4/Pi */
    y = _mm_mul_ps(x, _ps_cephes_FOPI);
  
    /* store the integer part of y in mm0 */
    emm2 = _mm_cvttps_epi32(y);
    /* j=(j+1) & (~1) (see the cephes sources) */
    emm2 = _mm_add_epi32(emm2, _pi32_1.vi);
    emm2 = _mm_and_si128(emm2, _pi32_inv1.vi);
    y = _mm_cvtepi32_ps(emm2);

    emm2 = _mm_sub_epi32(emm2, _pi32_2.vi);
  
    /* get the swap sign flag */
    emm0 = _mm_andnot_si128(emm2, _pi32_4.vi);
    emm0 = _mm_slli_epi32(emm0, 29);
    /* get the polynom selection mask */
    emm2 = _mm_and_si128(emm2, _pi32_2.vi);
    emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
  
    __m128 sign_bit = _mm_castsi128_ps(emm0);
    __m128 poly_mask = _mm_castsi128_ps(emm2);

    /* The magic pass: "Extended precision modular arithmetic" 
        x = ((x - y * DP1) - y * DP2) - y * DP3; */
    xmm1 = _ps_minus_cephes_DP1;
    xmm2 = _ps_minus_cephes_DP2;
    xmm3 = _ps_minus_cephes_DP3;
    xmm1 = _mm_mul_ps(y, xmm1);
    xmm2 = _mm_mul_ps(y, xmm2);
    xmm3 = _mm_mul_ps(y, xmm3);
    x = _mm_add_ps(x, xmm1);
    x = _mm_add_ps(x, xmm2);
    x = _mm_add_ps(x, xmm3);
  
    // Evaluate the first polynom  (0 <= x <= Pi/4)
    y = _ps_coscof_p0;
    __m128 z = _mm_mul_ps(x,x);

    y = _mm_mul_ps(y, z);
    y = _mm_add_ps(y, _ps_coscof_p1);
    y = _mm_mul_ps(y, z);
    y = _mm_add_ps(y, _ps_coscof_p2);
    y = _mm_mul_ps(y, z);
    y = _mm_mul_ps(y, z);
    __m128 tmp = _mm_mul_ps(z, _ps_0p5);
    y = _mm_sub_ps(y, tmp);
    y = _mm_add_ps(y, _ps_1);
  
    // Evaluate the second polynom  (Pi/4 <= x <= 0)
    __m128 y2 = _ps_sincof_p0;
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_add_ps(y2, _ps_sincof_p1);
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_add_ps(y2, _ps_sincof_p2);
    y2 = _mm_mul_ps(y2, z);
    y2 = _mm_mul_ps(y2, x);
    y2 = _mm_add_ps(y2, x);

    // select the correct result from the two polynoms
    xmm3 = poly_mask;
    y2 = _mm_and_ps(xmm3, y2); //, xmm3);
    y = _mm_andnot_ps(xmm3, y);
    y = _mm_add_ps(y,y2);
    // update the sign
    y = _mm_xor_ps(y, sign_bit);
    return y;
}

#undef _PS_CONST
#undef _PI32_CONST
#undef _PS_CONST_TYPE

__forceinline vmvec sinZeroHalfPI4(vmvecFastParam a)
{
    static const __m128 sin_c0 = {-2.39e-08f, -2.39e-08f, -2.39e-08f, -2.39e-08f};
    static const __m128 sin_c1 = {2.7526e-06f, 2.7526e-06f, 2.7526e-06f, 2.7526e-06f};
    static const __m128 sin_c2 = {-1.98409e-04f, -1.98409e-04f, -1.98409e-04f, -1.98409e-04f};
    static const __m128 sin_c3 = {8.3333315e-03f, 8.3333315e-03f, 8.3333315e-03f, 8.3333315e-03f};
    static const __m128 sin_c4 = {-1.666666664e-01f, -1.666666664e-01f, -1.666666664e-01f, -1.666666664e-01f};
    static const __m128 one    = {1.0f, 1.0f, 1.0f, 1.0f};

    __m128 s, t;
    s = _mm_mul_ps(a, a);
    t = _mm_add_ps(_mm_mul_ps(sin_c0, s), sin_c1);
    t = _mm_add_ps(_mm_mul_ps(t, s), sin_c2);
    t = _mm_add_ps(_mm_mul_ps(t, s), sin_c3);
    t = _mm_add_ps(_mm_mul_ps(t, s), sin_c4);
    t = _mm_add_ps(_mm_mul_ps(t, s), one);
    return _mm_mul_ps(t, a);
}

__forceinline vmvec atanPositive4(vmvecFastParam y, vmvecFastParam x)
{
    static const __m128 atan_c0  = { 0.0028662257f,  0.0028662257f,  0.0028662257f,  0.0028662257f};
    static const __m128 atan_c1  = {-0.0161657367f, -0.0161657367f, -0.0161657367f, -0.0161657367f};
    static const __m128 atan_c2  = { 0.0429096138f,  0.0429096138f,  0.0429096138f,  0.0429096138f};
    static const __m128 atan_c3  = {-0.0752896400f, -0.0752896400f, -0.0752896400f, -0.0752896400f};
    static const __m128 atan_c4  = { 0.1065626393f,  0.1065626393f,  0.1065626393f,  0.1065626393f};
    static const __m128 atan_c5  = {-0.1420889944f, -0.1420889944f, -0.1420889944f, -0.1420889944f};
    static const __m128 atan_c6  = { 0.1999355085f,  0.1999355085f,  0.1999355085f,  0.1999355085f};
    static const __m128 atan_c7  = {-0.3333314528f, -0.3333314528f, -0.3333314528f, -0.3333314528f};
    static const __m128 halfpi   = {1.57079633f, 1.57079633f, 1.57079633f, 1.57079633f};
    static const __m128 one      = {1.0f, 1.0f, 1.0f, 1.0f};
    static const VMMASK signMask = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

    const __m128 minv = _mm_min_ps(x, y);
    const __m128 maxv = _mm_max_ps(x, y);
    const __m128 cmp = _mm_cmpeq_ps(y, maxv);
    // reciprocal with newton-raphson
    // r = rcp(maxv)
    // r = minv * ((r + r) - ((maxv * r) * r))
    const __m128 r = _mm_rcp_ps(maxv);
    const __m128 ra = _mm_mul_ps(_mm_mul_ps(maxv, r), r);
    const __m128 rb = _mm_sub_ps(_mm_add_ps(r, r), ra);
    const __m128 rc = _mm_mul_ps(minv, rb);
    const __m128 a = _mm_xor_ps(rc, _mm_and_ps(cmp, signMask.v)); // (y>x) ? (-x/y) : (y/x)
    const __m128 d = _mm_and_ps(cmp, halfpi);                     // (y>x) ? (pi/2) :   0

    __m128 s, t;
    s = _mm_mul_ps(a, a);
    t = _mm_add_ps(_mm_mul_ps(atan_c0, s), atan_c1);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c2);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c3);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c4);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c5);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c6);
    t = _mm_add_ps(_mm_mul_ps(t, s), atan_c7);
    t = _mm_add_ps(_mm_mul_ps(t, s), one);
    return _mm_add_ps(_mm_mul_ps(t, a), d);
}
//-----------------------------------------------------------------------------

__forceinline vmvec isNaN(vmvecFastParam v)
{
    const __m128i infinity = _mm_set1_epi32(0x7F800000);
    const __m128i nanTest  = _mm_set1_epi32(0x007FFFFF);

    __m128i vTempInf = _mm_and_si128(reinterpret_cast<const __m128i*>(&v)[0], infinity);
    __m128i vTempNan = _mm_and_si128(reinterpret_cast<const __m128i*>(&v)[0], nanTest);
    vTempInf = _mm_cmpeq_epi32(vTempInf, infinity);
    vTempNan = _mm_cmpeq_epi32(vTempNan, _mm_xor_si128(vTempNan,vTempNan));
    vTempNan = _mm_andnot_si128(vTempNan, vTempInf);
    return reinterpret_cast<const __m128*>(&vTempNan)[0];
}

__forceinline vmvec isEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmpeq_ps(v1, v2);
}

__forceinline vmvec isNotEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmpneq_ps(v1, v2);
}

__forceinline vmvec isGreater(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmpgt_ps(v1, v2);
}

__forceinline vmvec isGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmpge_ps(v1, v2);
}

__forceinline vmvec isLess(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmplt_ps(v1, v2);
}

__forceinline vmvec isLessOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_cmple_ps(v1, v2);
}

__forceinline vmvec isInBounds(vmvecFastParam v, vmvecFastParam bounds)
{
    const __m128 n = neg4(bounds);
    const __m128 a = _mm_cmple_ps(v, bounds);
    const __m128 b = _mm_cmple_ps(n, v);
    return _mm_and_ps(a, b);
}

__forceinline uint32 allEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) == 0xf;
}

__forceinline uint32 allLess(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmplt_ps(v1, v2)) == 0xf;
}

__forceinline uint32 allLessOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmple_ps(v1, v2)) == 0xf;
}

__forceinline uint32 allGreater(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpgt_ps(v1, v2)) == 0xf;
}

__forceinline uint32 allGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpge_ps(v1, v2)) == 0xf;
}

__forceinline uint32 anyEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) != 0;
}

__forceinline uint32 anyLess(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmplt_ps(v1, v2)) != 0;
}

__forceinline uint32 anyLessOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmple_ps(v1, v2)) != 0;
}

__forceinline uint32 anyGreater(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpgt_ps(v1, v2)) != 0;
}

__forceinline uint32 anyGreaterOrEqual(vmvecFastParam v1, vmvecFastParam v2)
{
    return _mm_movemask_ps(_mm_cmpge_ps(v1, v2)) != 0;
}

__forceinline uint32 allSame(vmvecFastParam v)
{
    return _mm_movemask_ps(_mm_cmpeq_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0)))) == 0xf;
}
//-----------------------------------------------------------------------------

__forceinline vmvec randomSeed()
{
    static const VMMASK defaultSeed = {
        0x10D63AF1, // 16087^2
        0x60B7A437, // 16087^3
        0xB87E16E1, // 16087^4
        0x5E100DC7  // 16087^5
    };
    return defaultSeed.v;
}

__forceinline vmvec random01(vmvec& seed)
{
    // (((seed * 16807) & 0x007fffff) | 0x3f800000) - 1.0f
    const __m128i magic = _mm_set1_epi32(16807);
    const __m128i andMask = _mm_set1_epi32(0x007fffff);
    const __m128i orMask = _mm_set1_epi32(0x3f800000);
    __m128i s = reinterpret_cast<__m128i*>(&seed)[0];
    s = _mm_mul_epu32(s, magic);
    s = _mm_or_si128(_mm_and_si128(s, andMask), orMask);
    seed = reinterpret_cast<__m128*>(&s)[0];
    return _mm_sub_ps(seed, splatOne());
}

__forceinline vmvec randomSigned(vmvec& seed)
{
    // (((seed * 16807) & 0x007fffff) | 0x40000000) - 3.0f
    const __m128i magic = _mm_set1_epi32(16807);
    const __m128i andMask = _mm_set1_epi32(0x007fffff);
    const __m128i orMask = _mm_set1_epi32(0x40000000);
    __m128i s = reinterpret_cast<__m128i*>(&seed)[0];
    s = _mm_mul_epu32(s, magic);
    s = _mm_or_si128(_mm_and_si128(s, andMask), orMask);
    seed = reinterpret_cast<__m128*>(&s)[0];
    return _mm_sub_ps(seed, replicate(3.0f));
}

__forceinline vmvec randomNormal(vmvec& seed)
{
    return normalize3(randomSigned(seed));
}

__forceinline vmvec randomQuat(vmvec& seed)
{
    // use precomputed angle
    static const __m128 sin = {0.841470985f, 0.841470985f, 0.841470985f, 0.0f};
    static const __m128 cos = {0.0f, 0.0f, 0.0f, 0.540302306f};

    vmvec axis = randomNormal(seed);
    __m128 r = _mm_mul_ps(axis, sin);
    return _mm_add_ps(r, cos);
}
//-----------------------------------------------------------------------------

} // end of vm
} // end of matho

#endif
