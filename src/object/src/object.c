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

object car(object o) {
    if (Tcons_cell == o->T)
        return (object)(o->slot_1);
    exit(1);
}

object cdr(object o) {
    if (Tcons_cell == o->T)
        return (object)(o->slot_2);
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
    
}
