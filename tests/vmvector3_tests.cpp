// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/math.h"

namespace m = mathos;
namespace v = mathos::vm;

static const float epsilon = 0.000001f;

TEST_SUITE(vmVector3)
{
    TEST(Add)
    {
        v::vmvec v1 = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec v2 = v::make(5.0f, 6.0f, 7.0f, 0.0f);
        v::vmvec res = v::add3(v1, v2);
        EXPECT_CLOSE(v::extractX(res), 6.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 8.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 10.0f, epsilon);
    }

    TEST(Subtract)
    {
        v::vmvec v1 = v::make(1.0f, 22.0f, 53.0f, 0.0f);
        v::vmvec v2 = v::make(5.0f, 17.0f, 91.0f, 0.0f);
        v::vmvec res = v::sub3(v1, v2);
        EXPECT_CLOSE(v::extractX(res), -4.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 5.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -38.0f, epsilon);
    }

    TEST(Cross)
    {
        v::vmvec v1 = v::make(1.0f, 0.0f, 0.0f, 0.0f);
        v::vmvec v2 = v::make(0.0f, 1.0f, 0.0f, 0.0f);
        v::vmvec res = v::cross3(v1, v2);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 1.0f, epsilon);
    }

    TEST(Dot)
    {
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 0.0f);
        v::vmvec res = v::dot3(v, v);
        EXPECT_CLOSE(v::extractX(res), 38.0f, epsilon);
    }

    TEST(Length)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 0.0f);
        v::vmvec res = v::length3(v);
        EXPECT_CLOSE(v::extractX(res), 6.164414f, epsilon);
    }

    TEST(LengthEst)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 0.0f);
        v::vmvec res = v::lengthEst3(v);
        EXPECT_CLOSE(v::extractX(res), 6.164414f, 0.001f);
    }

    TEST(Normalize)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 0.0f);
        v::vmvec v2 = v::normalize3(v);
        v::vmvec res = v::length3(v2);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
    }

    TEST(NormalizeEst)
    {
        v::vmvec v = v::make(2.0f, -3.0f, 5.0f, 0.0f);
        v::vmvec v2 = v::normalizeEst3(v);
        v::vmvec res = v::length3(v2);
        EXPECT_CLOSE(v::extractX(res), 1.0f, 0.002f);
    }

    TEST(Reflect)
    {
        // TODO better test
        v::vmvec v = v::make(2.0f, -4.0f, 0.0f, 0.0f);
        v::vmvec n = v::make(0.0f, 1.0f, 0.0f, 0.0f);
        v::vmvec res = v::reflect3(v, n);
        EXPECT_CLOSE(v::extractX(res), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 4.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
    }

    TEST(Refract)
    {
        v::vmvec v = v::make(0.707107f, -0.707107f, 0.0f, 0.0f);
        v::vmvec n = v::make(0.0f, 1.0f, 0.0f, 0.0f);
        v::vmvec res = v::refract3(v, n, v::replicate(0.5f));
        EXPECT_CLOSE(v::extractX(v::length3(res)), 1.0f, epsilon);
        // TODO check vector components
    }

    TEST(Random01)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec v1 = v::random01(seed);
        v::vmvec v2 = v::random01(seed);
        EXPECT_NOTEQUAL(v::extractX(v1), v::extractX(v2));
        EXPECT_NOTEQUAL(v::extractY(v1), v::extractY(v2));
        EXPECT_NOTEQUAL(v::extractZ(v1), v::extractZ(v2));
    }

    TEST(RandomSigned)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec v1 = v::randomSigned(seed);
        v::vmvec v2 = v::randomSigned(seed);
        EXPECT_NOTEQUAL(v::extractX(v1), v::extractX(v2));
        EXPECT_NOTEQUAL(v::extractY(v1), v::extractY(v2));
        EXPECT_NOTEQUAL(v::extractZ(v1), v::extractZ(v2));
    }

    TEST(RandomNormal)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec v1 = v::randomNormal(seed);
        v::vmvec v2 = v::randomNormal(seed);
        EXPECT_NOTEQUAL(v::extractX(v1), v::extractX(v2));
        EXPECT_NOTEQUAL(v::extractY(v1), v::extractY(v2));
        EXPECT_NOTEQUAL(v::extractZ(v1), v::extractZ(v2));
        EXPECT_CLOSE(v::extractX(v::length3(v1)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(v::length3(v2)), 1.0f, epsilon);
    }
}
