/*=============================================================================

Copyright (c) 2010 Ville Ruusutie

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
#include "unitos/unitos.h"
#include "mathos/math.h"

namespace m = mathos;
namespace v = mathos::vm;

static const float epsilon = 0.000001f;

class ConvertFixture : public unitos::SuiteTest
{
public:
    virtual ~ConvertFixture() {}
    ConvertFixture()
    {
        avec = (float*)(((uintptr_t)data + 15) & ~15);
        avec[0] = -38.3f;
        avec[1] = 32.43f;
        avec[2] = 577.9f;
        avec[3] = -11129.9f;
        vec[0] = -38.3f;
        vec[1] = 32.43f;
        vec[2] = 577.9f;
        vec[3] = -11129.9f;
    }

protected:
    float padding[2];
    float vec[4];
    float data[8];
    float* avec;
};

TEST_SUITE(VMConvert)
{
    TEST_FIXTURE(ConvertFixture, LoadVector2)
    {
        v::vmvec v = v::load2(vec);
        EXPECT_EQUAL(v::extractX(v), vec[0]);
        EXPECT_EQUAL(v::extractY(v), vec[1]);
        EXPECT_EQUAL(v::extractZ(v), 0.0f);
        EXPECT_EQUAL(v::extractW(v), 0.0f);
    }

    TEST_FIXTURE(ConvertFixture, LoadVector2A)
    {
        v::vmvec v = v::load2A(avec);
        EXPECT_EQUAL(v::extractX(v), avec[0]);
        EXPECT_EQUAL(v::extractY(v), avec[1]);
        EXPECT_EQUAL(v::extractZ(v), 0.0f);
        EXPECT_EQUAL(v::extractW(v), 0.0f);
    }

    TEST_FIXTURE(ConvertFixture, LoadScalarVector3)
    {
        v::vmvec v = v::load3(vec);
        EXPECT_EQUAL(v::extractX(v), vec[0]);
        EXPECT_EQUAL(v::extractY(v), vec[1]);
        EXPECT_EQUAL(v::extractZ(v), vec[2]);
        EXPECT_EQUAL(v::extractW(v), 0.0f);
    }

    TEST_FIXTURE(ConvertFixture, LoadVector3A)
    {
        v::vmvec v = v::load3A(avec);
        EXPECT_EQUAL(v::extractX(v), avec[0]);
        EXPECT_EQUAL(v::extractY(v), avec[1]);
        EXPECT_EQUAL(v::extractZ(v), avec[2]);
        // DONT care: EXPECT_EQUAL(v::extractW(v), 0.0f);
    }

    TEST_FIXTURE(ConvertFixture, LoadVector4)
    {
        v::vmvec v = v::load4(vec);
        EXPECT_EQUAL(v::extractX(v), vec[0]);
        EXPECT_EQUAL(v::extractY(v), vec[1]);
        EXPECT_EQUAL(v::extractZ(v), vec[2]);
        EXPECT_EQUAL(v::extractW(v), vec[3]);
    }

    TEST_FIXTURE(ConvertFixture, LoadVector4A)
    {
        v::vmvec v = v::load4A(avec);
        EXPECT_EQUAL(v::extractX(v), avec[0]);
        EXPECT_EQUAL(v::extractY(v), avec[1]);
        EXPECT_EQUAL(v::extractZ(v), avec[2]);
        EXPECT_EQUAL(v::extractW(v), avec[3]);
    }
/*
    vmmat vmLoadMatrix4A(void const* source);
    vmtrans vmLoadTransformA(void const* source);

    void VMStoreVector2(void* destination, vmVectorFastParam v);
    void VMStoreVector2A(void* destination, vmVectorFastParam v);
    void VMStoreVector3(void* destination, vmVectorFastParam v);
    void VMStoreVector3A(void* destination, vmVectorFastParam v);
    void VMStoreVector4(void* destination, vmVectorFastParam v);
    void VMStoreVector4A(void* destination, vmVectorFastParam v);
    void VMStoreMatrix4A(void* destination, vmMatrixParam m);
    void VMStoreTransformA(void* destination, vmTransformParam t);*/
}
