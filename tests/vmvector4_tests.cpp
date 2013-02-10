// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/math.h"

namespace v = mathos::vm;

static const float epsilon = 0.000001f;
static const float estimateEpsilon = 0.002f;

TEST_SUITE(VMVector4)
{
    TEST(Dot)
    {
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 1.0f);
        v::vmvec res = v::dot4(v, v);
        EXPECT_CLOSE(v::extractX(res), 39.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 39.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 39.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 39.0f, epsilon);
    }

    TEST(Length)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 1.0f);
        v::vmvec res = v::length4(v);
        EXPECT_CLOSE(v::extractX(res), 6.244998f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 6.244998f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 6.244998f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 6.244998f, epsilon);
    }

    TEST(LengthEst)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 1.0f);
        v::vmvec res = v::lengthEst4(v);
        EXPECT_CLOSE(v::extractX(res), 6.244998f, estimateEpsilon);
        EXPECT_CLOSE(v::extractY(res), 6.244998f, estimateEpsilon);
        EXPECT_CLOSE(v::extractZ(res), 6.244998f, estimateEpsilon);
        EXPECT_CLOSE(v::extractW(res), 6.244998f, estimateEpsilon);
    }

    TEST(Normalize)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 1.0f);
        v::vmvec v2 = v::normalize4(v);
        v::vmvec res = v::length4(v2);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
    }

    TEST(NormalizeEst)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 1.0f);
        v::vmvec v2 = v::normalizeEst4(v);
        v::vmvec res = v::length4(v2);
        EXPECT_CLOSE(v::extractX(res), 1.0f, estimateEpsilon);
    }
}
