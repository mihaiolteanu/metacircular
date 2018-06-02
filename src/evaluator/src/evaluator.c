#include <string.h>
#include <stdlib.h>
#include "evaluator.h"
#include "base.h"

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
    return (is_cons(expr) &&
            ((is_symbol(car(expr))) || (is_cons(car(expr)))));
}

static bool is_if_expression(object exp) {
    return is_car_name(exp, "if");
}

static bool is_let_expression(object exp) {
    return is_car_name(exp, "let");
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

static object eval_procedure(object expr, environment env) {
    /* Pass a list of formal arguments and a list of expressions (list of lists) to the
     * new_procedure function*/
    object formal_args = cadr(expr);
    object body = cddr(expr);
    return new_procedure(formal_args, body, env);
}

static object application_operator(object app) {
    return car(app);
}

/* Forward declarations. */
object apply(object procedure, object args);
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

static object eval_let_expression(object exp, environment env) {
    /* Transform a let expression into a lambda and apply it to it's arguments. */
    object o;
    object formal_args_base = cons_empty();
    object formal_args = formal_args_base;
    object args_base = cons_empty();
    object args = args_base;
    object binding;
    object params = cadr(exp);    
    unsigned int params_count = length(params);
    for (unsigned int i = 0; i < params_count; i++) {
        binding = car(params);
        if (is_car_empty(formal_args))
            add_car(formal_args, car(binding));
        else {
            o = cons_empty();
            add_car(o, car(binding));
            add_cdr(formal_args, o);
            formal_args = o;
        }
        if (is_car_empty(args))
            add_car(args, cadr(binding));
        else {
            o = cons_empty();
            add_car(o, cadr(binding));
            add_cdr(args, o);
            args = o;
        }
        params = cdr(params);
    }
    add_cdr(formal_args, nil);
    add_cdr(args, nil);
    object body = cddr(exp);
    object new_proc = new_procedure(formal_args_base, body, env);
    return apply(new_proc, args_base);
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
    if (is_let_expression(exp))
        return eval_let_expression(exp, env);
    if (is_procedure_id(exp)) {
        return eval_procedure(exp, env);
    }
    if (is_application(exp)) {
        object operator = eval(application_operator(exp), env);
        object subexprs = eval_subexprs(cdr(exp), env);
        return apply(operator, subexprs);
    }
    if (is_symbol(exp))
        return find(symbol_name(exp), env);
}

object apply(object procedure, object parameters) {
    if (is_primitive_procedure(procedure))
        return apply_primitive_procedure(procedure, parameters);
    if (is_compound_procedure(procedure)) {
        object body = body_procedure(procedure);
        object formal_args = formal_args_procedure(procedure);
        environment appl_env = env_procedure(procedure);
        environment extended_env = extend_environment(appl_env, formal_args, parameters);
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
