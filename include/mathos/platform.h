/*=============================================================================

Copyright (c) 2010-2013 Ville Ruusutie

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
#ifndef mathos_platform_h
#define mathos_platform_h

#if defined(__linux__) && defined(__ELF__)
#   define __MATHOS_LINUX__ (1)
#elif defined(WIN32) || defined(_WIN32)
#   define __MATHOS_WIN32__ (1)
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#elif defined(__APPLE__) && defined(__MACH__)
#   if defined(_ARM)
#       define __MATHOS_IOS__ (1)
#   else
#       define __MATHOS_MACOSX__ (1)
#   endif
#else
#error Could not determine your operating system in platform.h
#endif

#if !defined(_MSC_VER) && !defined(__forceinline)
#   if defined(__GNUC__)
#       define __forceinline inline __attribute__((always_inline))
#   else
#       define __forceinline inline
#   endif
#endif

#if !defined(_MSC_VER)
typedef signed long long __int64;
#endif
typedef unsigned int uint32;

#if defined(_MSC_VER)
#   define PREALIGN16 __declspec(align(16))
#   define POSTALIGN16
#elif defined(__GNUC__)
#   define PREALIGN16
#   define POSTALIGN16 __attribute__((aligned(16)))
#else
#error unknown compiler
#endif

// Swizzle define helpers
#define STYPE_x 0
#define STYPE_y 1
#define STYPE_z 2
#define STYPE_w 3
#define SD_F(OFFSET,SIZE)\
    struct {\
        operator float() const { return f[OFFSET]; }\
        float operator=(float v) { f[OFFSET] = v; return f[OFFSET]; }\
        float operator+=(float v) { f[OFFSET] += v; return f[OFFSET]; }\
        float operator-=(float v) { f[OFFSET] -= v; return f[OFFSET]; }\
        float operator*=(float v) { f[OFFSET] *= v; return f[OFFSET]; }\
        float operator/=(float v) { f[OFFSET] /= v; return f[OFFSET]; }\
        float f[SIZE];\
    }
#define S_V2(X,Y,SIZE)\
    struct _##X##Y {\
        operator vec2() const { return vec2::make(f[STYPE_##X], f[STYPE_##Y]); }\
        vec2 operator=(vec2 v) { f[STYPE_##X] = v.x; f[STYPE_##Y] = v.y; return *this; }\
        float f[SIZE];\
    } ##X##Y
#define S_V2G(X,Y,SIZE)\
    struct _##X##Y {\
        operator vec2() const { return vec2::make(f[STYPE_##X], f[STYPE_##Y]); }\
        float f[SIZE];\
    } ##X##Y
#define S_V3(X,Y,Z,SIZE)\
    struct _##X##Y##Z {\
        operator vec3() const { return vec3::make(f[STYPE_##X], f[STYPE_##Y], f[STYPE_##Z]); }\
        vec3 operator=(vec3 v) { f[STYPE_##X] = v.x; f[STYPE_##Y] = v.y; f[STYPE_##Z] = v.z; return *this; }\
        float f[SIZE];\
    } X##Y##Z
#define S_V3G(X,Y,Z,SIZE)\
    struct _##X##Y##Z {\
        operator vec3() const { return vec3::make(f[STYPE_##X], f[STYPE_##Y], f[STYPE_##Z]); }\
        float f[SIZE];\
    } X##Y##Z
#define S_V4(X,Y,Z,W,SIZE)\
    struct _##X##Y##Z##W {\
        operator vec4() const { return vec4::make(f[STYPE_##X], f[STYPE_##Y], f[STYPE_##Z], f[STYPE_##W]); }\
        vec4 operator=(vec4 v) { f[STYPE_##X] = v.x; f[STYPE_##Y] = v.y; f[STYPE_##Z] = v.z; f[STYPE_##W] = v.w; return *this; }\
        float f[SIZE];\
    } X##Y##Z##W
#define S_V4G(X,Y,Z,W,SIZE)\
    struct _##X##Y##Z##W {\
        operator vec4() const { return vec4::make(f[STYPE_##X], f[STYPE_##Y], f[STYPE_##Z], f[STYPE_##W]); }\
        float f[SIZE];\
    } X##Y##Z##W

#define S2_V2(X,Y)      S_V2(X,Y,2)
#define S2_V2G(X,Y)     S_V2G(X,Y,2)
#define S2_V3(X,Y,Z)    S_V3(X,Y,Z,2)
#define S2_V3G(X,Y,Z)   S_V3G(X,Y,Z,2)
#define S2_V4(X,Y,Z,W)  S_V4(X,Y,Z,W,2)
#define S2_V4G(X,Y,Z,W) S_V4G(X,Y,Z,W,2)
#define S3_V2(X,Y)      S_V2(X,Y,3)
#define S3_V2G(X,Y)     S_V2G(X,Y,3)
#define S3_V3(X,Y,Z)    S_V3(X,Y,Z,3)
#define S3_V3G(X,Y,Z)   S_V3G(X,Y,Z,3)
#define S3_V4(X,Y,Z,W)  S_V4(X,Y,Z,W,3)
#define S3_V4G(X,Y,Z,W) S_V4G(X,Y,Z,W,3)
#define S4_V2(X,Y)      S_V2(X,Y,4)
#define S4_V2G(X,Y)     S_V2G(X,Y,4)
#define S4_V3(X,Y,Z)    S_V3(X,Y,Z,4)
#define S4_V3G(X,Y,Z)   S_V3G(X,Y,Z,4)
#define S4_V4(X,Y,Z,W)  S_V4(X,Y,Z,W,4)
#define S4_V4G(X,Y,Z,W) S_V4G(X,Y,Z,W,4)

//#define MATHOS_NO_HW_FLOAT (1)

#endif
