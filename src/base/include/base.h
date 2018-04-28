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
extern bool   null_object(object o);
extern object cons(object car, object cdr);
extern object object_from_token(char *token);
extern bool   is_cons(object o);
extern object car (object o);
extern object cdr (object o);
extern unsigned int length(object list);

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
/* Create a new identifier but don't assign it to anything yet. */
extern object new_identifier(char *name);
extern bool   is_identifier(object o);
extern char  *identifier_name(object o);
extern object identifier_object(object identifier);

/* Environments. */
extern environment extend_environment(environment base_env);

/* Functions. */
extern object new_lambda(char **formal_args, object *body);
extern bool   is_lambda(object expr);
extern char **formal_args_lambda(object lambda);
extern object *body_lambda(object lambda);

#endif
