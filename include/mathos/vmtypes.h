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
#ifndef mathos_vmtypes_h
#define mathos_vmtypes_h

#include "mathos\math.h"

#if defined(MATHOS_WINDOWS) || defined(MATHOS_MACOSX) || defined(MATHOS_LINUX)
    #define VMATH_SSE
#elif defined(MATHOS_IOS)
    #define VMATH_ARM_NEON
#endif

#if defined(VMATH_SSE)
#   include <immintrin.h>
#elif defined(VMATH_ARM_NEON)
#   include <arm_neon.h>
#endif

namespace mathos {
namespace vm {

#if defined(VMATH_SSE)
    typedef __m128 vmvec;
    typedef vmvec const& vmvecFastParam;
    typedef vmvec const& vmvecParam;

    mathos_prealign16 struct vmmask {
        union {
            uint32_t i[4];
            __m128 v;
            __m128i vi;
        };
    } mathos_postalign16;

    #define VM_PERMUTE_0X       0x00000000
    #define VM_PERMUTE_0Y       0x00000001
    #define VM_PERMUTE_0Z       0x00000002
    #define VM_PERMUTE_0W       0x00000003
    #define VM_PERMUTE_1X       0xfffffff4
    #define VM_PERMUTE_1Y       0xfffffff5
    #define VM_PERMUTE_1Z       0xfffffff6
    #define VM_PERMUTE_1W       0xfffffff7
#elif defined(VMATH_ARM_NEON)
    typedef float32x4_t vmvec;
    typedef vmvec const vmvecFastParam;
    typedef vmvec const& vmvecParam;

    #define VM_PERMUTE_0X       0x00010203
    #define VM_PERMUTE_0Y       0x04050607
    #define VM_PERMUTE_0Z       0x08090a0b
    #define VM_PERMUTE_0W       0x0c0d0e0f
    #define VM_PERMUTE_1X       0x10111213
    #define VM_PERMUTE_1Y       0x14151617
    #define VM_PERMUTE_1Z       0x18191a1b
    #define VM_PERMUTE_1W       0x1c1d1e1f
#else // && defined(VMATH_ARM_VFP)
    typedef mathos::vec4a vmvec;
    typedef vmvec const& vmvecFastParam;
    typedef vmvec const& vmvecParam;

    #define VM_PERMUTE_0X       0x00000000
    #define VM_PERMUTE_0Y       0x00000001
    #define VM_PERMUTE_0Z       0x00000002
    #define VM_PERMUTE_0W       0x00000003
    #define VM_PERMUTE_1X       0xfffffff4
    #define VM_PERMUTE_1Y       0xfffffff5
    #define VM_PERMUTE_1Z       0xfffffff6
    #define VM_PERMUTE_1W       0xfffffff7
#endif

#define VM_VECTOR_FALSE         0x00000000
#define VM_VECTOR_TRUE          0xffffffff
#define VM_FALSE                0
#define VM_TRUE                 1

typedef struct
{
    vmvec row0;
    vmvec row1;
    vmvec row2;
    vmvec row3;
} vmmat;
typedef vmmat const& vmmatParam;

typedef struct
{
    vmvec row0;
    vmvec row1;
    vmvec row2;
} vmmat34;
typedef vmmat34 const& vmmat43Param;

typedef struct
{
    vmvec posScale;
    vmvec quat;
} vmtrans;
typedef vmtrans const& vmtransParam;

} // end of vm
} // end of mathos

#endif
