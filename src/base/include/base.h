#ifndef OBJECT_H
#define OBJECT_H
#include <stdbool.h>

/* Types. */
typedef struct object__       *object;
typedef struct environment__  *environment;

/* Globals. */
extern object nil;
extern environment null_environment;

/* Creation and manipulation of cons cells. */
extern void   object_init(void);
extern object cons(object car, object cdr);
extern bool   is_cons(object o);
extern object car (object o);
extern object cdr (object o);

/* Numbers and arithmetic on numbers. */
extern object new_number(int value);
extern bool   is_number(object o);
extern int    number_value(object o);
extern object add_numbers(object o1, object o2);

/* Identifiers and definitions. */
/* Create a new identifier object and assign it the given object in the given environment. */
extern void   define(char *name, object o, environment env);
extern object find(char *name, environment env);
/* Reasign the given identifier to point to a new object in the given environment. */
extern void   redefine(object identifier, object o, environment env);
extern bool   is_identifier(object o);
extern char  *identifier_name(object o);
extern object identifier_object(object identifier);

/* Environments. */
extern environment extend_environment(environment base_env);

/* Functions. */
extern object new_function(char **formal_args, object *body, environment env);

#endif
