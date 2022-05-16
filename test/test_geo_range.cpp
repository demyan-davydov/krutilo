#include <mbed.h>
#include <unity.h>
#include "geo/gc_range.h"

void test_ctor_empty(void) 
{
    const auto r = GCRange::createEmpty(0);

    TEST_ASSERT_EQUAL(r.a, 0);
    TEST_ASSERT_EQUAL(r.b, -1);
    TEST_ASSERT_EQUAL(r.size(), 0);
    TEST_ASSERT_EQUAL(r.sizeAbs(), 0);
    TEST_ASSERT_EQUAL(r.contain(r.a), false);
    TEST_ASSERT_EQUAL(r.contain(r.b), false);

    TEST_ASSERT_EQUAL(r.empty(), true);
    TEST_ASSERT_EQUAL(r.positive(), false);

    TEST_ASSERT_EQUAL(r.valid(), true);
    TEST_ASSERT_EQUAL(r == r.validCollapsed(), true);
    TEST_ASSERT_EQUAL(r == r.validSwaped(), true);

    auto rvc = r; rvc.validateCollapse();
    auto rvs = r; rvs.validateSwap();
    TEST_ASSERT_EQUAL(r == rvc, true);
    TEST_ASSERT_EQUAL(r == rvs, true);
}

void test_ctor_point(void) 
{
    auto r = GCRange(0);

    TEST_ASSERT_EQUAL(r.a, 0);
    TEST_ASSERT_EQUAL(r.b, 0);
    TEST_ASSERT_EQUAL(r.a, r.b);
    TEST_ASSERT_EQUAL(r.size(), 1);
    TEST_ASSERT_EQUAL(r.sizeAbs(), 1);
    TEST_ASSERT_EQUAL(r.contain(r.a), true);
    TEST_ASSERT_EQUAL(r.contain(r.b), true);
    
    TEST_ASSERT_EQUAL(r.valid(), true);
    TEST_ASSERT_EQUAL(r.empty(), false);
    TEST_ASSERT_EQUAL(r.positive(), true);
}

void test_modify(void) 
{
    const auto r1 = GCRange(-5, 5);

    auto r2 = r1;
    r2.moveATo(42);
    TEST_ASSERT_EQUAL(r2.a, 42);
    TEST_ASSERT_EQUAL(r2.size(), r1.size());

    r2.moveBTo(42);
    TEST_ASSERT_EQUAL(r2.b, 42);
    TEST_ASSERT_EQUAL(r2.size(), r1.size());

    r2.moveCenterTo(42);
    TEST_ASSERT_EQUAL(r2.center(), 42);
    TEST_ASSERT_EQUAL(r2.size(), r1.size());
}

void test_operators(void) 
{
    auto r1 = GCRange(0);
    auto r2 = r1;
    GCRange r3{5};

    r3 = r2;

    TEST_ASSERT_EQUAL(r1 == r2, true);
    TEST_ASSERT_EQUAL(r1 != r2, false);
    TEST_ASSERT_EQUAL(r1 == r3, true);
}


int main() 
{
    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_ctor_empty);
    RUN_TEST(test_ctor_point);
    RUN_TEST(test_modify);
    RUN_TEST(test_operators);
    UNITY_END(); // stop unit testing

    while(1)
    {
        wait_ms(1000);
    }
}
