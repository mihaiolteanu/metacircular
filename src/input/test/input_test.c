#include "unity.h"
#include "base.h"
#include "input.h"

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

void test_parse_number(void) {
    object o = parse("5");
    number_test(o, 5);
}

void test_parse_single_identifier(void) {
    object o = parse("(myf)");
    not_null_object_test(o);
    object car_o = car(o);
    object cdr_o = cdr(o);
    identifier_test(car_o, "myf");
    null_object_test(cdr_o);
}

void test_parse_multiple_identifier_list(void) {
    object o = parse("(myf 5 10)");
    not_null_object_test(o);
    object car_o = car(o);
    identifier_test(car_o, "myf");
    object cadr = car(cdr(o));
    number_test(cadr, 5);
    object caddr = car(cdr(cdr(o)));
    number_test(caddr, 10);
    object cdddr = cdr(cdr(cdr(o)));
    null_object_test(cdddr);
}

void test_parse_embedded_list(void) {
    object o = parse("((myf) 5 10)");
    not_null_object_test(o);
    object car_o = car(o);
    object caar = car(car_o);
    identifier_test(caar, "myf");
    object cadr = car(cdr(o));
    number_test(cadr, 5);
    object caddr = car(cdr(cdr(o)));
    number_test(caddr, 10);
    object cdddr = cdr(cdr(cdr(o)));
    null_object_test(cdddr);
}

void test_parse_embedded_middle_list(void) {
    object o = parse("(5 (myf) 10)");
    not_null_object_test(o);
    object car_o = car(o);
    number_test(car_o, 5);
    object caadr = car(car(cdr(o)));
    identifier_test(caadr, "myf");
    object caddr = car(cdr(cdr(o)));
    number_test(caddr, 10);
    object cdddr = cdr(cdr(cdr(o)));
    null_object_test(cdddr);
}

void test_parse_multiple_embedded_lists(void) {
    object o = parse("(lambda (x) (+ x 1))");
    not_null_object_test(o);
    object car_o = car(o);
    identifier_test(car_o, "lambda");
    object caadr = car(car(cdr(o)));
    identifier_test(caadr, "x");
    object cddr = cdr(cdr(o));
    object caaddr = car(car(cddr));
    identifier_test(caaddr, "+");
    object cadaddr = car(cdr(car(cdr(cdr(o)))));
    identifier_test(cadaddr, "x");
    object caddaddr = car(cdr(cdr(car(cdr(cdr(o))))));
    number_test(caddaddr, 1);
    object cdddaddr = cdr(cdr(cdr(car(cdr(cdr(o))))));
    null_object_test(cdddaddr);
    object cdddr = cdr(cdr(cdr(o)));
    null_object_test(cdddr);
}
