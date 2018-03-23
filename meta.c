#include <stdio.h>
#include <stdlib.h>

typedef enum {
    number_type,                /* The value can be directly converted to a number. */
    string_type,
    symbol_type,
    pair_type,                  /* Pointer to another cons */
} object_type;

typedef struct {
    void *pointer;
    object_type type;
} object_;

typedef object_ *object;

typedef struct {
    object car;
    object cdr;
} pair_;

typedef pair_ *pair;

typedef struct {
    int value;
} number_object_;

typedef number_object_ *number_object;

void *allocate(size_t size) {
    return malloc(size);
}

number_object new_number(long long value) {
    number_object_ *number = allocate(sizeof(number_object_));
    number->value = value;
    return number;
}

object new_object(void *pointer, object_type type) {
    object_ *o = allocate(sizeof(object_));
    o->pointer = pointer;
    o->type = type;
    return o;
}

pair cons(object car, object cdr)
{
    pair_ *p = allocate(sizeof(pair_));
    p->car = car;
    p->cdr = cdr;
    return p;
}

void print_pair(pair p);

object car(pair p) {
    return p->car;
}

object cdr(pair p) {
    return p->cdr;
}

object_type type(object o) {
    return o->type;
}

void print_number(number_object number) {
    printf("%d", number->value);
}

void print_object(object o) {
    switch(type(o)) {
    case number_type:
        print_number((number_object)o->pointer);
        break;
    case pair_type:
        print_pair((pair)o->pointer);
        break;
    default:
        break;
    }
}

void print_pair(pair p) {
    printf("(");
    print_object(car(p));
    printf(" ");
    print_object(cdr(p));
    printf(")");
}

void print_pair_main(pair p) {
    print_pair(p);
    printf("\n");
}

int main(void) {
    number_object number5  = new_number( 5);
    number_object number10 = new_number(10);
    number_object number15 = new_number(15);
    
    pair p1 = cons(new_object(number5, number_type),
                   new_object(number10, number_type));
    
    pair p2 = cons(new_object(number15, number_type),
                   new_object(p1, pair_type));
    print_pair_main(p2);
    
    return 0;
}
