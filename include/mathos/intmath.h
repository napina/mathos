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
#ifndef mathos_intmath_h
#define mathos_intmath_h

namespace mathos {

int8_t  abs(int8_t value);
int16_t abs(int16_t value);
int32_t abs(int32_t value);
int64_t abs(int64_t value);
int8_t  negate(int8_t value);
int16_t negate(int16_t value);
int32_t negate(int32_t value);
int64_t negate(int64_t value);

//int signNegate(int value, int sign);
//int conditionalNegate(int value, bool condition);

template<typename T> T min(T a, T b);
template<typename T> T max(T a, T b);
template<typename T> T clamp(T value, T minValue, T maxValue);
template<typename T> T snap(T value, T gridStep);

} // end of mathos

#include "mathos/intmath.inl"

#endif
