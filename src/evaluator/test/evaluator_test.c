#include "unity.h"
#include "test_helpers.h"
#include "evaluator.h"
#include "input.h"

void test_eval_definition() {
    object def_expr = parse("(define speed 100)");
    environment env = extend_environment(null_environment, nil, nil);
    eval(def_expr, env);
    object o = find("speed", env);
    number_test(o, 100);
}

void test_eval_lambda() {
    object lmbda = parse("(lambda (x y z) (+ x y z))");
    environment env = extend_environment(null_environment, nil, nil);

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
    object evaled = eval(o, extend_environment(null_environment, nil, nil));
    number_test(evaled, 5);
}

void test_apply_primitive_procedure() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(- 5 3)");
    object res = eval(o, env);
    number_test(res, 2);
    o = parse("(+ 2 5)");
    res = eval(o, env);
    number_test(res, 7);
}

void test_apply_primitive_procedure_nested() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(+ 2 (- 4 3 1))");
    object res = eval(o, env);
    number_test(res, 2);
}

void test_apply_primitive_procedure_to_variables() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object exp = parse("(define speed 100)");
    eval(exp, env);
    object o = parse("(+ speed speed)");
    object res = eval(o, env);
    number_test(res, 200);
}

void test_apply_compound_procedure() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object exp = parse("(define add_one (lambda (x) (+ x 1)))");
    eval(exp, env);
    object o = find("add_one", env);
    object res = parse("(add_one 5)");
    TEST_ASSERT_TRUE(is_compound_procedure(o));
    object evaled = eval(res, env);
    number_test(evaled, 6);
}

void test_evaluate_primitive_cons() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(cons 2 3)");
    object res = eval(o, env);
    number_test(car(res), 2);
    number_test(cdr(res), 3);
    null_object_test(cddr(res));
}

void test_evaluate_primitive_car() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(car (cons 2 3))");
    object res = eval(o, env);
    number_test(res, 2);
}

void test_evaluate_primitive_cdr() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(cdr (cons 2 3))");
    object res = eval(o, env);
    number_test(res, 3);
}

void test_evaluate_quoted() {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("'(2 3)");
    object res = eval(o, env);
    char *str = strfy(res);
    number_test(car(res), 2);
    number_test(cadr(res), 3);
    null_object_test(cddr(res));
}

void test_less_than_primitive(void) {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);
    object o = parse("(< 2 3)");
    object res = eval(o, env);
    TEST_ASSERT_EQUAL(truthhood, res);
    o = parse("(< 10 5)");
    res = eval(o, env);
    TEST_ASSERT_EQUAL(falsehood, res);
}
