#include "unity.h"
#include "object.h"

void test_number_object_create() {
    int value = 123;
    object o = new_number(value);
    TEST_ASSERT_EQUAL(number_value(o), value);
}

void test_number_identification() {
    int ignore_value = 123;
    object o = new_number(ignore_value);
    TEST_ASSERT_TRUE(is_number(o));
}


