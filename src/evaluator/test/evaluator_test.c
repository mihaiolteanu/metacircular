#include "unity.h"
#include "evaluator.h"
#include "input.h"

void test_eval_definition() {
    object def_expr = parse("(define speed 100)");
    environment env = extend_environment(null_environment);
    eval(def_expr, env);
    object o = find("speed", env);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_EQUAL(100, number_value(o));
}

void test_eval_lambda() {
    object lmbda = parse("(lambda (x y z) (+ x y z))");
    environment env = extend_environment(null_environment);

    object evaled = eval(lmbda, env);
    TEST_ASSERT_NOT_NULL(evaled);
    TEST_ASSERT_TRUE(is_procedure(evaled));

    char **formal_args = formal_args_procedure(evaled);
    TEST_ASSERT_EQUAL_STRING("x", formal_args[0]);
    TEST_ASSERT_EQUAL_STRING("y", formal_args[1]);
    TEST_ASSERT_EQUAL_STRING("z", formal_args[2]);

    object body_expected = body_procedure(evaled)[0];
    object car_body = car(body_expected);
    TEST_ASSERT_TRUE(is_symbol(car_body));
    TEST_ASSERT_EQUAL_STRING("+", symbol_name(car_body));
    object cadr_body = car(cdr(body_expected));
    TEST_ASSERT_TRUE(is_symbol(cadr_body));
    TEST_ASSERT_EQUAL_STRING("x", symbol_name(cadr_body));
    /* TEST_ASSERT_EQUAL(body, body_expected[0]); */
}

void test_eval_self_evaluating() {
    object o = parse("5");
    object evaled = eval(o, extend_environment(null_environment));
    TEST_ASSERT_EQUAL(o, evaled);
}
