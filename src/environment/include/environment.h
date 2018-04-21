#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

typedef struct symbol__ *symbol;
typedef struct environment__  *environment;

extern symbol null_symbol;
extern environment null_environment;

#include "object.h"

/* Create a new symbol associated with the given object in the given environment */
extern symbol define_symbol(char *name, object o, environment env);
/* Search for a symbol with name in all the enclosing environments. */
extern symbol find_symbol(char *name, environment env);
extern object symbol_object(symbol s);
/* Create a new environment and make base_env it's enclosing environment */
extern environment extend_environment(environment base_env);

#endif

    
