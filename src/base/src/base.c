#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "base.h"
#include "base_common.h"

bool is_equal_type(Tobject t1, Tobject t2) {
    return t1 == t2;
}

typedef struct object__ {
    Tobject T;                  /* object type */
    void *slot_1;
    void *slot_2;
} object_;
object nil;
static object empty_slot;

object new_object(Tobject T, void *slot_1, void *slot_2) {
    object o = malloc(sizeof(object_));
    o->T = T;
    o->slot_1 = slot_1;
    o->slot_2 = slot_2;
    return o;
}

bool null_object(object o) {
    return o == nil;
}

object cons(object car, object cdr) {
    return new_object(Tcons_cell, (void *)car, (void *)cdr);
}

object cons_empty(void) {
    return cons(empty_slot, empty_slot);
}

bool is_car_empty(object o) {
    return ((object)car(o)) == empty_slot;
}

bool is_cdr_empty(object o) {
    return ((object)cdr(o)) == empty_slot;
}

object add_car(object empty_cons, object new_car) {
    empty_cons->slot_1 = new_car;
}

object add_cdr(object empty_cons, object new_cdr) {
    empty_cons->slot_2 = new_cdr;
}

bool number_token_p(char *token) {
    bool result = true;
    for (int i = 0; i < strlen(token); i++) {
        if (!isdigit(token[i]))
            result = false;
    }
    return result;
}

bool string_token_p(char *token) {
    size_t len;
    if ((token[0] == '"') && (token[len-1] == '"'))
        return true;
    return false;
}

bool null_object_token_p(char *token) {
    return (strcmp(token, "nil") == 0);
}

object object_from_token(char *token) {
    object o;
    if (number_token_p(token))
        o = new_number(atoi(token));
    /* else if (string_token_p(token)) */
    /*     o = new_string(token); */
    else if (null_object_token_p(token))
        o = nil;
    else
        o = new_symbol(token);
    return o;
}

bool is_cons(object o) {
    if (nil != o)
        return (Tcons_cell == o->T);
    return false;
}

Tobject type(object o) {
    return o->T;
}

object car(object o) {
    /* if (is_cons(o)) */
        return (object)(o->slot_1);
    /* fprintf(stderr, "Object is not a cons: %s at %d\n", __FILE__, __LINE__ ); */
    /* exit(1); */
}

object cdr(object o) {
    if (is_cons(o))
        return (object)(o->slot_2);
    exit(1);
}

object cadr (object o) {
    return car(cdr(o));
}

object caddr (object o) {
    return car(cdr(cdr(o)));
}

object cadddr (object o) {
    return car(cdr(cdr(cdr(o))));
}

object caadr (object o) {
    return car(car(cdr(o)));
}

object cddr (object o) {
    return cdr(cdr(o));
}

object caar (object o) {
    return car(car(o));
}

object cdddr (object o) {
    return cdr(cdr(cdr(o)));
}

object last(object o) {
    if (null_object(o))
        return nil;
    if (null_object(cdr(o)))
        return o;
    return last(cdr(o));
}

unsigned int length(object list) {
    unsigned int res = 0;
    while (nil != list) {
        res++;
        list = cdr(list);
    }
    return res;
}

typedef struct symbol__ {
    char *name;
    object o;
    struct symbol__ *next;  /* Helpful to store multiple symbols in environments. */
} symbol_;
typedef symbol_ *symbol;

typedef struct {
    object formal_args;
    object body;
    environment env;
} procedure_;
typedef procedure_ *procedure;

typedef struct environment__ {
    symbol symbol_list;             /* Each environment has a list of symbols */
    struct environment__ *next;      /* Enclosing frames for the current environment */
} environment_;
environment null_environment = NULL;

/* Associate a name with an object. */
static symbol new_id(char *name, object o) {
    symbol symbol = malloc(sizeof(symbol_));
    symbol->name = malloc(strlen(name));
    strcpy(symbol->name, name);
    symbol->o = o;
    symbol->next = NULL;
    return symbol;
}

static void store(symbol psymbol, environment env) {
    symbol *symbol_list = &(env->symbol_list);
    /* Add the new symbol to the end of the symbols list */
    if (NULL != *symbol_list) {
        while (NULL != (*symbol_list)->next)
            symbol_list = &((*symbol_list)->next);
        (*symbol_list)->next = psymbol;
    }
    else
        /* First symbol in the environment */
        *symbol_list = psymbol;
}

void define(char *name, object o, environment env) {
    symbol symbol = new_id(name, o);
    if (is_procedure(o))
        /* Attach an environment to lambda making this a procedure definition. */
        ((procedure)(o->slot_1))->env = env;
    store(symbol, env);
}

object new_symbol(char *name) {
    symbol symbol = new_id(name, nil);
    return new_object(Tsymbol, (void *)symbol, NULL);
}

/* Search for a symbol with name in the given list of symbols. */
static symbol find_id_in_frame(char *name, symbol symbols) {
    if ((NULL == symbols) || (0 == strcmp(name, symbols->name)))
        return symbols;
    return find_id_in_frame(name, symbols->next);
}

object find(char *name, environment env) {
    symbol res;
    if (null_environment == env)
        /* Symbol not found in any of the enclosing environments */
        return nil;
    res = find_id_in_frame(name, env->symbol_list);
    if (NULL != res)
        /* Symbol found in the current frame */
        return res->o;
    return find(name, env->next);
}

bool is_symbol(object o) {
    return is_equal_type(o->T, Tsymbol);
}

char *symbol_name(object o) {
    if (is_symbol(o))
        return ((symbol)(o->slot_1))->name;
}

object symbol_object(object symbol) {
    return symbol->slot_1;
}

environment extend_environment(environment base_env, object formal_args, object parameters) {
    /* Create a new environment over the base environment*/
    environment env = malloc(sizeof(environment_));
    env->symbol_list = NULL;
    env->next = base_env;

    if (null_object(formal_args) && null_object(parameters)) 
        /* Only extend the environment, but don't add any new symbols in the new environment. */
        return env;
    if (null_object(formal_args) || null_object(parameters))
        /* Wrong numbers of parameters given */
        exit(1);
    unsigned int len = length(formal_args);
    if (len != length(parameters))
        exit(1);                /* Wrong numbers of parameters given. */
    /* Bind formal args to parameteres in the new environment. */
    object arg, param;
    char *arg_name;
    for(unsigned int i = 0; i < len; i++) {
        arg = car(formal_args);
        param = car(parameters);
        arg_name = symbol_name(arg);
        define(arg_name, param, env);
        formal_args = cdr(formal_args);
        parameters = cdr(parameters);
    }
    return env;
}

/* §§§ Procedures */
object new_procedure(object formal_args, object body) {
    procedure f = malloc(sizeof(procedure_));
    f->formal_args = formal_args;
    f->body = body;
    return new_object(Tprocedure, (void *)f, NULL);
}

bool is_procedure(object expr) {
    return is_equal_type(expr->T, Tprocedure);
}

object formal_args_procedure(object proc) {
    return ((procedure)(proc->slot_1))->formal_args;
}

object body_procedure(object proc) {
    return ((procedure)(proc->slot_1))->body;
}

