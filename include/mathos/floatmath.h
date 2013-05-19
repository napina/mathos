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
#ifndef mathos_floatmath_h
#define mathos_floatmath_h

namespace mathos {

float abs(float value);
float negate(float value);
float signNegate(float value, float sign);
float conditionalNegate(float value, bool condition);
float min(float value1, float value2);
float max(float value1, float value2);
float clamp01(float value);
float clampZero(float value);
float clampOne(float value);
float clamp(float value, float minValue, float maxValue);
float remap(float value, float srcMin, float srcMax, float dstMin, float dstMax);
float snap(float value, float gridStep);
float lerp(float a, float b, float t);
float floor(float value);
float ceil(float value);
float round(float value);
float truncate(float value);
float fract(float value);
float wrap(float value, float minValue, float maxValue);

float mulByPower(float value, int power);
float mulAdd(float value1, float value2, float addValue);
float negMulSub(float value1, float value2, float subValue);
float divByPower(float value, int power);
float mod(float value, float divider);
void divMod(float value, float divider, float& result, float& remainder);
float angleMod(float value);
float pow(float value, float exponent);
float reciprocal(float value);

// trigonometric
float degToRad(float degrees);
float radToDeg(float radians);
float sin(float angle);
float cos(float angle);
void sinCos(float angle, float& sinResult, float& cosResult);
float sin0HalfPI(float a);
float tan(float angle);
float acos(float cos);
float atan2(float y, float x);
float atan2Positive(float y, float x);
float sqrt(float value);
float reciprocalSqrt(float value);
float reciprocalSqrtEst(float value);

// conditions
bool isPositive(float value);
bool isNegative(float value);
bool isGreater(float value1, float value2);
bool isGreaterThanZero(float value);
bool isGreaterOrEqual(float value1, float value2);
bool isLess(float value1, float value2);
bool isLessOrEqual(float value1, float value2);
bool areEqual(float value1, float value2);
bool areClose(float value1, float value2, float threshold);
bool isCloseZero(float value, float threshold);
bool isInRange(float value, float min, float max);
bool isInBounds(float value, float positiveBound);

} // end of mathos

#include "mathos/floatmath.inl"

#endif
