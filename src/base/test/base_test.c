#include "unity.h"
#include "base.h"

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

void test_new_identifier() {
    object id = new_identifier("define");
    TEST_ASSERT_TRUE(is_identifier(id));
    char *id_name = identifier_name(id);
    TEST_ASSERT_EQUAL_STRING("define", id_name);
}

void test_extend_the_null_environment(void) {
    environment env = extend_environment(null_environment);
    TEST_ASSERT_NOT_EQUAL(env, null_environment);
}

void test_define_and_find_one_symbol_in_environment(void) {
    object x = new_number(3);
    environment env = extend_environment(null_environment);
    define("x", x, env);
    object stored_x = find("x", env);
    TEST_ASSERT_NOT_NULL(stored_x);
    TEST_ASSERT_EQUAL(3, number_value(stored_x));
}

void test_define_and_find_multiple_symbols_in_environment(void) {
    object x = new_number(3);
    object y = new_number(5);
    environment env = extend_environment(null_environment);
    define("x", x, env);
    define("y", y, env);
    object stored_x = find("x", env);
    object stored_y = find("y", env);
    TEST_ASSERT_NOT_NULL(stored_x);
    TEST_ASSERT_NOT_NULL(stored_y);
    TEST_ASSERT_EQUAL(3, number_value(stored_x));
    TEST_ASSERT_EQUAL(5, number_value(stored_y));
}

void test_define_and_find_multiple_symbols_in_multiple_environments(void) {
    /* Define x in one environment. */
    object x = new_number(3);
    environment env = extend_environment(null_environment);
    define("x", x, env);
    /* Define y in another environment enclosing the first one. */
    env = extend_environment(env);
    object y = new_number(5);
    define("y", y, env);

    /* Find symbols. Multiple environments have to be searched to find all of them. */
    object stored_x = find("x", env);
    object stored_y = find("y", env);
    TEST_ASSERT_NOT_NULL(stored_x);
    TEST_ASSERT_NOT_NULL(stored_y);
    TEST_ASSERT_EQUAL(3, number_value(stored_x));
    TEST_ASSERT_EQUAL(5, number_value(stored_y));
}
