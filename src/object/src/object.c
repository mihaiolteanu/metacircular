#include <stdlib.h>
#include <stdbool.h>
#include "object.h"

typedef enum {
    Tcons_cell,
    Tnumber,
    Tnull,
} Tobject;

static bool is_equal_type(Tobject t1, Tobject t2) {
    return t1 == t2;
}

typedef struct object__ {
    Tobject T;                  /* object type */
    void *value;
    struct object__ *car;
    struct object__ *cdr;
} object_;
object nil;

static object new_object(Tobject T, void *value, object car, object cdr) {
    object o = malloc(sizeof(object_));
    o->T = T;
    o->value = value;
    o->car = car;
    o->cdr = cdr;
    return o;
}

object cons(object car, object cdr) {
    return new_object(Tcons_cell, NULL, car, cdr);
}

object car(object o) {
    if (Tcons_cell == o->T)
        return o->car;
    exit(1);
}

object cdr(object o) {
    if (Tcons_cell == o->T)
        return o->cdr;
    exit(1);
}

/* §§§ Numbers */
typedef struct {
    int value;
} number_;
typedef number_ *number;

object new_number(int value) {
    number n = malloc(sizeof(number_));
    n->value = value;
    return new_object(Tnumber, (void *)n, NULL, NULL);
}

bool is_number(object o) {
    return is_equal_type(o->T, Tnumber);
}

int number_value(object o) {
    if (is_number(o))
        return ((number)o->value)->value;
}

void object_init(void) {
    nil->T = Tnull;
    nil->car = NULL;
    nil->cdr = NULL;
}
