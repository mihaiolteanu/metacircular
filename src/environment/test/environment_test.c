#include "unity.h"
#include "environment.h"

void test_extend_the_null_environment(void) {
    environment env = extend_environment(null_environment);
    TEST_ASSERT_NOT_EQUAL(env, null_environment);
}

void test_define_and_find_symbol_object(void) {
    object x = new_number(3);
    environment env = extend_environment(null_environment);
    symbol s = define_symbol("x", x, env);
    object stored_x = symbol_object(s);
    TEST_ASSERT_EQUAL(x, stored_x);
    TEST_ASSERT_EQUAL(3, number_value(stored_x));
}

void test_define_and_find_one_symbol_in_environment(void) {
    object x = new_number(3);
    environment env = extend_environment(null_environment);
    define_symbol("x", x, env);
    symbol stored_s = find_symbol("x", env);
    TEST_ASSERT_NOT_EQUAL(stored_s, null_symbol);
    TEST_ASSERT_EQUAL(3, number_value(symbol_object(stored_s)));
}

void test_define_and_find_multiple_symbols_in_environment(void) {
    object x = new_number(3);
    object y = new_number(5);
    environment env = extend_environment(null_environment);
    define_symbol("x", x, env);
    define_symbol("y", y, env);
    symbol stored_sx = find_symbol("x", env);
    symbol stored_sy = find_symbol("y", env);
    TEST_ASSERT_NOT_EQUAL(stored_sx, null_symbol);
    TEST_ASSERT_NOT_EQUAL(stored_sy, null_symbol);
    TEST_ASSERT_EQUAL(3, number_value(symbol_object(stored_sx)));
    TEST_ASSERT_EQUAL(5, number_value(symbol_object(stored_sy)));
}

void test_define_and_find_multiple_symbols_in_multiple_environments(void) {
    /* Define x in one environment. */
    object x = new_number(3);
    environment env = extend_environment(null_environment);
    define_symbol("x", x, env);
    /* Define y in another environment enclosing the first one. */
    env = extend_environment(env);
    object y = new_number(5);
    define_symbol("y", y, env);

    /* Find symbols. Multiple environments have to be searched to find all of them. */
    symbol stored_sx = find_symbol("x", env);
    symbol stored_sy = find_symbol("y", env);
    TEST_ASSERT_NOT_EQUAL(stored_sx, null_symbol);
    TEST_ASSERT_NOT_EQUAL(stored_sy, null_symbol);
    TEST_ASSERT_EQUAL(3, number_value(symbol_object(stored_sx)));
    TEST_ASSERT_EQUAL(5, number_value(symbol_object(stored_sy)));
} 
