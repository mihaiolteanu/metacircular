#include <string.h>
#include <stdlib.h>
#include "environment.h"
#include "object.h"

typedef struct symbol__ {
    char *name;
    object o;
    struct symbol__ *next;      /* List of symbols */
} symbol_;

typedef struct environment__ {
    symbol symbols;             /* Each environment has a list of symbols */
    struct environment__ *next; /* Enclosing frames for the current environment */
} environment_;

symbol null_symbol = NULL;
environment null_environment = NULL;

/* Associate a name with an object. */
static symbol new_symbol(char *name, object o) {
    symbol s = malloc(sizeof(symbol_));
    s->name = malloc(strlen(name));
    strcpy(s->name, name);
    s->o = o;
    s->next = null_symbol;
    return s;
}

static void add_symbol_to_env(symbol s, environment env) {
    symbol *symbols = &(env->symbols);
    /* Add the new symbol to the end of the symbols list */
    if (null_symbol != *symbols) {
        while (null_symbol != (*symbols)->next)
            symbols = &((*symbols)->next);
        (*symbols)->next = s;
    }
    else
        /* First symbol in the environment */
        *symbols = s;
}

/* Search for a symbol with name in the given list of symbols. */
static symbol find_symbol_in_frame(char *name, symbol symbols) {
    if ((null_symbol == symbols) ||
        (0 == strcmp(name, symbols->name)))
        return symbols;
    return find_symbol_in_frame(name, symbols->next);
}

symbol define_symbol(char *name, object o, environment env) {
    symbol s = new_symbol(name, o);
    add_symbol_to_env(s, env);
    return s;
}

symbol find_symbol(char *name, environment env) {
    symbol res;
    if (null_environment == env)
        /* Symbol not found in any of the enclosing environments */
        return null_symbol;
    res = find_symbol_in_frame(name, env->symbols);
    if (null_symbol != res)
        /* Symbol found in the current frame */
        return res;    
    return find_symbol(name, env->next);
}

object symbol_object(symbol s) {
    return s->o;
}

environment extend_environment(environment base_env) {
    environment env = malloc(sizeof(environment_));
    env->symbols = null_symbol;
    env->next = base_env;
    return env;
}
