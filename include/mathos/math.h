// Copyright (C) Ville Ruusutie, 2010

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

#if defined(__GNUC__)
#   define __forceinline inline __attribute__((always_inline))
#elif !defined(_MSC_VER) && !defined(__forceinline)
#   define inline __forceinline
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

static const float pi = 3.14159265358979323846f;
static const float pi2 = 3.14159265358979323846f * 2.0f;
static const float pi4 = 3.14159265358979323846f * 4.0f;
static const float halfpi = 1.57079632679489661923f;
static const float quaterpi = 0.785398163397448309616f;
static const float oopi = 0.318309886183790671538f;
static const float e = 2.71828182845904523536f;

} // end of korppu

#include "mathos/vmath.h"
/*#include "mathos/quaternion.h"
#include "mathos/transform3.h"
#include "mathos/transform2.h"
#include "mathos/matrix44.h"
#include "mathos/vector4.h"
#include "mathos/vector3.h"
#include "mathos/vector2.h"
#include "mathos/curve.h"*/

#endif