#include "unity.h"
#include "test_helpers.h"
#include "base.h"
#include "input.h"

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

void test_create_empty_cons() {
    object o = cons_empty();
    TEST_ASSERT_TRUE(is_car_empty(o));
    TEST_ASSERT_TRUE(is_cdr_empty(o));
}

void test_add_to_empty_cons() {
    object o = cons_empty();
    add_car(o, new_number(5));
    add_cdr(o, new_number(10));
    number_test(car(o), 5);
    number_test(cdr(o), 10);
}

void test_last() {
    object o = parse("(1 2 3 4 5)");
    object l = last(o);
    number_test(car(l), 5);
}

void test_list_length() {
    object o = parse("(1 2 3 4 5)");
    TEST_ASSERT_EQUAL(5, length(o));
}

void test_number_object_create() {
    int value = 123;
    object o = new_number(value);
    number_test(o, 123);
}

void test_number_identification() {
    int ignore_value = 123;
    object o = new_number(ignore_value);
    TEST_ASSERT_TRUE(is_number(o));
}

void test_new_symbol() {
    object id = new_symbol("define");
    symbol_test(id, "define");
}

void test_extend_the_null_environment(void) {
    environment env = extend_environment(null_environment, nil, nil);
    TEST_ASSERT_NOT_EQUAL(env, null_environment);
}

void test_define_and_find_one_symbol_in_environment(void) {
    object x = new_number(3);
    environment env = extend_environment(null_environment, nil, nil);
    define("x", x, env);
    number_test(find("x", env), 3);
}

void test_define_and_find_multiple_symbols_in_environment(void) {
    object x = new_number(3);
    object y = new_number(5);
    environment env = extend_environment(null_environment, nil, nil);
    define("x", x, env);
    define("y", y, env);
    number_test(find("x", env), 3);
    number_test(find("y", env), 5);
}

void test_define_and_find_multiple_symbols_in_multiple_environments(void) {
    /* Define x in one environment. */
    object x = new_number(3);
    environment env = extend_environment(null_environment, nil, nil);
    define("x", x, env);
    /* Define y in another environment enclosing the first one. */
    env = extend_environment(env, nil, nil);
    object y = new_number(5);
    define("y", y, env);
    /* Find symbols. Multiple environments have to be searched to find all of them. */
    number_test(find("x", env), 3);
    number_test(find("y", env), 5);
}

void test_extend_environment_with_args(void) {
    environment env = extend_environment(null_environment, nil, nil);
    define("x", new_number(1), env);
    object formal_args = cons(new_symbol("y"), cons(new_symbol("z"), nil));
    object parameters  = cons(new_number(2), cons(new_number(3), nil));
    environment ext_env = extend_environment(env, formal_args, parameters);
    object x = find("x", ext_env);
    object y = find("y", ext_env);
    object z = find("z", ext_env);
    number_test(x, 1);
    number_test(y, 2);
    number_test(z, 3);
}

void test_printing_representation_basic(void) {
    object x = new_number(3);
    char *str = strfy(x);
    TEST_ASSERT_EQUAL_STRING("3", str);
}

void test_printing_representation_cons_cell(void) {
    object o = parse("(plus 3 4)");
    TEST_ASSERT_EQUAL_STRING("(plus 3 4)", strfy(o));
}

void test_printing_representation_dot_notation(void) {
    object o = parse("(3 . 4)");
    TEST_ASSERT_EQUAL_STRING("(3 . 4)", strfy(o));
}

void test_printing_representation_embedded_conses(void) {
    object o = parse("(plus 3 (plus 4 5))");
    TEST_ASSERT_EQUAL_STRING("(plus 3 (plus 4 5))", strfy(o));
}
