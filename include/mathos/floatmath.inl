/*=============================================================================

Copyright (c) 2007 Ville Ruusutie

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
#ifndef mathos_floatmath_inl
#define mathos_floatmath_inl

#include "mathos/math.h"
#include <math.h>

namespace mathos {
namespace internal {
    typedef union {float f; int i; } IntOrFloat;

#define float_to_int(fval)  ((int&)(fval))
#define float_to_uint(fval) ((uint32&)(fval))
#define int_to_float(ival)  ((float&)(ival))

#define fval_to_ival(fval,ival) (ival) = *(int*)&(fval)
#define ival_to_fval(ival,fval) (fval) = *(float*)&(ival)
} // end of internal

__forceinline float abs(float fval)
{
#ifdef MATHOS_NO_HW_FLOAT
    int ival;
    m_fval_to_ival(fval, ival);
    ival &= 0x7FFFFFFF;       // strip sign bit
    m_ival_to_fval(ival, fval);
    return fval;
#else
    return ::fabsf(fval);
#endif
}

__forceinline float negate(float fval)
{
    int ival;
    fval_to_ival(fval, ival);
    ival ^= 0x80000000;       // flip sign bit
    ival_to_fval(ival, fval);
    return fval;
}

__forceinline float signNegate(float fval, float sign)
{
    int ival;
    int sval;
    fval_to_ival(fval, ival);
    fval_to_ival(sign, sval);
    ival ^= sval & 0x80000000;
    ival_to_fval(ival, fval);
    return fval;
}

__forceinline float conditionalNegate(float fval, bool condition)
{
    int ival;
    fval_to_ival(fval, ival);
    ival ^= condition ? 0x80000000 : 0;
    ival_to_fval(ival, fval);
    return fval;
}

__forceinline float min(float value1, float value2)
{
    return isLess(value1, value2) ? value1 : value2;
}

__forceinline float max(float value1, float value2)
{
    return isGreater(value1, value2) ? value1 : value2;
}

__forceinline float clamp01(float value)
{
    int s;
    internal::IntOrFloat val;
    val.f = value;
    s = val.i >> 31; // all 1's if sign bit 1
    val.i &= ~s; // 0 if was negative

    // clamp to 1
    val.f = 1.0f - val.f;
    s = val.i >> 31; // all 1's if sign bit 1
    val.i &= ~s; // 0 if was negative
    val.f = 1.0f - val.f;

    return val.f;
}

__forceinline float clampZero(float value)
{
    return (float_to_int(value) < 0) ? 0.0f : value;
}

__forceinline float clampOne(float value)
{
    return min(value, 1.0f);
}

__forceinline float clamp(float fval, float minValue, float maxValue)
{
    return max(minValue, min(maxValue, fval));
}

__forceinline float remap(float value, float srcMin, float srcMax, float dstMin, float dstMax)
{
    float offset = (value - srcMin) / (srcMax - srcMin);
    return dstMin + offset * (dstMax - dstMin);
}

__forceinline float floor(float value)
{
    return ::floorf(value);
}

__forceinline float ceil(float value)
{
    return ::ceilf(value);
}

__forceinline float round(float value)
{
    return (float)(int)(value + (isGreaterThanZero(value) ? 0.5f : -0.5f));
}

__forceinline float truncate(float value)
{
    return (float)(int)value;
}

__forceinline float fract(float value)
{
    return value - truncate(value);
}

__forceinline float wrap(float value, float minValue, float maxValue)
{
    mathos_assert(minValue < maxValue);
    const float range = maxValue - minValue;
    float offset = (value - minValue) / range;
    offset -= floor(offset);
    return range * offset + minValue;
}

__forceinline float mulByPower(float value, int power)
{
    return value * float(1 << power);
}

__forceinline float mulAdd(float value1, float value2, float addValue)
{
    return value1 * value2 + addValue;
}

__forceinline float negMulSub(float value1, float value2, float subValue)
{
    return subValue - value1 * value2;
}

__forceinline float divByPower(float value, int power)
{
    return value / float(1 << power);
}

__forceinline float mod(float value, float divider)
{
    return ::fmodf(value, divider);
}

__forceinline void divMod(float value, float divider, float& result, float& remainder)
{
    // TODO optimize
    result = value / divider;
    remainder = ::fmodf(value, divider);
}

__forceinline float angleMod(float value)
{
    return ::fmodf(value + 4.5f * mathos::pi, mathos::pi2) - mathos::pi;
}

__forceinline float pow(float value, float exponent)
{
    return ::powf(value, exponent);
}

__forceinline float reciprocal(float value)
{
    return 1.0f / value;
}

__forceinline float snap(float value, float gridStep)
{
    return floor((value + gridStep * 0.5f) / gridStep) * gridStep;
}

__forceinline float lerp(float a, float b, float t)
{
    return mulAdd(t, b, negMulSub(t, a, a));
}

__forceinline float degToRad(float degrees)
{
    return degrees * (mathos::pi / 180.0f);
}

__forceinline float radToDeg(float radians)
{
    return radians * (180.0f / mathos::pi);
}

__forceinline float sin(float angle)
{
    return ::sinf(angle);
}

__forceinline float sin0HalfPI(float a)
{
    float s, t;
    s = a * a;
    t = -2.39e-08f;
    t *= s;
    t += 2.7526e-06f;
    t *= s;
    t += -1.98409e-04f;
    t *= s;
    t += 8.3333315e-03f;
    t *= s;
    t += -1.666666664e-01f;
    t *= s;
    t += 1.0f;
    t *= a;
    return t;
}

__forceinline float cos(float angle)
{
    return ::cosf(angle);
}

__forceinline void sinCos(float angle, float& sinResult, float& cosResult)
{
    // TODO optimize?
    //sinCosf(angle, &sinResult, &cosResult);
    sinResult = ::sinf(angle);
    cosResult = ::cosf(angle);
}

__forceinline float acos(float cosValue)
{
    return ::acosf(cosValue);
}

__forceinline float tan(float angle)
{
    return ::tanf(angle);
}

__forceinline float atan2(float y, float x)
{
    return ::atan2f(y, x);
}

__forceinline float atan2Positive(float y, float x)
{
    float a, d, s, t;
    if ( y > x ) {
        a = -x / y;
        d = 1.57079633f;
    } else {
        a = y / x;
        d = 0.0f;
    }
    s = a * a;
    t = 0.0028662257f;
    t *= s;
    t += -0.0161657367f;
    t *= s;
    t += 0.0429096138f;
    t *= s;
    t += -0.0752896400f;
    t *= s;
    t += 0.1065626393f;
    t *= s;
    t += -0.1420889944f;
    t *= s;
    t += 0.1999355085f;
    t *= s;
    t += -0.3333314528f;
    t *= s;
    t += 1.0f;
    t *= a;
    t += d;
    return t;
}

__forceinline float sqrt(float value)
{
    return ::sqrtf(value);
}

__forceinline float reciprocalSqrt(float value)
{
    return 1.0f / ::sqrtf(value);
}

__forceinline float reciprocalSqrtEst(float x)
{
    const float xhalf = 0.5f * x;
    int i;
    fval_to_ival(x, i);
    i = 0x5f3759df - (i >> 1);
    //i = 0x5f375a86 - (i >> 1);
    ival_to_fval(i, x);
    x = x * (1.5f - xhalf * x * x);
    return x;
}

__forceinline bool isPositive(float value)
{
    int ival;
    fval_to_ival(value, ival);
    return (ival & 0x80000000) == 0;
}

__forceinline bool isNegative(float value)
{
    int ival;
    fval_to_ival(value, ival);
    return (ival & 0x80000000) == 0x80000000;
}

__forceinline bool isGreater(float value1, float value2)
{
    int ai;
    int bi;
    fval_to_ival(value1, ai);
    fval_to_ival(value2, bi);
    int test = ai & bi & 0x80000000; // check sign bits
    if(test) {
        return bi > ai; // both negative, reverse compare
    }
    return ai > bi;
}

__forceinline bool isGreaterThanZero(float value)
{
    return isGreater(value, 0.0f);
}

__forceinline bool isGreaterOrEqual(float value1, float value2)
{
    return value1 >= value2;
}

__forceinline bool isLess(float value1, float value2)
{
    int ai;
    int bi;
    fval_to_ival(value1, ai);
    fval_to_ival(value2, bi);
    int test = ai & bi & 0x80000000; // check sign bits
    if(test) {
        return bi < ai; // both negative, reverse compare
    }
    return ai < bi;
}

__forceinline bool isLessOrEqual(float value1, float value2)
{
    return value1 <= value2;
}

__forceinline bool areEqual(float value1, float value2)
{
    return value1 == value2;
}

__forceinline bool areClose(float value1, float value2, float threshold)
{
    float difference = value1 - value2;
    float absValue = mathos::abs(difference);
    return isLess(absValue, threshold);
}

__forceinline bool isCloseZero(float value, float threshold)
{
    float absValue = mathos::abs(value);
    return isLess(absValue, threshold);
}

__forceinline bool isInRange(float value, float min, float max)
{
    return !(isLess(value, min) | isGreater(value, max));
}

__forceinline bool isInBounds(float value, float positiveBound)
{
    return !isGreater(mathos::abs(value), positiveBound);
}

} // end of mathos

#endif
