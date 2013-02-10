// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "mathos/math.h"

namespace m = mathos;
namespace v = mathos::vm;

static const float epsilon = 0.000005f;

TEST_SUITE(vmTransform)
{
    TEST(Identity)
    {
        v::vmtrans transform = v::identityT();
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 4.0f);
        vec = v::transform3(vec, transform);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 3.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(vec), 4.0f, epsilon);
    }

    TEST(Translate)
    {
        v::vmtrans transform = v::identityT();
        v::vmvec vec = v::splatZero();
        transform = v::translateT(transform, v::make(1.0f, 2.0f, 3.0f, 0.0f));
        vec = v::transform3(vec, transform);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 3.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(vec), 0.0f, epsilon);
    }

    TEST(Scale)
    {
        v::vmtrans transform = v::identityT();
        transform = v::scaleT(transform, v::replicate(3.0f));
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        vec = v::transform3(vec, transform);
        EXPECT_CLOSE(v::extractX(vec), 3.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), 6.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 9.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(vec), 0.0f, epsilon);
    }

    TEST(Rotate)
    {
        v::vmtrans transform = v::identityT();
        transform = v::rotateT(transform, v::makeXRotationQ(m::degToRad(90.0f)));
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        vec = v::transform3(vec, transform);
        EXPECT_CLOSE(v::extractX(vec), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(vec), -3.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(vec), 2.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(vec), 0.0f, epsilon);
    }

    TEST(Invert)
    {
        v::vmtrans t = v::identityT();
        t = v::translateT(t, v::make(1.0f, 2.0f, 3.0f, 4.0f));
        t = v::scaleT(t, v::replicate(2.0f));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f)));
        v::vmtrans tInv = v::invertT(t);
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec temp = v::transform3(vec, t);
        v::vmvec res = v::transform3(temp, tInv);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 3.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(Combine)
    {
        v::vmtrans t1 = v::identityT();
        t1 = v::translateT(t1, v::make(1.0f, 2.0f, 3.0f, 4.0f));
        t1 = v::scaleT(t1, v::replicate(2.0f));
        t1 = v::rotateT(t1, v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f)));
        v::vmtrans t2 = v::identityT();
        t2 = v::translateT(t2, v::make(5.0f, 6.0f, 7.0f, 8.0f));
        t2 = v::scaleT(t2, v::replicate(5.0f));
        t2 = v::rotateT(t2, v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(90.0f)));
        v::vmtrans tt = v::combineT(t1, t2);
        v::vmvec vec = v::make(0.25f, 0.75f, 0.5f, 0.0f);
        v::vmvec res = v::transform3(vec, tt);
        EXPECT_CLOSE(v::extractX(res), 3.5f, epsilon);
        EXPECT_CLOSE(v::extractY(res), -17.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 17.5f, epsilon);
        //EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(FromLookAt)
    {
        v::vmvec eye = v::make(10.0f, 10.0f, 0.0f, 0.0f);
        v::vmvec at = v::make(10.0f, 0.0f, 0.0f, 0.0f);
        v::vmvec up = v::make(0.0f, 0.0f, 1.0f, 0.0f);
        v::vmtrans t = v::makeLookAtT(eye, at, up);
        v::vmvec vec = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res = v::transform3(vec, t);
        EXPECT_CLOSE(v::extractX(res), 11.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 7.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 2.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(Transform)
    {
        v::vmtrans t = v::identityT();
        t = v::translateT(t, v::make(1.0f, 2.0f, 3.0f, 4.0f));
        t = v::scaleT(t, v::replicate(3.0f));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f)));
        v::vmvec v = v::make(0.0f, 5.0f, 0.0f, 0.0f);
        v::vmvec res = v::transform3(v, t);
        EXPECT_CLOSE(v::extractX(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 2.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 18.0f, epsilon);
        //EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TEST(TransformNormal)
    {
        v::vmtrans t = v::identityT();
        t = v::translateT(t, v::make(1.0f, 2.0f, 3.0f, 4.0f));
        t = v::scaleT(t, v::replicate(5.0f));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(90.0f)));
        v::vmvec n = v::make(0.0f, 1.0f, 0.0f, 0.0f);
        v::vmvec res = v::transformNormal(n, t);
        EXPECT_CLOSE(v::extractX(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 0.0f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), 1.0f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }

    TODO_TEST(Complicated)
    {
        v::vmtrans t = v::identityT();
        t = v::translateT(t, v::make(0.570326f, 25.148458f, -79.989700f, 0.0f));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(0.0f, 0.0f, 1.0f, 0.0f), m::degToRad(0.385849f)));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(0.0f, 1.0f, 0.0f, 0.0f), m::degToRad(-178.677734f)));
        t = v::rotateT(t, v::makeAxisAngleQ(v::make(1.0f, 0.0f, 0.0f, 0.0f), m::degToRad(0.696606f)));
        t = v::scaleT(t, v::replicate(1.0f));
        v::vmvec v = v::make(1.0f, 2.0f, 3.0f, 0.0f);
        v::vmvec res = v::transform3(v, t);
        EXPECT_CLOSE(v::extractX(res), -0.5546949f, epsilon);
        EXPECT_CLOSE(v::extractY(res), 27.6320295f, epsilon);
        EXPECT_CLOSE(v::extractZ(res), -82.9954367f, epsilon);
        EXPECT_CLOSE(v::extractW(res), 0.0f, epsilon);
    }
}
