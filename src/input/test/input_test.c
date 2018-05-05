#include "base.h"
#include "input.h"
#include "test_helpers.h"

void test_parse_number(void) {
    object o = parse("5");
    number_test(o, 5);
}

void test_parse_single_identifier(void) {
    object o = parse("(myf)");
    not_null_object_test(o);
    identifier_test(car(o), "myf");
    null_object_test(cdr(o));
}

void test_parse_multiple_identifier_list(void) {
    object o = parse("(myf 5 10)");
    not_null_object_test(o);
    identifier_test(car(o), "myf");
    number_test(cadr(o), 5);
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_embedded_list(void) {
    object o = parse("((myf) 5 10)");
    not_null_object_test(o);
    identifier_test(caar(o), "myf");
    number_test(cadr(o), 5);
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_embedded_middle_list(void) {
    object o = parse("(5 (myf) 10)");
    not_null_object_test(o);
    number_test(car(o), 5);
    identifier_test(caadr(o), "myf");
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_multiple_embedded_lists(void) {
    object o = parse("(lambda (x) (+ x 1))");
    not_null_object_test(o);
    identifier_test(car(o), "lambda");
    identifier_test(caadr(o), "x");
    identifier_test(car(caddr(o)), "+");
    identifier_test(car(cdr(car(cdr(cdr(o))))), "x");
    number_test(car(cdr(cdr(car(cdr(cdr(o)))))), 1);
    null_object_test(cdr(cdr(cdr(car(cdr(cdr(o)))))));
    null_object_test(cdddr(o));
}
