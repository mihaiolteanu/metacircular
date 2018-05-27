#include <string.h>
#include <stdlib.h>
#include "evaluator.h"
#include "base.h"

static bool is_car_name(object expr, char *name) {
    object expr_car;
    char *car_name;
    if (!is_cons(expr))
        return false;
    expr_car = car(expr);
    if (!is_symbol(expr_car))
        return false;
    car_name = symbol_name(expr_car);
    if (0 == strcmp(car_name, name))
        return true;
    return false;
}

static bool is_self_evaluating(object exp) {
    return null_object(exp) || is_number(exp);
}

static bool is_definition_id(object expr) {
    return is_car_name(expr, "define");
}

static bool is_procedure_id(object expr) {
    return is_car_name(expr, "lambda");
}

static bool is_application(object expr) {
    return (is_cons(expr) && (is_symbol(car(expr))));
}

static bool is_if_expression(object exp) {
    return is_car_name(exp, "if");
}

/* Assuming the car of expr is "define", associate the cadr of expr 
 with the caddr object in the given environment*/
static object eval_definition(object expr, environment env) {
    char *id_name;
    if (!is_symbol(cadr(expr)))
        exit(1);                /* Invalid definition (e.g, (define 5 10)) */
    id_name = symbol_name(cadr(expr));
    /* We could have a (define x (+ 2 3)), for example, so the expression after the symbol name also
     needs to be evaluated.*/
    object o = eval(caddr(expr), env);
    define(id_name, o, env);
    return cadr(expr);
}

static object eval_procedure(object expr) {
    /* Pass a list of formal arguments and a list of expressions (list of lists) to the
     * new_procedure function*/
    object formal_args = cadr(expr);
    object body = cddr(expr);
    return new_procedure(formal_args, body);
}

static object application_operator(object app) {
    return car(app);
}

/* Forward declarations. */
object apply(object procedure, object args, environment env);
object eval(object exp, environment env);

/* sybexprs might be something like (2 (- 6 5)), which should evaluate to (2 1)*/
static object eval_subexprs(object exp, environment env) {
    object result;
    if (nil != exp) {
        result = eval(car(exp), env);
        return cons(result, eval_subexprs(cdr(exp), env));
    }
    return nil;
}

static object eval_if_expression(object exp, environment env) {
    object body = cdr(exp);
    if (length(body) != 3)
        exit(1);
    else {
        object condition   = car(body);
        object consequent  = cadr(body);
        object alternative = caddr(body);
        if (is_true(eval(condition, env)))
            return eval(consequent, env);
        return eval(alternative, env);
    }
}

object eval(object exp, environment env) {
    if (is_self_evaluating(exp))
        return exp;
    if (is_quoted(exp)) {
        object body = quote_body(exp);
        /* Quirky input parser; puts an extra cons when cons is quoted */
        if (is_cons(body))
            return car(body);
        return body;
    }
    if (is_definition_id(exp))
        return eval_definition(exp, env);
    if (is_if_expression(exp))
        return eval_if_expression(exp, env);
    if (is_procedure_id(exp)) {
        return eval_procedure(exp);
    }
    if (is_application(exp)) {
        object operator = eval(application_operator(exp), env);
        object subexprs = eval_subexprs(cdr(exp), env);
        return apply(operator, subexprs, env);
    }
    if (is_symbol(exp))
        return find(symbol_name(exp), env);
}

object apply(object procedure, object parameters, environment env) {
    if (is_primitive_procedure(procedure))
        return apply_primitive_procedure(procedure, parameters);
    if (is_compound_procedure(procedure)) {
        object body = body_procedure(procedure);
        object formal_args = formal_args_procedure(procedure);
        environment extended_env = extend_environment(env, formal_args, parameters);
        object result = eval_subexprs(body, extended_env);
        /* Eval all the subexpressions but return the result of the last one */
        return car(last(result));
    }
    else
        exit(1);
}

void multi_eval(object *objs, size_t count, environment env) {
    for(size_t i = 0; i < count; i++) {
        eval(*objs, env);
        objs++;
    }
}
