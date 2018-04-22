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
