#ifndef OBJECT_H
#define OBJECT_H
#include <stdbool.h>

/* Types. */
typedef struct object__       *object;
typedef struct environment__  *environment;
typedef struct symbol__ *symbol;

extern symbol symbol_list(environment env);
extern symbol next_symbol(symbol s);
extern char  *s_name(symbol s);
extern object s_object(symbol s);
extern environment next_env(environment env);

/* Globals. */
extern object nil;
extern object truthhood;
extern object falsehood;
extern environment null_environment;

/* Creation and manipulation of cons cells. */
extern void   object_init(void);
extern bool   null_object(object o);
extern bool   is_bool(object o);
extern bool   is_true(object o);
extern bool   is_false(object o);
extern object cons(object car, object cdr);
extern object object_from_token(char *token);
extern bool   is_cons(object o);
/* Create a cons cell, but don't assign anything to car nor cdr (i.e. leave it empty) */
extern object cons_empty(void);
extern object empty_quote(void);
extern bool   is_quoted(object o);
extern object quote_body(object quote);
extern bool is_car_empty(object o);
extern bool is_cdr_empty(object o);
extern bool is_car_name(object expr, char *name);
extern object add_car(object empty_cons, object new_car);
extern object add_cdr(object empty_cons, object new_car);
extern object car (object o);
extern object cdr (object o);
extern object cadr (object o);
extern object caddr (object o);
extern object cadddr (object o);
extern object caadr (object o);
extern object cddr (object o);
extern object caar (object o);
extern object cdddr (object o);
extern object last(object o);
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
/* Extend the given environment with a new environment by pairing each formal arg symbol with
the corresponding parameter object. */
extern environment extend_environment(environment base_env, object formal_args, object parameters);

/* Functions. */
extern object new_procedure(object formal_args, object body);
extern bool   is_procedure(object expr);
extern bool   is_primitive_procedure(object procedure);
extern bool   is_compound_procedure(object procedure);
extern object formal_args_procedure(object procedure);
extern object body_procedure(object procedure);
extern object apply_primitive_procedure(object procedure, object args);
extern void   install_primitive_procedures(environment env);

/* Printing representation */
extern char *strfy(object exp);
extern char *env_strfy(environment env);

#endif
