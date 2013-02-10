// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/math.h"

namespace v = mathos::vm;

static const float epsilon = 0.00001f;
static const float estimateEpsilon = 0.002f;
static const float rcpEstEpsilon = 0.0002f;
static const float rcpSqrtEstEpsilon = 0.0002f;
static const float sqrtEstEpsilon = 0.013f;

TEST_SUITE(VMVector)
{
    TEST(Set)
    {
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 1.0f);
        EXPECT_EQUAL(v::extractX(v), 2.0f);
        EXPECT_EQUAL(v::extractY(v), 3.0f);
        EXPECT_EQUAL(v::extractZ(v), 5.0f);
        EXPECT_EQUAL(v::extractW(v), 1.0f);
    }

    TEST(SetInt)
    {
        v::vmvec v = v::makeInt(1, 2, 3, 4);
        EXPECT_EQUAL(v::extractXInt(v), 1);
        EXPECT_EQUAL(v::extractYInt(v), 2);
        EXPECT_EQUAL(v::extractZInt(v), 3);
        EXPECT_EQUAL(v::extractWInt(v), 4);
    }

    TEST(Zero)
    {
        v::vmvec v = v::splatZero();
        EXPECT_EQUAL(v::extractX(v), 0.0f);
        EXPECT_EQUAL(v::extractY(v), 0.0f);
        EXPECT_EQUAL(v::extractZ(v), 0.0f);
        EXPECT_EQUAL(v::extractW(v), 0.0f);
    }

    TEST(One)
    {
        v::vmvec v = v::splatOne();
        EXPECT_EQUAL(v::extractX(v), 1.0f);
        EXPECT_EQUAL(v::extractY(v), 1.0f);
        EXPECT_EQUAL(v::extractZ(v), 1.0f);
        EXPECT_EQUAL(v::extractW(v), 1.0f);
    }

    TEST(SplatSignMask)
    {
        v::vmvec v = v::splatSignMask();
        EXPECT_EQUAL(v::extractXInt(v), 0x80000000);
        EXPECT_EQUAL(v::extractYInt(v), 0x80000000);
        EXPECT_EQUAL(v::extractZInt(v), 0x80000000);
        EXPECT_EQUAL(v::extractWInt(v), 0x80000000);
    }

    TEST(SplatMask)
    {
        v::vmvec v = v::splatMask(0x12345678);
        EXPECT_EQUAL(v::extractXInt(v), 0x12345678);
        EXPECT_EQUAL(v::extractYInt(v), 0x12345678);
        EXPECT_EQUAL(v::extractZInt(v), 0x12345678);
        EXPECT_EQUAL(v::extractWInt(v), 0x12345678);
    }

    TEST(SplatX)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = v::splatX(v);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 32.0f);
        EXPECT_EQUAL(v::extractZ(res), 32.0f);
        EXPECT_EQUAL(v::extractW(res), 32.0f);
    }

    TEST(SplatY)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = v::splatY(v);
        EXPECT_EQUAL(v::extractX(res), 584.0f);
        EXPECT_EQUAL(v::extractY(res), 584.0f);
        EXPECT_EQUAL(v::extractZ(res), 584.0f);
        EXPECT_EQUAL(v::extractW(res), 584.0f);
    }

    TEST(SplatZ)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = v::splatZ(v);
        EXPECT_EQUAL(v::extractX(res), 3848.0f);
        EXPECT_EQUAL(v::extractY(res), 3848.0f);
        EXPECT_EQUAL(v::extractZ(res), 3848.0f);
        EXPECT_EQUAL(v::extractW(res), 3848.0f);
    }

    TEST(SplatW)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = v::splatW(v);
        EXPECT_EQUAL(v::extractX(res), 483.9f);
        EXPECT_EQUAL(v::extractY(res), 483.9f);
        EXPECT_EQUAL(v::extractZ(res), 483.9f);
        EXPECT_EQUAL(v::extractW(res), 483.9f);
    }

    TEST(Replicate)
    {
        float const v = 438434.f;
        v::vmvec res = v::replicate(v);
        EXPECT_EQUAL(v::extractX(res), v);
        EXPECT_EQUAL(v::extractY(res), v);
        EXPECT_EQUAL(v::extractZ(res), v);
        EXPECT_EQUAL(v::extractW(res), v);
    }

    TEST(Select)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec control1010 = v::makeInt(VM_VECTOR_TRUE, VM_VECTOR_FALSE, VM_VECTOR_TRUE, VM_VECTOR_FALSE);
        v::vmvec control0101 = v::makeInt(VM_VECTOR_FALSE, VM_VECTOR_TRUE, VM_VECTOR_FALSE, VM_VECTOR_TRUE);
        v::vmvec res = v::select(v1, v2, control0101);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 3848.0f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
        res = v::select(res, v2, control1010);
        EXPECT_EQUAL(v::extractX(res), 454.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 12.f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
    }

    TEST(MergeXY)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::mergeXY(v1, v2);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 454.0f);
        EXPECT_EQUAL(v::extractZ(res), 584.0f);
        EXPECT_EQUAL(v::extractW(res), 2334.f);
    }

    TEST(MergeZW)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::mergeZW(v1, v2);
        EXPECT_EQUAL(v::extractX(res), 3848.0f);
        EXPECT_EQUAL(v::extractY(res), 12.f);
        EXPECT_EQUAL(v::extractZ(res), 483.9f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
    }

    TEST(Insert)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = vm_insert(v1, v2, false, true, false, true);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 3848.0f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
        res = vm_insert(res, v2, true, false, true, false);
        EXPECT_EQUAL(v::extractX(res), 454.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 12.f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
    }

    TEST(ShiftLeft)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = vm_shiftLeft(v1, v2, 2);
        EXPECT_EQUAL(v::extractX(res), 3848.0f);
        EXPECT_EQUAL(v::extractY(res), 483.9f);
        EXPECT_EQUAL(v::extractZ(res), 454.0f);
        EXPECT_EQUAL(v::extractW(res), 2334.f);
    }

    TEST(RotateLeft)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = vm_rotateLeft(v, 3);
        EXPECT_EQUAL(v::extractX(res), 483.9f);
        EXPECT_EQUAL(v::extractY(res), 32.0f);
        EXPECT_EQUAL(v::extractZ(res), 584.0f);
        EXPECT_EQUAL(v::extractW(res), 3848.0f);
    }

    TEST(RotateRight)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = vm_rotateRight(v, 2);
        EXPECT_EQUAL(v::extractX(res), 3848.0f);
        EXPECT_EQUAL(v::extractY(res), 483.9f);
        EXPECT_EQUAL(v::extractZ(res), 32.0f);
        EXPECT_EQUAL(v::extractW(res), 584.0f);
    }

    TEST(Swizzle)
    {
        v::vmvec v = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec res = vm_swizzle(v, 1, 3, 0, 2);
        EXPECT_EQUAL(v::extractX(res), 584.0f);
        EXPECT_EQUAL(v::extractY(res), 483.9f);
        EXPECT_EQUAL(v::extractZ(res), 32.0f);
        EXPECT_EQUAL(v::extractW(res), 3848.0f);
    }

    TEST(Permute)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::permute(v1, v2, VM_PERMUTE_0Z, VM_PERMUTE_1Y, VM_PERMUTE_0W, VM_PERMUTE_1X);
        EXPECT_EQUAL(v::extractX(res), 3848.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 483.9f);
        EXPECT_EQUAL(v::extractW(res), 454.0f);
    }

    TEST(PermuteV)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, 3848.0f, 483.9f);
        v::vmvec v2 = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res1 = v::permute(v1, v2, VM_PERMUTE_0Z, VM_PERMUTE_1Y, VM_PERMUTE_0W, VM_PERMUTE_1X);
        v::vmvec res2 = v::permute(v1, v2, v::makeInt(VM_PERMUTE_0Z, VM_PERMUTE_1Y, VM_PERMUTE_0W, VM_PERMUTE_1X));
        EXPECT_EQUAL(v::extractX(res2), 3848.0f);
        EXPECT_EQUAL(v::extractY(res2), 2334.f);
        EXPECT_EQUAL(v::extractZ(res2), 483.9f);
        EXPECT_EQUAL(v::extractW(res2), 454.0f);
        EXPECT_EQUAL(v::allEqual(res1, res2), VM_TRUE);
    }

    TEST(Abs)
    {
        v::vmvec v = v::make(32.0f, -584.0f, -3848.0f, 483.9f);
        v::vmvec res = v::abs4(v);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 584.0f);
        EXPECT_EQUAL(v::extractZ(res), 3848.0f);
        EXPECT_EQUAL(v::extractW(res), 483.9f);
    }

    TEST(Negate)
    {
        v::vmvec v = v::make(32.0f, -584.0f, -3848.0f, 483.9f);
        v::vmvec res = v::neg4(v);
        EXPECT_EQUAL(v::extractX(res), -32.0f);
        EXPECT_EQUAL(v::extractY(res), 584.0f);
        EXPECT_EQUAL(v::extractZ(res), 3848.0f);
        EXPECT_EQUAL(v::extractW(res), -483.9f);
    }

    TEST(Min)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, -3848.0f, -483.9f);
        v::vmvec v2 = v::make(-454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::min4(v1, v2);
        EXPECT_EQUAL(v::extractX(res), -454.0f);
        EXPECT_EQUAL(v::extractY(res), 584.0f);
        EXPECT_EQUAL(v::extractZ(res), -3848.0f);
        EXPECT_EQUAL(v::extractW(res), -483.9f);
    }

    TEST(Max)
    {
        v::vmvec v1 = v::make(32.0f, 584.0f, -3848.0f, -483.9f);
        v::vmvec v2 = v::make(-454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::max4(v1, v2);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), 2334.f);
        EXPECT_EQUAL(v::extractZ(res), 12.f);
        EXPECT_EQUAL(v::extractW(res), 992.f);
    }

    TEST(Fract)
    {
        v::vmvec v = v::make(32.238311f, -64.252220f, -128.512525f, 483.954933f);
        v::vmvec res = v::fract4(v);
        EXPECT_CLOSE(v::extractX(res), 0.238311f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -0.252220f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -0.512525f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.954933f, epsilon);
    }

    TEST(Round)
    {
        v::vmvec v = v::make(32.2383f, -584.49990f, -3848.50001f, 483.9549f);
        v::vmvec res = v::round4(v);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), -584.0f);
        EXPECT_EQUAL(v::extractZ(res), -3849.0f);
        EXPECT_EQUAL(v::extractW(res), 484.0f);
    }

    TEST(Floor)
    {
        v::vmvec v = v::make(32.2383f, -584.49999f, -3848.50001f, 483.9549f);
        v::vmvec res = v::floor4(v);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), -585.0f);
        EXPECT_EQUAL(v::extractZ(res), -3849.0f);
        EXPECT_EQUAL(v::extractW(res), 483.0f);
    }

    TEST(Ceil)
    {
        v::vmvec v = v::make(32.2383f, -584.49999f, -3848.50001f, 483.9549f);
        v::vmvec res = v::ceil4(v);
        EXPECT_EQUAL(v::extractX(res), 33.0f);
        EXPECT_EQUAL(v::extractY(res), -584.0f);
        EXPECT_EQUAL(v::extractZ(res), -3848.0f);
        EXPECT_EQUAL(v::extractW(res), 484.0f);
    }

    TEST(Truncate)
    {
        v::vmvec v = v::make(32.2383f, -584.49999f, -3848.50001f, 483.9549f);
        v::vmvec res = v::truncate4(v);
        EXPECT_EQUAL(v::extractX(res), 32.0f);
        EXPECT_EQUAL(v::extractY(res), -584.0f);
        EXPECT_EQUAL(v::extractZ(res), -3848.0f);
        EXPECT_EQUAL(v::extractW(res), 483.0f);
    }

    TEST(Saturate)
    {
        v::vmvec v = v::make(0.2383f, -1.49999f, 0.98989f, 3.323f);
        v::vmvec res = v::saturate4(v);
        EXPECT_CLOSE(v::extractX(res), 0.2383f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.98989f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(Clamp)
    {
        v::vmvec v = v::make(34.0f, 12.0f, -2324.0f, -10329.f);
        v::vmvec min = v::make(-32.0f, 584.0f, -3848.0f, -483.9f);
        v::vmvec max = v::make(454.0f, 2334.f, 12.f, 992.f);
        v::vmvec res = v::clamp4(v, min, max);
        EXPECT_EQUAL(v::extractX(res), 34.0f);
        EXPECT_EQUAL(v::extractY(res), 584.0f);
        EXPECT_EQUAL(v::extractZ(res), -2324.0f);
        EXPECT_EQUAL(v::extractW(res), -483.9f);
    }

    TEST(Add)
    {
        v::vmvec v1 = v::make(32.5f, 584.2f, -3848.5f, -483.8f);
        v::vmvec v2 = v::make(-454.2f, 2334.3f, 12.1f, 992.3f);
        v::vmvec res = v::add4(v1, v2);
        EXPECT_EQUAL(v::extractX(res), -421.7f);
        EXPECT_EQUAL(v::extractY(res), 2918.5f);
        EXPECT_EQUAL(v::extractZ(res), -3836.4f);
        EXPECT_EQUAL(v::extractW(res), 508.5f);
    }

    TEST(Subtract)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.5f, -483.8f);
        v::vmvec v2 = v::make(-454.2f, 2234.5f, 12.1f, 992.3f);
        v::vmvec res = v::sub4(v1, v2);
        EXPECT_CLOSE(v::extractX(res), 486.7f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -1648.1f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -3860.6f, epsilon);
        EXPECT_CLOSE(v::extractW(res), -1476.1f, epsilon);
    }

    TEST(Multiply)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -38.5f, -483.8f);
        v::vmvec v2 = v::make(-454.2f, 223.5f, 12.1f, -992.3f);
        v::vmvec res = v::mul4(v1, v2);
        EXPECT_CLOSE(v::extractX(res), -14761.5f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 131060.40625f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -465.85f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 480074.71875f, epsilon);
    }

    TEST(MultiplyAdd)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -38.5f, -483.8f);
        v::vmvec v2 = v::make(-454.2f, 223.5f, 12.1f, -992.3f);
        v::vmvec v3 = v::make(-1.0f, 2.0f, 4.0f, -8.0f);
        v::vmvec res = v::mulAdd4(v1, v2, v3);
        EXPECT_CLOSE(v::extractX(res), -14762.5f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 131062.40625f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -461.85f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 480066.71875f, epsilon);
    }

    TEST(NegativeMultiplySubtract)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -38.5f, -483.8f);
        v::vmvec v2 = v::make(-454.2f, 223.5f, 12.1f, -992.3f);
        v::vmvec v3 = v::make(-1.0f, 2.0f, 4.0f, -8.0f);
        v::vmvec res = v::negMulSub4(v1, v2, v3);
        EXPECT_CLOSE(v::extractX(res), 14760.5f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -131058.40625f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 469.85f, epsilon);
        EXPECT_CLOSE(v::extractW(res), -480082.71875f, epsilon);
    }

    TEST(Divide)
    {
        v::vmvec v1 = v::make(3232.5f, 586.4f, -38.5f, -483.8f);
        v::vmvec v2 = v::make(-45.2f, 223.5f, 12.1f, -992.3f);
        v::vmvec res = v::div4(v1, v2);
        EXPECT_CLOSE(v::extractX(res), -71.515487f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 2.623714f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -3.181818f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.487554f, epsilon);
    }

    TEST(Sqrt)
    {
        v::vmvec v = v::make(232.4f, 7653.554f, 546.3f, 8374.3f);
        v::vmvec res = v::sqrt4(v);
        EXPECT_CLOSE(v::extractX(res), 15.244671f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 87.4845929f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 23.373061f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 91.511201f, epsilon);
    }

    TEST(SqrtEst)
    {
        v::vmvec v = v::make(232.4f, 7653.554f, 546.3f, 8374.3f);
        v::vmvec res = v::sqrtEst4(v);
        EXPECT_CLOSE(v::extractX(res), 15.244671f, sqrtEstEpsilon);
        EXPECT_CLOSE(v::extractY(res), 87.484593f, sqrtEstEpsilon);
        EXPECT_CLOSE(v::extractZ(res), 23.373061f, sqrtEstEpsilon);
        EXPECT_CLOSE(v::extractW(res), 91.511201f, sqrtEstEpsilon);
    }

    TEST(ReciprocalSqrt)
    {
        v::vmvec v = v::make(232.4f, 76536.3f, 546.3f, 8374.3f);
        v::vmvec res = v::reciprocalSqrt4(v);
        EXPECT_CLOSE(v::extractX(res), 0.065597f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.003615f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.042784f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.010928f, epsilon);
    }

    TEST(ReciprocalSqrtEst)
    {
        v::vmvec v = v::make(232.4f, 76536.3f, 546.3f, 8374.3f);
        v::vmvec res = v::reciprocalSqrtEst4(v);
        EXPECT_CLOSE(v::extractX(res), 0.065597f, rcpSqrtEstEpsilon);
        EXPECT_CLOSE(v::extractY(res), 0.003615f, rcpSqrtEstEpsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.042784f, rcpSqrtEstEpsilon);
        EXPECT_CLOSE(v::extractW(res), 0.010928f, rcpSqrtEstEpsilon);
    }

    TEST(Reciprocal)
    {
        v::vmvec v = v::make(232.4f, 7.3f, 5.3f, 83.3f);
        v::vmvec res = v::reciprocal4(v);
        EXPECT_CLOSE(v::extractX(res), 0.004303f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.136986f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.188679f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.012005f, epsilon);
    }

    TEST(ReciprocalEst)
    {
        v::vmvec v = v::make(232.4f, 7.3f, 5.3f, 83.3f);
        v::vmvec res = v::reciprocalEst4(v);
        EXPECT_CLOSE(v::extractX(res), 0.004303f, rcpEstEpsilon);
        EXPECT_CLOSE(v::extractY(res), 0.136986f, rcpEstEpsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.188679f, rcpEstEpsilon);
        EXPECT_CLOSE(v::extractW(res), 0.012005f, rcpEstEpsilon);
    }

    TEST(Sin)
    {
        v::vmvec v = v::make(1.234f, 4.3283f, 79.383883f, -8.333f);
        v::vmvec res = v::sin4(v);
        EXPECT_CLOSE(v::extractX(res), 0.943818209f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -0.92714019f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -0.7473513f, epsilon);
        EXPECT_CLOSE(v::extractW(res), -0.887447793f, epsilon);
    }

    TEST(SinZeroHalfPI)
    {
        v::vmvec v = v::make(0.123456f, 0.565656f, 1.0f, 1.515151f);
        v::vmvec res = v::sinZeroHalfPI4(v);
        v::vmvec s = v::sin4(v);
        EXPECT_CLOSE(v::extractX(res), 0.123142f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.535969f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.841470f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.998452f, epsilon);
    }

    TEST(Cos)
    {
        v::vmvec v = v::make(1.234f, 4.3283f, 79.383883f, -8.333f);
        v::vmvec res = v::cos4(v);
        EXPECT_CLOSE(v::extractX(res), 0.330465108f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -0.374714649f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -0.664429104f, epsilon);
        EXPECT_CLOSE(v::extractW(res), -0.460908249f, epsilon);
    }

    TEST(ATanPositive)
    {
        v::vmvec y = v::make(1.234f, 4.3283f, 79.383883f, 8.333f);
        v::vmvec x = v::make(10.123456f, 0.565656f, 1.0f, 121.515151f);
        v::vmvec res = v::atanPositive4(y, x);
        EXPECT_CLOSE(v::extractX(res), 0.121296f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 1.440845f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 1.558199f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.068468f, epsilon);
    }

    TEST(Random01)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec v1 = v::random01(seed);
        v::vmvec v2 = v::random01(seed);
        EXPECT_NOTEQUAL(v::extractX(v1), v::extractX(v2));
        EXPECT_NOTEQUAL(v::extractY(v1), v::extractY(v2));
        EXPECT_NOTEQUAL(v::extractZ(v1), v::extractZ(v2));
        EXPECT_NOTEQUAL(v::extractW(v1), v::extractW(v2));
    }

    TEST(RandomSigned)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec v1 = v::randomSigned(seed);
        v::vmvec v2 = v::randomSigned(seed);
        EXPECT_NOTEQUAL(v::extractX(v1), v::extractX(v2));
        EXPECT_NOTEQUAL(v::extractY(v1), v::extractY(v2));
        EXPECT_NOTEQUAL(v::extractZ(v1), v::extractZ(v2));
        EXPECT_NOTEQUAL(v::extractW(v1), v::extractW(v2));
    }

    TEST(Equal)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.5f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isEqual(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_TRUE);
    }

    TEST(NotEqual)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.5f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isNotEqual(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_FALSE);
    }

    TEST(Less)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.6f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isLess(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_FALSE);
    }

    TEST(LessOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.6f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isLessOrEqual(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_TRUE);
    }

    TEST(Greater)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.6f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isGreater(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_FALSE);
    }

    TEST(GreaterOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.6f, 585.4f, -3848.5f, -483.8f);
        v::vmvec res = v::isGreaterOrEqual(v1, v2);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_TRUE);
    }

    TEST(InBounds)
    {
        v::vmvec v = v::make(34.0f, 12.0f, -2324.0f, -1032.f);
        v::vmvec b = v::make(454.0f, 2.f, 2355.f, 992.f);
        v::vmvec res = v::isInBounds(v, b);
        EXPECT_EQUAL(v::extractXInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractYInt(res), VM_VECTOR_FALSE);
        EXPECT_EQUAL(v::extractZInt(res), VM_VECTOR_TRUE);
        EXPECT_EQUAL(v::extractWInt(res), VM_VECTOR_FALSE);
    }

    TEST(AllEqual)
    {
        v::vmvec v1 = v::make(32.5f, 586.4f, -3848.4f, -483.8f);
        v::vmvec v2 = v::make(32.6f, 585.4f, -3848.5f, -483.8f);
        EXPECT_EQUAL(v::allEqual(v1, v2), VM_FALSE);
        EXPECT_EQUAL(v::allEqual(v1, v1), VM_TRUE);
        EXPECT_EQUAL(v::allEqual(v2, v2), VM_TRUE);
    }

    TEST(AllLess)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(33.5f, 585.4f, -3848.5f, -483.7f);
        v::vmvec v3 = v::make(33.5f, 585.4f, -3848.5f, -482.7f);
        EXPECT_EQUAL(v::allLess(v1, v2), VM_FALSE);
        EXPECT_EQUAL(v::allLess(v1, v3), VM_TRUE);
    }

    TEST(AllLessOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(33.5f, 585.4f, -3848.5f, -484.7f);
        v::vmvec v3 = v::make(33.5f, 585.4f, -3848.5f, -483.7f);
        EXPECT_EQUAL(v::allLessOrEqual(v1, v2), VM_FALSE);
        EXPECT_EQUAL(v::allLessOrEqual(v1, v3), VM_TRUE);
        EXPECT_EQUAL(v::allLessOrEqual(v1, v1), VM_TRUE);
    }

    TEST(AllGreater)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(33.5f, 585.4f, -3848.5f, -483.7f);
        v::vmvec v3 = v::make(33.5f, 585.4f, -3848.5f, -482.7f);
        EXPECT_EQUAL(v::allGreater(v2, v1), VM_FALSE);
        EXPECT_EQUAL(v::allGreater(v3, v1), VM_TRUE);
    }

    TEST(AllGreaterOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(33.5f, 585.4f, -3848.5f, -484.7f);
        v::vmvec v3 = v::make(33.5f, 585.4f, -3848.5f, -483.7f);
        EXPECT_EQUAL(v::allGreaterOrEqual(v2, v1), VM_FALSE);
        EXPECT_EQUAL(v::allGreaterOrEqual(v3, v1), VM_TRUE);
        EXPECT_EQUAL(v::allGreaterOrEqual(v1, v1), VM_TRUE);
    }

    TEST(AnyEqual)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(33.5f, 585.4f, -3848.5f, -484.7f);
        v::vmvec v3 = v::make(34.5f, 585.4f, -3847.5f, -483.7f);
        EXPECT_EQUAL(v::anyEqual(v1, v2), VM_FALSE);
        EXPECT_EQUAL(v::anyEqual(v1, v3), VM_TRUE);
        EXPECT_EQUAL(v::anyEqual(v2, v3), VM_TRUE);
    }

    TEST(AnyLess)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(32.5f, 584.4f, -3848.5f, -483.7f);
        EXPECT_EQUAL(v::anyLess(v1, v1), VM_FALSE);
        EXPECT_EQUAL(v::anyLess(v1, v2), VM_TRUE);
    }

    TEST(AnyLessOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(31.5f, 583.4f, -3848.5f, -483.7f);
        v::vmvec v3 = v::make(31.5f, 583.4f, -3850.5f, -484.7f);
        EXPECT_EQUAL(v::anyLessOrEqual(v1, v1), VM_TRUE);
        EXPECT_EQUAL(v::anyLessOrEqual(v1, v2), VM_TRUE);
        EXPECT_EQUAL(v::anyLessOrEqual(v1, v3), VM_FALSE);
    }

    TEST(AnyGreater)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3848.5f, -483.7f);
        v::vmvec v2 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        EXPECT_EQUAL(v::anyGreater(v1, v1), VM_FALSE);
        EXPECT_EQUAL(v::anyGreater(v1, v2), VM_TRUE);
    }

    TEST(AnyGreaterOrEqual)
    {
        v::vmvec v1 = v::make(32.5f, 584.4f, -3849.5f, -483.7f);
        v::vmvec v2 = v::make(31.5f, 583.4f, -3848.5f, -483.7f);
        v::vmvec v3 = v::make(31.5f, 583.4f, -3850.5f, -484.7f);
        EXPECT_EQUAL(v::anyGreaterOrEqual(v1, v1), VM_TRUE);
        EXPECT_EQUAL(v::anyGreaterOrEqual(v2, v1), VM_TRUE);
        EXPECT_EQUAL(v::anyGreaterOrEqual(v3, v1), VM_FALSE);
    }
}
