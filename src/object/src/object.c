#include <stdlib.h>
#include "object.h"

object nil;

typedef struct {
    int value;
} number_;
typedef number_ *number;

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

object new_number(int value) {
    number n = malloc(sizeof(number_));
    n->value = value;
    return new_object(Tnumber, (void *)n, NULL, NULL);
}

void object_init(void) {
    nil->T = Tnull;
    nil->car = NULL;
    nil->cdr = NULL;
}

int main(void) {
    object_init();

    return 0;
}
