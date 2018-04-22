#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "base.h"

typedef enum {
    Tcons_cell,
    Tnumber,
    Tfunction,
    Tidentifier,
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

object cons(object car, object cdr) {
    return new_object(Tcons_cell, (void *)car, (void *)cdr);
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

typedef struct identifier__ {
    char *name;
    object o;
    struct identifier__ *next;  /* Helpful to store multiple identifiers in environments. */
} identifier_;
typedef identifier_ *identifier;

typedef struct environment__ {
    identifier id_list;             /* Each environment has a list of identifiers */
    struct environment__ *next;      /* Enclosing frames for the current environment */
} environment_;
environment null_environment = NULL;

/* Associate a name with an object. */
static identifier new_id(char *name, object o) {
    identifier id = malloc(sizeof(identifier_));
    id->name = malloc(strlen(name));
    strcpy(id->name, name);
    id->o = o;
    id->next = NULL;
    return id;
}

static void store(identifier id, environment env) {
    identifier *id_list = &(env->id_list);
    /* Add the new symbol to the end of the symbols list */
    if (NULL != *id_list) {
        while (NULL != (*id_list)->next)
            id_list = &((*id_list)->next);
        (*id_list)->next = id;
    }
    else
        /* First symbol in the environment */
        *id_list = id;
}

void define(char *name, object o, environment env) {
    identifier id = new_id(name, o);
    store(id, env);
}

object new_identifier(char *name) {
    identifier id = new_id(name, nil);
    return new_object(Tidentifier, (void *)id, NULL);
}

/* Search for a symbol with name in the given list of symbols. */
static identifier find_id_in_frame(char *name, identifier ids) {
    if ((NULL == ids) || (0 == strcmp(name, ids->name)))
        return ids;
    return find_id_in_frame(name, ids->next);
}

object find(char *name, environment env) {
    identifier res;
    if (null_environment == env)
        /* Symbol not found in any of the enclosing environments */
        return nil;
    res = find_id_in_frame(name, env->id_list);
    if (NULL != res)
        /* Symbol found in the current frame */
        return res->o;
    return find(name, env->next);
}

bool is_identifier(object o) {
    return is_equal_type(o->T, Tidentifier);
}

char *identifier_name(object o) {
    if (is_identifier(o))
        return ((identifier)(o->slot_1))->name;
}

object identifier_object(object identifier) {
    return identifier->slot_1;
}

environment extend_environment(environment base_env) {
    environment env = malloc(sizeof(environment_));
    env->id_list = NULL;
    env->next = base_env;
    return env;
}

/* §§§ Functions */
typedef struct {
    char **formal_args;
    object *body;               /* Multiple objects are allowed as a body */
    environment env;
} function_;
typedef function_ *function;

object new_function(char **formal_args, object *body, environment env) {
    function f = malloc(sizeof(function_));
    f->formal_args = formal_args;
    f->body = body;
    f->env = env;
    return new_object(Tfunction, (void *)f, NULL);
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

object add_numbers(object o1, object o2) {
    if (is_number(o1) && is_number(o2)) {
        return new_number(
            number_value(o1) +
            number_value(o2));
    }
    exit(1);
}
    
/* §§§ Nil object */
bool is_object_nil(object o) {
    return o == NULL;
}
