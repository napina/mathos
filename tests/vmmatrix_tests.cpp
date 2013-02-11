// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/vmath.h"

namespace m = mathos;
namespace v = mathos::vm;

static const float epsilon = 0.000001f;
static const float estimateEpsilon = 0.002f;

TEST_SUITE(vmMatrix)
{
    TEST(Identity)
    {
        v::vmmat m = v::identityM();
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 5.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromTranslation)
    {
        v::vmmat m = v::makeTranslationM(v::make(10.0f, 20.f, 30.0f, 40.0f));
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), 12.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 23.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 35.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromScale)
    {
        v::vmmat m = v::makeScaleM(v::replicate(10.0f));
        v::vmvec v = v::make(2.0f, 3.0f, 5.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), 20.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 30.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 50.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromRotationX)
    {
        v::vmmat m = v::makeXRotationM(m::degToRad(90.0f));
        v::vmvec v = v::make(0.0f, 0.0f, 1.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res),  1.0f, epsilon);
    }

    TEST(FromRotationY)
    {
        v::vmmat m = v::makeYRotationM(m::degToRad(90.0f));
        v::vmvec v = v::make(0.0f, 0.0f, 1.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromRotationZ)
    {
        v::vmmat m = v::makeZRotationM(m::degToRad(90.0f));
        v::vmvec v = v::make(0.0f, 1.0f, 0.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), -1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromAxisAngle)
    {
        // TODO test more complicated rotation
        v::vmmat m = v::makeAxisAngleM(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f));
        v::vmvec v = v::make(0.0f, 0.0f, 1.0f, 1.0f);
        v::vmvec res = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 1.0f, epsilon);
    }

    TEST(FromQuaternion)
    {
        v::vmvec q = v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(135.0f));
        v::vmmat m = v::toMatrix(q);
        v::vmvec v = v::make(0.0f, 1.0f, 0.0f, 0.0f);
        v::vmvec resq = v::transform3(v, q);
        v::vmvec resm = v::transform3(v, m);
        EXPECT_CLOSE(v::extractX(resm), v::extractX(resq), epsilon);
        EXPECT_CLOSE(v::extractY(resm), v::extractY(resq), epsilon);
        EXPECT_CLOSE(v::extractZ(resm), v::extractZ(resq), epsilon);
        //EXPECT_CLOSE(v::extractW(resm), v::extractW(resq), epsilon);
    }

    TEST(ToQuaternionAndBack)
    {
        v::vmmat m1 =  v::makeXRotationM(m::degToRad(135.0f));
        v::vmvec q = v::toQuaternion(m1);
        v::vmmat m2 = v::toMatrix(q);
        v::vmvec v = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res1 = v::transform3(v, m1);
        v::vmvec res2 = v::transform3(v, m2);
        EXPECT_CLOSE(v::extractX(res2), v::extractX(res1), epsilon);
        EXPECT_CLOSE(v::extractY(res2), v::extractY(res1), epsilon);
        EXPECT_CLOSE(v::extractZ(res2), v::extractZ(res1), epsilon);
        EXPECT_CLOSE(v::extractW(res2), v::extractW(res1), epsilon);
    }

    TEST(FromLookAt)
    {
        v::vmvec eye = v::make(10.0f, 10.0f, 0.0f, 0.0f);
        v::vmvec at = v::make(10.0f, 0.0f, 0.0f, 0.0f);
        v::vmvec up = v::make(0.0f, 0.0f, 1.0f, 0.0f);
        v::vmmat m = v::makeLookAtM(eye, at, up);
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res = v::transform3(vec, m);
        EXPECT_CLOSE(v::extractX(res), 11.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 7.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 2.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(FromLookAtRotation)
    {
        v::vmvec eye = v::make(10.0f, 10.0f, 0.0f, 0.0f);
        v::vmvec at = v::make(10.0f, 0.0f, 0.0f, 0.0f);
        v::vmvec up = v::make(0.0f, 0.0f, 1.0f, 0.0f);
        v::vmmat m = v::makeLookAtRotationM(eye, at, up);
        EXPECT_CLOSE(v::extractX(m.row0),  1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(m.row0),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(m.row0),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractX(m.row1),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(m.row1),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(m.row1),  1.0f, epsilon);
        EXPECT_CLOSE(v::extractX(m.row2),  0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(m.row2), -1.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(m.row2),  0.0f, epsilon);
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res = v::transform3(vec, m);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 2.0f, epsilon);
    }
/*
v::vmmat vmMatrixFromTransform(vmTransformParam t);
v::vmmat vmMatrixTranspose(vmMatrixParam m);
v::vmmat vmMatrixMultiply(vmMatrixParam m1, vmMatrixParam m2);
v::vmmat vmMatrixMultiplyTranspose(vmMatrixParam m1, vmMatrixParam m2);
v::vmmat vmMatrixTranslate(vmMatrixParam m, vmVectorParam v);
v::vmmat vmMatrixScale(vmMatrixParam m, vmVectorParam s);
v::vmmat vmMatrixInvert(vmMatrixParam m);
v::vmmat vmMatrixInvertRotationTranslation(vmMatrixParam m);
v::vmmat vmMatrixLookAtRotation(vmVectorFastParam eye, vmVectorFastParam at, vmVectorFastParam up);
*/
}
