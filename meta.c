#include <stdio.h>
#include <stdlib.h>

typedef enum {
    number_type,                /* The value can be directly converted to a number. */
    pair_type,                  /* Pointer to another cons */
} object_type;

typedef struct {
    long value;
} number_object;

typedef struct {
    void *car;
    object_type car_type;
    void *cdr;
    object_type cdr_type;
} pair;

void print_pair(pair *p);

void *car(pair *p) {
    return p->car;
}

object_type car_type(pair *p) {
    return p->car_type;
}

void *cdr(pair *p) {
    return p->cdr;
}

object_type cdr_type(pair *p) {
    return p->cdr_type;
}

pair cons(void *car, object_type car_type,
          void *cdr, object_type cdr_type)
{
    pair p = {car, car_type, cdr, cdr_type};
    return p;
}

void print_number(void *object) {
    printf("%d", (int)object);
}

void print_object(void *object, object_type type) {
    switch(type) {
    case number_type:
        print_number(object);
        break;
    case pair_type:
        print_pair((pair *)object);
        break;
    default:
        break;
    }
}

void print_pair(pair *p) {
    printf("(");
    print_object(car(p), car_type(p));
    printf(" ");
    print_object(cdr(p), cdr_type(p));
    printf(")");
}

void print_pair_main(pair *p) {
    print_pair(p);
    printf("\n");
}

int main(void) {
    pair p1 = cons((void *)5, number_type,
                   (void *)10, number_type);
    pair p2 = cons((void *)15, number_type,
                   (void *)&p1, pair_type);
    print_pair_main(&p2);
    
    return 0;
}
