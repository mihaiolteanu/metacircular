#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "base.h"

typedef enum {
    Tcons_cell,
    Tnumber,
    Tprocedure,
    Tprimitive,
    Tsymbol,
    Tnull,
} Tobject;

static bool is_equal_type(Tobject t1, Tobject t2) {
    return t1 == t2;
}

typedef struct object__ {
    Tobject T;                  /* object type */
    void *slot_1;
    void *slot_2;
} object_;
object nil;

static object new_object(Tobject T, void *slot_1, void *slot_2) {
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
    return (Tcons_cell == o->T);
}

object car(object o) {
    if (is_cons(o))
        return (object)(o->slot_1);
    fprintf(stderr, "Object is not a cons: %s at %d\n", __FILE__, __LINE__ );
    exit(1);
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

environment extend_environment(environment base_env) {
    environment env = malloc(sizeof(environment_));
    env->symbol_list = NULL;
    env->next = base_env;
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

/* §§§ Primitive Procedures. */
typedef object (*proc)(object body);
typedef struct {
    char *name;
    proc  proc;
} primitive_procedure_;
typedef primitive_procedure_ *primitive_procedure;

static char *primitive_procedure_name(object procedure) {
    return ((primitive_procedure)procedure->slot_1)->name;
}

static proc primitive_procedure_proc(object procedure) {
    return ((primitive_procedure)procedure->slot_1)->proc;
}

object new_primitive_procedure(char *name, proc proc) {
    primitive_procedure pp = malloc(sizeof(primitive_procedure_));
    pp->name = name;
    pp->proc = proc;
    return new_object(Tprimitive, (void *)pp, NULL);
}

static object add_numbers(object body) {
    int value = 0;
    while (nil != body) {
        value += number_value(car(body));
        body = cdr(body);
    }
    return new_number(value);
}

static object substract_numbers(object body) {
    int value = 0;
    if (nil != body) {
        value = number_value(car(body));
        body = cdr(body);
        while (nil != body) {
            value -= number_value(car(body));
            body = cdr(body);
        }
    }
    return new_number(value);
}

static primitive_procedure_ primitive_procedures[] = {
    {"+", add_numbers},
    {"-", substract_numbers},
    /* {"*", multiply_numbers}, */
    /* {"/", divide_numbers}, */
};

static unsigned int primitive_procedures_count =
    sizeof(primitive_procedures)/sizeof(primitive_procedures[0]);

static primitive_procedure find_primitive_procedure(object procedure) {
    primitive_procedure primitive;
    char *name = primitive_procedure_name(procedure);
    for (int i=0; i<primitive_procedures_count; i++) {
        primitive = &primitive_procedures[i];
        if (0 == strcmp(primitive->name, name))
            return primitive;
    }
    /* No primitive procedure with that name found, or the object is not a symbol */
    return NULL;
}

void install_primitive_procedures(environment env) {
    primitive_procedure pp;
    for (int i=0; i<primitive_procedures_count; i++) {
        pp = &primitive_procedures[i];
        object ppobject = new_primitive_procedure(pp->name, pp->proc);
        define(pp->name, ppobject, env);
    }
}

bool is_primitive_procedure(object procedure) {
    return is_equal_type(procedure->T, Tprimitive);
}

object apply_primitive_procedure(object procedure, object args) {
    proc p = primitive_procedure_proc(procedure);
    return p(args);
}

/* §§§ Numbers */
typedef struct {
    int value;
} number_;
typedef number_ *number;

object new_number(int value) {
    number n = malloc(sizeof(number_));
    n->value = value;
    return new_object(Tnumber, (void *)n, NULL);
}

bool is_number(object o) {
    return is_equal_type(o->T, Tnumber);
}

int number_value(object o) {
    if (is_number(o))
        return ((number)o->slot_1)->value;
}
    
/* §§§ Nil object */
bool is_object_nil(object o) {
    return o == NULL;
}

/* §§§ Printing representation of conses. */
void _stringify(object exp, char *base, char **res) {
    if (is_object_nil(exp)) {
        /* Remove the extra space, if available, e.g. (2 3) instead of (2 3 ) */
        if (*(*res - 1) == ' ')
            *res = *res - 1;
        **res = ')';
        *res = *res + 1;
    }
    else if (is_cons(exp)) {
        if (is_cons(car(exp))) {
            **res = '(';
            *res = *res + 1;
        }
        _stringify(car(exp), base, res);
        /* Add spaces between objects. */
        **res = ' ';
        *res = *res + 1;
        _stringify(cdr(exp), base, res);
    }
    else if (is_number(exp)) {
        *res += sprintf(*res, "%d", number_value(exp));
    }
    else if (is_symbol(exp)) {
        strcpy(*res, symbol_name(exp));
        *res += strlen(symbol_name(exp));
    }
}

char *stringify(object exp) {
    char *result = malloc(100);
    char *base = result;
    if (is_cons(exp))
        *result++ = '(';
    _stringify(exp, base, &result);
    return base;
}
