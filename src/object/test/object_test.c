#include "unity.h"
#include "object.h"

void test_cons_preserves_car_and_cdr() {
    object n1 = new_number(12);
    object n2 = new_number(23);
    object c = cons(n1, n2);
    object n1_car = car(c);
    object n2_cdr = cdr(c);
    TEST_ASSERT_EQUAL_INT64(n1_car, n1);
    TEST_ASSERT_EQUAL_INT64(n2_cdr, n2);
}

void test_is_cons() {
    object o1 = new_number(1);
    object o2 = new_number(2);
    object c = cons(o1, o2);
    TEST_ASSERT_FALSE(is_cons(o1));
    TEST_ASSERT_FALSE(is_cons(o2));
    TEST_ASSERT_TRUE(is_cons(c));
}

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

void test_number_add_numbers() {
    object n1 = new_number(4);
    object n2 = new_number(6);
    object sum = add_numbers(n1, n2);
    TEST_ASSERT_EQUAL(number_value(sum), 10);
}


void test_identifier() {
    object o = new_identifier("myvar");
    TEST_ASSERT_TRUE(is_identifier(o));
    TEST_ASSERT_EQUAL_STRING(identifier_name(o), "myvar");
}
