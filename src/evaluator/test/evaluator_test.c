#include "unity.h"
#include "evaluator.h"

void test_eval_definition() {
    object def = new_identifier("define");
    object speed = new_identifier("speed");
    object kmph = new_number(100);
    /* '(define speed 100) */
    object def_expr = cons(def, cons(speed, cons(kmph, nil)));
    environment env = extend_environment(null_environment);
    eval(def_expr, env);

    object o = find("speed", env);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_EQUAL(100, number_value(o));
}

void test_eval_lambda() {
    object lmbda_id = new_identifier("lambda");
    object x = new_identifier("x");
    object y = new_identifier("y");
    object z = new_identifier("z");
    object plus = new_identifier("+");
    /* (+ x y z)' */
    object body = cons(plus, (cons (x, cons(y, cons(z, nil)))));
    /* (lambda (x y z) (+ x y z)) */
    object lmbda = cons(lmbda_id,
                        cons(cons (x, cons(y, cons(z, nil))), cons(body, nil)));
    environment env = extend_environment(null_environment);

    object evaled = eval(lmbda, env);
    TEST_ASSERT_NOT_NULL(evaled);
    TEST_ASSERT_TRUE(is_lambda(evaled));

    char **formal_args = formal_args_lambda(evaled);
    TEST_ASSERT_EQUAL_STRING("x", formal_args[0]);
    TEST_ASSERT_EQUAL_STRING("y", formal_args[1]);
    TEST_ASSERT_EQUAL_STRING("z", formal_args[2]);

    object *body_expected = body_lambda(evaled);
    TEST_ASSERT_EQUAL(body, body_expected[0]);
}
