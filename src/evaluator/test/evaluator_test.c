#include "unity.h"
#include "test_helpers.h"
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

    object formal_args = formal_args_procedure(evaled);
    symbol_test(car(formal_args), "x");
    symbol_test(cadr(formal_args), "y");
    symbol_test(caddr(formal_args), "z");
    
    object body = body_procedure(evaled);
    body = car(body);           /* Only one expression inside procedure */
    symbol_test(car(body), "+");
    symbol_test(cadr(body), "x");
    symbol_test(caddr(body), "y");
    symbol_test(cadddr(body), "z");
}

void test_eval_self_evaluating() {
    object o = parse("5");
    object evaled = eval(o, extend_environment(null_environment));
    TEST_ASSERT_EQUAL(o, evaled);
}

/* void test_apply_primitive_procedure() { */
/*     object o = parse("(+ 2 3)"); */
/*     object res = eval(o, extend_environment(null_environment)); */
/*     number_test(o, 5); */
/* } */
