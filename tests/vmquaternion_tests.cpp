// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/math.h"

namespace m = mathos;
namespace v = mathos::vm;

static const float epsilon = 0.000001f;
static const float matrixEpsilon = 0.000001f;//0.007f;

TEST_SUITE(vmQuaternion)
{
    TEST(Identity)
    {
        v::vmvec quat = v::identityQ();
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 4.0f);
        vec = v::transform3(vec, quat);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 3.0f, epsilon);
    }

    TEST(FromMatrix)
    {
        v::vmmat m = v::makeXRotationM(m::degToRad(135.0f));
        v::vmvec q = v::toQuaternion(m);
        v::vmvec v = v::make(0.0f, 0.0f, 1.0f, 0.0f);
        v::vmvec resm = v::transform3(v, m);
        v::vmvec resq = v::transform3(v, q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(resq), v::extractX(resm), epsilon);
        EXPECT_CLOSE(v::extractY(resq), v::extractY(resm), epsilon);
        EXPECT_CLOSE(v::extractZ(resq), v::extractZ(resm), epsilon);
    }

    TEST(ToMatrixAndBack)
    {
        v::vmvec q1 =  v::makeXRotationQ(m::degToRad(135.0f));
        v::vmmat m = v::toMatrix(q1);
        v::vmvec q2 = v::toQuaternion(m);
        EXPECT_CLOSE(v::extractX(v::length4(q2)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(q2), v::extractX(q1), epsilon);
        EXPECT_CLOSE(v::extractY(q2), v::extractY(q1), epsilon);
        EXPECT_CLOSE(v::extractZ(q2), v::extractZ(q1), epsilon);
        EXPECT_CLOSE(v::extractW(q2), v::extractW(q1), epsilon);
    }

    TEST(FromRotationX)
    {
        v::vmvec q = v::makeXRotationQ(m::degToRad(90.0f));
        v::vmvec vec = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), -3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 2.0f, epsilon);
    }

    TEST(FromRotationY)
    {
        v::vmvec q = v::makeYRotationQ(m::degToRad(90.0f));
        v::vmvec vec = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(vec), 3.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), -1.0f, epsilon);
    }

    TEST(FromRotationZ)
    {
        v::vmvec q = v::makeZRotationQ(m::degToRad(90.0f));
        v::vmvec vec = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(vec), -2.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 3.0f, epsilon);
    }

    TEST(FromAxisAngle)
    {
        // TODO test more complicated rotation
        v::vmvec q = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec vec = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), -3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 2.0f, epsilon);
    }

    TEST(FromLookAt)
    {
        v::vmvec eye = v::make(10.0f, 10.0f, 0.0f, 0.0f);
        v::vmvec at = v::make(10.0f, 0.0f, 0.0f, 0.0f);
        v::vmvec up = v::make(0.0f, 0.0f, 1.0f, 0.0f);
        v::vmvec q = v::makeLookAtQ(eye, at, up);
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res = v::transform3(vec, q);
        EXPECT_CLOSE(v::extractX(v::length4(q)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 2.0f, epsilon);
    }

    TEST(Conjugate)
    {
        v::vmvec q = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec invQ = v::conjugateQ(q);
        EXPECT_CLOSE(v::extractX(invQ), -v::extractX(q), epsilon);
        EXPECT_CLOSE(v::extractY(invQ), -v::extractY(q), epsilon);
        EXPECT_CLOSE(v::extractZ(invQ), -v::extractZ(q), epsilon);
        EXPECT_CLOSE(v::extractW(invQ), v::extractW(q), epsilon);

        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec temp = v::transform3(vec, q);
        v::vmvec res = v::transform3(temp, invQ);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 3.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(Add)
    {
        v::vmvec q1 = v::make(1.0f, 2.0f, 3.0f, 1.0f);
        v::vmvec q2 = v::make(5.0f, -6.0f, 7.0f, 1.0f);
        v::vmvec res = v::addQ(q1, q2);
        EXPECT_CLOSE(v::extractX(res), 6.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -4.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 10.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 2.0f, epsilon);
    }

    TEST(Sub)
    {
        v::vmvec q1 = v::make(1.0f, 22.0f, 53.0f, 14.0f);
        v::vmvec q2 = v::make(5.0f, 17.0f, 91.0f, 13.0f);
        v::vmvec res = v::subQ(q1, q2);
        EXPECT_CLOSE(v::extractX(res), -4.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 5.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -38.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(Multiply)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(180.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec qq = v::mulQ(q2, q1);
        v::vmvec vec = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(vec), 3.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), -2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 1.0f, epsilon);
    }

    TODO_TEST(Rotate)
    {
        v::vmvec q = v::identityQ();
        //q = vmQuaternionRotate(q, v::make(0.5f, 0.0f, 0.0f, 0.0f), 0.5f);
        //q = vmQuaternionRotate(q, v::make(m::degToRad(90.0f), 0.0f, 0.0f, 0.0f), 1.0f);
        q = v::rotateQ(q, v::make(1.0f, 0.0f, 0.0f, 0.0f), 0.5f);
        //q = vmQuaternionNormalize(q);
        v::vmvec vec = v::transform3(v::make(0.0f, 0.0f, 1.0f, 0.0f), q);
        EXPECT_CLOSE(v::extractX(vec), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), -1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 0.0f, epsilon);
    }

    TEST(LerpXAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::lerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(0.0f, 1.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 1.0f, epsilon);
    }

    TEST(LerpYAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::lerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -1.0f, epsilon);
    }

    TEST(LerpZAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::lerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
    }

    TEST(Lerp)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec qq = v::lerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(0.0f, 0.0f, 1.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res),  0.666666f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -0.666666f, epsilon);
        EXPECT_CLOSE(v::extractZ(res),  0.333333f, epsilon);
    }

    TEST(SlerpXAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::slerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(0.0f, 1.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 1.0f, epsilon);
    }

    TEST(SlerpYAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::slerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -1.0f, epsilon);
    }

    TEST(SlerpZAxis)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(45.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(135.0f));
        v::vmvec qq = v::slerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec res = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
    }

    TODO_TEST(SlerpDelta)
    {
        v::vmvec q1 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        //v::vmvec q2 = vmQuaternionRotate(q1, v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec qq = v::slerpQ(q1, q2, v::replicate(0.5f));
        v::vmvec v1 = v::transform3(v::make(0.0f, 0.0f, 1.0f, 0.0f), q1);
        v::vmvec v2 = v::transform3(v::make(0.0f, 0.0f, 1.0f, 0.0f), q2);
        v::vmvec res = v::transform3(v::make(0.0f, 0.0f, 1.0f, 0.0f), qq);
        EXPECT_CLOSE(v::extractX(v::length4(qq)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(res), -0.707107f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -0.707107f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
    }

    TEST(Random)
    {
        v::vmvec seed = v::randomSeed();
        v::vmvec q1 = v::randomQuat(seed);
        v::vmvec q2 = v::randomQuat(seed);
        v::vmvec v1 = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), q1);
        v::vmvec v2 = v::transform3(v::make(1.0f, 0.0f, 0.0f, 0.0f), q2);
        EXPECT_CLOSE(v::extractX(v::length4(q1)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(v::length4(q2)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(v::length3(v1)), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(v::length3(v2)), 1.0f, epsilon);
        EXPECT_NOTEQUAL(v::extractX(q1), v::extractX(q2));
        EXPECT_NOTEQUAL(v::extractY(q1), v::extractY(q2));
        EXPECT_NOTEQUAL(v::extractZ(q1), v::extractZ(q2));
    }

    TEST(RotationXAxisMatch)
    {
        v::vmvec q1 = v::makeXRotationQ(m::degToRad(90.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec vec1 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q1);
        v::vmvec vec2 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q2);
        EXPECT_CLOSE(v::extractX(vec1), v::extractX(vec2), epsilon);
        EXPECT_CLOSE(v::extractY(vec1), v::extractY(vec2), epsilon);
        EXPECT_CLOSE(v::extractZ(vec1), v::extractZ(vec2), epsilon);
    }

    TEST(RotationYAxisMatch)
    {
        v::vmvec q1 = v::makeYRotationQ(m::degToRad(90.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec vec1 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q1);
        v::vmvec vec2 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q2);
        EXPECT_CLOSE(v::extractX(vec1), v::extractX(vec2), epsilon);
        EXPECT_CLOSE(v::extractY(vec1), v::extractY(vec2), epsilon);
        EXPECT_CLOSE(v::extractZ(vec1), v::extractZ(vec2), epsilon);
    }

    TEST(RotationZAxisMatch)
    {
        v::vmvec q1 = v::makeZRotationQ(m::degToRad(90.0f));
        v::vmvec q2 = v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec vec1 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q1);
        v::vmvec vec2 = v::transform3(v::make(1.0f, 2.0f, 3.0f, 0.0f), q2);
        EXPECT_CLOSE(v::extractX(vec1), v::extractX(vec2), epsilon);
        EXPECT_CLOSE(v::extractY(vec1), v::extractY(vec2), epsilon);
        EXPECT_CLOSE(v::extractZ(vec1), v::extractZ(vec2), epsilon);
    }
}
