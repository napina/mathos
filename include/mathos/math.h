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
#ifndef mathos_math_h
#define mathos_math_h

#if defined(__linux__) && defined(__ELF__)
#   define MATHOS_LINUX
#   define MATHOS_ARCH32
#elif defined(__APPLE__) && defined(__MACH__)
#   if defined(_ARM)
#       define MATHOS_IOS
#       define MATHOS_ARCH32
#   else
#       define MATHOS_MACOSX
#       define MATHOS_ARCH32
#   endif
#elif defined(_WIN64) || defined(_M_X64)
#   define MATHOS_WINDOWS
#   define MATHOS_ARCH64
#elif defined(_WIN32) || defined(_M_IX86)
#   define MATHOS_WINDOWS
#   define MATHOS_ARCH32
#endif

#define mathos_tostring_impl(x) #x
#define mathos_tostring(x)      mathos_tostring_impl(x)

#if defined(_MSC_VER)
#   define mathos_lineinfo      __FILE__ "(" mathos_tostring(__LINE__) ")"
#   define mathos_todo(msg)     __pragma(message(mathos_lineinfo " TODO " msg))
#   define mathos_prealign16    __declspec(align(16))
#   define mathos_postalign16
#elif defined(__GNUC__)
#   define mathos_lineinfo      __FILE__ ":" mathos_tostring(__LINE__)
#   define mathos_todo(msg)     __Pragma(message("TODO " msg))
#   define mathos_prealign16    __attribute__((aligned(16)))
#   define mathos_postalign16
#   define __forceinline        inline __attribute__((always_inline))
#   define __restrict           __restrict__
#else
#   define mathos_lineinfo      __FILE__ ":" mathos_tostring(__LINE__)
#   define mathos_todo(msg)
#   define mathos_prealign16
#   define mathos_postalign16
#   define __forceinline        inline
#   define __restrict
#endif

// your exe must have this somewhere
#define MATHOS_STATIC()\
    const mathos::mat44 mathos::mat44::identity = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };\
    const mathos::trans3 mathos::trans3::identity = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };\
    const mathos::trans2 mathos::trans2::identity = { 0.0f, 0.0f, 1.0f, 0.0f };\
    const mathos::quat mathos::quat::identity = { 0.0f, 0.0f, 0.0f, 1.0f };\
    const mathos::quat mathos::quat::identityAdditive = { 0.0f, 0.0f, 0.0f, 0.0f };\
    const mathos::vec4 mathos::vec4::zero = { 0.0f, 0.0f, 0.0f, 0.0f };\
    const mathos::vec4 mathos::vec4::one = { 1.0f, 1.0f, 1.0f, 1.0f };\
    const mathos::vec3 mathos::vec3::zero = { 0.0f, 0.0f, 0.0f };\
    const mathos::vec2 mathos::vec2::zero = { 0.0f, 0.0f }

namespace mathos {

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

static const float pi = 3.14159265358979323846f;
static const float pi2 = 3.14159265358979323846f * 2.0f;
static const float pi4 = 3.14159265358979323846f * 4.0f;
static const float halfpi = 1.57079632679489661923f;
static const float quaterpi = 0.785398163397448309616f;
static const float oopi = 0.318309886183790671538f;
static const float e = 2.71828182845904523536f;

} // end of korppu

#include "mathos/vmtypes.h"
#include "mathos/floatmath.h"
#include "mathos/intmath.h"

#endif
