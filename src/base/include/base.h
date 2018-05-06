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
extern object cadr (object o);
extern object caddr (object o);
extern object cadddr (object o);
extern object caadr (object o);
extern object cddr (object o);
extern object caar (object o);
extern object cdddr (object o);
extern unsigned int length(object list);

/* Numbers and arithmetic on numbers. */
extern object new_number(int value);
extern bool   is_number(object o);
extern int    number_value(object o);

/* Symbols and definitions. */
/* Create a new symbol object and assign it the given object in the given environment. */
extern void   define(char *name, object o, environment env);
extern object find(char *name, environment env);
/* Reasign the given symbol to point to a new object in the given environment. */
extern void   redefine(object symbol, object o, environment env);
/* Create a new symbol but don't assign it to anything yet. */
extern object new_symbol(char *name);
extern bool   is_symbol(object o);
extern char  *symbol_name(object o);
extern object symbol_object(object symbol);

/* Environments. */
extern environment extend_environment(environment base_env);

/* Functions. */
extern object new_procedure(object formal_args, object body);
extern bool   is_procedure(object expr);
extern bool   is_primitive_procedure(object procedure);
extern object formal_args_procedure(object procedure);
extern object body_procedure(object procedure);
extern object apply_primitive_procedure(object procedure, object args);
extern void   install_primitive_procedures(environment env);

#endif
