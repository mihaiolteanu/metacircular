#include <string.h>
#include <stdlib.h>
#include "evaluator.h"
#include "base.h"

static bool is_definition(object expr) {
    object expr_car;
    char *id_name;
    if (!is_cons(expr))
        return false;
    expr_car = car(expr);
    if (!is_identifier(expr_car))
        return false;
    id_name = identifier_name(expr_car);
    if (0 == strcmp(id_name, "define"))
        return true;
    return false;
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

void eval(object expr, environment env) {
    if (is_definition(expr))
        eval_definition(expr, env);
}
