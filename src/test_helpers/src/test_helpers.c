#include "unity.h"
#include "base.h"

void number_test(object o, int expected) {
    TEST_ASSERT_TRUE(is_number(o));
    TEST_ASSERT_EQUAL(expected, number_value(o));
}

void identifier_test(object o, char *expected) {
    TEST_ASSERT_TRUE(is_identifier(o));
    TEST_ASSERT_EQUAL_STRING(expected, identifier_name(o));
}

void null_object_test(object o) {
    TEST_ASSERT_TRUE(null_object(o));
}

void not_null_object_test(object o) {
    TEST_ASSERT_FALSE(null_object(o));
}
