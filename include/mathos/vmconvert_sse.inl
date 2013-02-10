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
#ifndef mathos_vmconvert_sse_inl
#define mathos_vmconvert_sse_inl

namespace mathos {
namespace vm {

/*__forceinline vmvec vmLoadInt(void const* source)
{
    return _mm_load_ss((const float*)source);
}

__forceinline vmvec vmLoadFloat(void const* source)
{
    return _mm_load_ss((const float*)source);
}*/

__forceinline vmvec load2(void const* source)
{
    float const* element = (float const*)source;
    __m128 x = _mm_load_ss(&element[0]);
    __m128 y = _mm_load_ss(&element[1]);
    return _mm_unpacklo_ps(x, y);
}

__forceinline vmvec load2A(void const* source)
{
    __m128i V = _mm_loadl_epi64((const __m128i*)source);
    return reinterpret_cast<__m128 *>(&V)[0];
}

__forceinline vmvec load3(void const* source)
{
    float const* element = (float const*)source;
    __m128 x = _mm_load_ss(&element[0]);
    __m128 y = _mm_load_ss(&element[1]);
    __m128 z = _mm_load_ss(&element[2]);
#if 1 // TODO which is better??
    __m128 xy = _mm_unpacklo_ps(x, y);
    return _mm_movelh_ps(xy, z);
#else
    return _mm_shuffle_ps(_mm_unpacklo_ps(a,b), c, VM_SHUFFLE(0,1,0,1));
#endif
}

__forceinline vmvec load3A(void const* source)
{
    // Read 4 floats and ignore last
    return _mm_load_ps((float const*)source);
}

__forceinline vmvec load4(void const* source)
{
    return _mm_loadu_ps((float const*)source);
}

__forceinline vmvec load4A(void const* source)
{
    return _mm_load_ps((float const*)source);
}

__forceinline vmmat loadM(void const* source)
{
    float const* element = (float const*)source;
    vmmat m;
    m.row0 = _mm_loadu_ps(&element[0]);
    m.row1 = _mm_loadu_ps(&element[4]);
    m.row2 = _mm_loadu_ps(&element[8]);
    m.row3 = _mm_loadu_ps(&element[12]);
    return m;
}

__forceinline vmmat loadMA(void const* source)
{
    float const* element = (float const*)source;
    vmmat m;
    m.row0 = _mm_load_ps(&element[0]);
    m.row1 = _mm_load_ps(&element[4]);
    m.row2 = _mm_load_ps(&element[8]);
    m.row3 = _mm_load_ps(&element[12]);
    return m;
}

__forceinline vmtrans loadT(void const* source)
{
    float const* element = (float const*)source;
    vmtrans t;
    t.posScale = _mm_loadu_ps(&element[0]);
    t.quat = _mm_loadu_ps(&element[4]);
    return t;
}

__forceinline vmtrans loadTA(void const* source)
{
    float const* element = (float const*)source;
    vmtrans t;
    t.posScale = _mm_load_ps(&element[0]);
    t.quat = _mm_load_ps(&element[4]);
    return t;
}
//-----------------------------------------------------------------------------

/*__forceinline void storeInt(void* destination, vmvecFastParam v)
{
    _mm_store_ss((float*)destination, v);
}

__forceinline void storeFloat(void* destination, vmvecFastParam v)
{
    _mm_store_ss((float*)destination, v);
}*/

__forceinline void store2(void* destination, vmvecFastParam v)
{
    float* dest = (float*)destination;
    vmvec t = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
    _mm_store_ss( &dest[0], v );
    _mm_store_ss( &dest[1], t );
}

__forceinline void store2A(void* destination, vmvecFastParam v)
{
    _mm_storel_epi64( (__m128i*)destination, reinterpret_cast<const __m128i *>(&v)[0] );
}

__forceinline void store3(void* destination, vmvecFastParam v)
{
    float* dest = (float*)destination;
    vmvec t1 = _mm_shuffle_ps(v,v,_MM_SHUFFLE(1,1,1,1));
    vmvec t2 = _mm_shuffle_ps(v,v,_MM_SHUFFLE(2,2,2,2));
    _mm_store_ss( &dest[0], v );
    _mm_store_ss( &dest[1], t1 );
    _mm_store_ss( &dest[2], t2 );
}

__forceinline void store3A(void* destination, vmvecFastParam v)
{
    float* dest = (float*)destination;
    vmvec t = _mm_shuffle_ps(v,v,_MM_SHUFFLE(2,2,2,2));
    _mm_storel_epi64( (__m128i*)destination, reinterpret_cast<const __m128i *>(&v)[0] );
    _mm_store_ss( &dest[2], t );
}

__forceinline void store4(void* destination, vmvecFastParam v)
{
    _mm_storeu_ps((float*)destination, v);
}

__forceinline void store4A(void* destination, vmvecFastParam v)
{
    _mm_store_ps((float*)destination, v);
}

__forceinline void storeM4(void* destination, vmmatParam m)
{
    float* dest = (float*)destination;
    _mm_storeu_ps(&dest[0], m.row0);
    _mm_storeu_ps(&dest[4], m.row1);
    _mm_storeu_ps(&dest[8], m.row2);
    _mm_storeu_ps(&dest[12], m.row3);
}

__forceinline void storeMA(void* destination, vmmatParam m)
{
    float* dest = (float*)destination;
    _mm_store_ps(&dest[0], m.row0);
    _mm_store_ps(&dest[4], m.row1);
    _mm_store_ps(&dest[8], m.row2);
    _mm_store_ps(&dest[12], m.row3);
}

__forceinline void storeT(void* destination, vmtransParam t)
{
    float* dest = (float*)destination;
    _mm_storeu_ps(&dest[0], t.posScale);
    _mm_storeu_ps(&dest[4], t.quat);
}

__forceinline void storeTA(void* destination, vmtransParam t)
{
    float* dest = (float*)destination;
    _mm_store_ps(&dest[0], t.posScale);
    _mm_store_ps(&dest[4], t.quat);
}
//-----------------------------------------------------------------------------

__forceinline float extractX(vmvecFastParam v)
{
    return _mm_cvtss_f32(v);
}

__forceinline float extractY(vmvecFastParam v)
{
    const __m128 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
    return _mm_cvtss_f32(tmp);
}

__forceinline float extractZ(vmvecFastParam v)
{
    const __m128 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,2,2,2));
    return _mm_cvtss_f32(tmp);
}

__forceinline float extractW(vmvecFastParam v)
{
    const __m128 tmp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3));
    return _mm_cvtss_f32(tmp);
}

__forceinline uint32 extractXInt(vmvecFastParam v)
{
    return static_cast<uint32>(_mm_cvtsi128_si32(reinterpret_cast<const __m128i*>(&v)[0]));
}

__forceinline uint32 extractYInt(vmvecFastParam v)
{
    __m128i tmp = _mm_shuffle_epi32(reinterpret_cast<const __m128i*>(&v)[0], _MM_SHUFFLE(1,1,1,1));
    return static_cast<uint32>(_mm_cvtsi128_si32(tmp));
}

__forceinline uint32 extractZInt(vmvecFastParam v)
{
    __m128i tmp = _mm_shuffle_epi32(reinterpret_cast<const __m128i*>(&v)[0], _MM_SHUFFLE(2,2,2,2));
    return static_cast<uint32>(_mm_cvtsi128_si32(tmp));
}

__forceinline uint32 extractWInt(vmvecFastParam v)
{
    __m128i tmp = _mm_shuffle_epi32(reinterpret_cast<const __m128i*>(&v)[0], _MM_SHUFFLE(3,3,3,3));
    return static_cast<uint32>(_mm_cvtsi128_si32(tmp));
}

} // end of vm
} // end of mathos

#endif
