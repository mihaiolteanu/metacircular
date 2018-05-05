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
    return is_number(exp);
}

static bool is_definition_id(object expr) {
    return is_car_name(expr, "define");
}

static bool is_lambda_id(object expr) {
    return is_car_name(expr, "lambda");
}

/* Assuming the car of expr is "define", associate the cadr of expr 
 with the caddr object in the given environment*/
static void eval_definition(object expr, environment env) {
    char *id_name;
    if (!is_symbol(cadr(expr)))
        exit(1);                /* Invalid definition (e.g, (define 5 10)) */
    id_name = symbol_name(cadr(expr));
    define(id_name, caddr(expr), env);
}

static object eval_lambda(object expr) {
    object arg;                 /* One argument from the list of arguments */
    char *arg_name;
    object args = car(cdr(expr)); /* arguments part of the lambda expression */
    unsigned int args_count = length(args);
    char **formal_args = malloc(args_count * sizeof(char **));
    char **formal_args_base = formal_args;
    while (nil != args) {
        arg = car(args);
        if (!is_symbol(arg))
            exit(1);
        arg_name = symbol_name(arg);
        *formal_args = arg_name;
        formal_args++;
        args = cdr(args);
    }
    unsigned int body_count = length(expr) - 2;
    object *body = malloc(body_count * sizeof(object));
    object *body_base = body;
    object body_entries = cdr(cdr(expr)); /* First body expression */
    while (nil != body_entries) {
        *body = car(body_entries);
        body++;
        body_entries = cdr(body_entries);
    }
    return new_lambda(formal_args_base, body_base);
}

object eval(object exp, environment env) {
    if (is_self_evaluating(exp))
        return exp;
    if (is_definition_id(exp)) {
        eval_definition(exp, env);
        return nil;
    }
    if (is_lambda_id(exp)) {
        return eval_lambda(exp);
    }
}
