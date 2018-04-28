#include "unity.h"
#include "base.h"
#include "input.h"

void test_parse_number(void) {
    object o = parse("5");
    TEST_ASSERT_TRUE(is_number(o));
    TEST_ASSERT_EQUAL(5, number_value(o));
}

void test_parse_single_identifier(void) {
    object o = parse("(myf)");
    TEST_ASSERT_NOT_NULL(o);
    object car_o = car(o);
    object cdr_o = cdr(o);
    TEST_ASSERT_TRUE(is_identifier(car_o));
    TEST_ASSERT_EQUAL_STRING("myf", identifier_name(car_o));
    TEST_ASSERT_TRUE(null_object(cdr_o));
}

void test_parse_multiple_identifier_list(void) {
    object o = parse("(myf 5 10)");
    TEST_ASSERT_NOT_NULL(o);
    object car_o = car(o);
    TEST_ASSERT_TRUE(is_identifier(car_o));
    TEST_ASSERT_EQUAL_STRING("myf", identifier_name(car_o));
    object cadr_o = car(cdr(o));
    TEST_ASSERT_TRUE(is_number(cadr_o));
    TEST_ASSERT_EQUAL(5, number_value(cadr_o));
    object caddr_o = car(cdr(cdr(o)));
    TEST_ASSERT_TRUE(is_number(caddr_o));
    TEST_ASSERT_EQUAL(10, number_value(caddr_o));
    object cdddr_o = cdr(cdr(cdr(o)));
    TEST_ASSERT_TRUE(null_object(cdddr_o));
}

void test_parse_embedded_list(void) {
    object o = parse("((myf) 5 10)");
    TEST_ASSERT_NOT_NULL(o);
    object car_o = car(o);
    object car_car_o = car(car_o);
    TEST_ASSERT_TRUE(is_identifier(car_car_o));
    TEST_ASSERT_EQUAL_STRING("myf", identifier_name(car_car_o));
    object cadr_o = car(cdr(o));
    TEST_ASSERT_TRUE(is_number(cadr_o));
    TEST_ASSERT_EQUAL(5, number_value(cadr_o));
    object caddr_o = car(cdr(cdr(o)));
    TEST_ASSERT_TRUE(is_number(caddr_o));
    TEST_ASSERT_EQUAL(10, number_value(caddr_o));
    object cdddr_o = cdr(cdr(cdr(o)));
    TEST_ASSERT_TRUE(null_object(cdddr_o));
}

void test_parse_embedded_middle_list(void) {
    object o = parse("(5 (myf) 10)");
    TEST_ASSERT_NOT_NULL(o);
    object car_o = car(o);
    TEST_ASSERT_TRUE(is_number(car_o));
    TEST_ASSERT_EQUAL(5, number_value(car_o));
    object caadr = car(car(cdr(o)));
    TEST_ASSERT_TRUE(is_identifier(caadr));
    TEST_ASSERT_EQUAL_STRING("myf", identifier_name(caadr));
    object caddr = car(cdr(cdr(o)));
    TEST_ASSERT_TRUE(is_number(caddr));
    TEST_ASSERT_EQUAL(10, number_value(caddr));
    object cdddr = cdr(cdr(cdr(o)));
    TEST_ASSERT_TRUE(null_object(cdddr));
}
