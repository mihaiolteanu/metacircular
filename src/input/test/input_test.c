#include "base.h"
#include "input.h"
#include "test_helpers.h"

void test_parse_number(void) {
    object o = parse("5");
    number_test(o, 5);
    o = parse("my_variable");
    symbol_test(o, "my_variable");
}

void test_parse_single_symbol(void) {
    object o = parse("(myf)");
    not_null_object_test(o);
    symbol_test(car(o), "myf");
    null_object_test(cdr(o));
}

void test_parse_multiple_symbol_list(void) {
    object o = parse("(myf 5 10)");
    not_null_object_test(o);
    symbol_test(car(o), "myf");
    number_test(cadr(o), 5);
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_embedded_list(void) {
    object o = parse("((myf) 5 10)");
    not_null_object_test(o);
    symbol_test(caar(o), "myf");
    number_test(cadr(o), 5);
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_embedded_middle_list(void) {
    object o = parse("(5 (myf) 10)");
    not_null_object_test(o);
    number_test(car(o), 5);
    symbol_test(caadr(o), "myf");
    number_test(caddr(o), 10);
    null_object_test(cdddr(o));
}

void test_parse_multiple_embedded_lists(void) {
    object o = parse("(lambda (x) (+ x 1))");
    not_null_object_test(o);
    symbol_test(car(o), "lambda");
    symbol_test(caadr(o), "x");
    symbol_test(car(caddr(o)), "+");
    symbol_test(car(cdr(car(cdr(cdr(o))))), "x");
    number_test(car(cdr(cdr(car(cdr(cdr(o)))))), 1);
    null_object_test(cdr(cdr(cdr(car(cdr(cdr(o)))))));
    null_object_test(cdddr(o));
}

void test_mytest(void) {
    object o = parse("(+ 2 (- 4 3) 1)");
    not_null_object_test(o);
    symbol_test(car(o), "+");
    number_test(cadr(o), 2);
    object exp = caddr(o);       /* (- 4 3) */
    symbol_test(car(exp), "-");
    number_test(cadr(exp), 4);
    number_test(caddr(exp), 3);
}

void test_dot_notation(void) {
    object o = parse("(3 . 4)");
    number_test(car(o), 3);
    number_test(cdr(o), 4);
}

void test_multiple_sublists(void) {
    object o = parse("(myf ((3 4) 5))");
    symbol_test(car(o), "myf");
    object rest = cadr(o);    /* ((3 4) 5) */
    number_test(caar(rest), 3);
    number_test(cadr(car(rest)), 4);
    number_test(cadr(rest), 5);
}

void test_quoted(void) {
    object o = parse("(cons 3 '(4 5))");
    char *str = strfy(o);
    symbol_test(car(o), "cons");
}
