#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef struct object__ *object;
extern object nil;

extern void   object_init(void);
extern object cons(object car, object cdr);
extern bool   is_cons(object o);
extern object car (object o);
extern object cdr (object o);

extern object new_number(int value);
extern bool   is_number(object o);
extern int    number_value(object o);
extern object add_numbers(object o1, object o2);

#endif
