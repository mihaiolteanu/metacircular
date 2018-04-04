#include <stdbool.h>

typedef struct object__ *object;

extern void   object_init(void);
extern object cons(object car, object cdr);
extern object car (object o);
extern object cdr (object o);

extern object new_number(int value);
extern bool   is_number(object o);
extern int    number_value(object o);
