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
    if (!is_identifier(expr_car))
        return false;
    car_name = identifier_name(expr_car);
    if (0 == strcmp(car_name, name))
        return true;
    return false;
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
    object expr_cdr = cdr(expr);
    object expr_cddr = cdr(expr_cdr);
    object expr_cadr = car(expr_cdr);
    object expr_caddr = car(expr_cddr);
    char *id_name;
    if (!is_identifier(expr_cadr))
        exit(1);                /* Invalid definition (e.g, (define 5 10)) */
    id_name = identifier_name(expr_cadr);
    define(id_name, expr_caddr, env);
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
        if (!is_identifier(arg))
            exit(1);
        arg_name = identifier_name(arg);
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

object eval(object expr, environment env) {
    if (is_definition_id(expr)) {
        eval_definition(expr, env);
        return nil;
    }
    if (is_lambda_id(expr)) {
        return eval_lambda(expr);
    }
}
