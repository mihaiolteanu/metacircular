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
} number_;

typedef struct {
    char *str;
} string_; typedef string_ *string;

void *allocate(size_t size) {
    return malloc(size);
}

number new_number(long long value) {
    number_ *number = allocate(sizeof(number_));
    number->value = value;
    return number;
}

string new_string(char *str) {
    size_t str_len = strlen(str);
    string_ *s = allocate(sizeof(string_));
    s->str = allocate(str_len);
    strcpy(s->str, str);
    return s;
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

void print_number(number number) {
    printf("%d", number->value);
}

void print_string(string s) {
    printf("%s", s->str);
}

void print_object(object o) {
    switch(type(o)) {
    case number_type:
        print_number((number)o->pointer);
        break;
    case pair_type:
        print_pair((pair)o->pointer);
        break;
    case string_type:
        print_string((string)o->pointer);
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
    number number5  = new_number( 5);
    string whatever = new_string("whatever");
    number number10 = new_number(10);
    number number15 = new_number(15);
    
    pair p1 = cons(new_object(number5, number_type),
                   new_object(whatever, string_type));
    
    pair p2 = cons(new_object(number15, number_type),
                   new_object(p1, pair_type));
    print_pair_main(p2);
    
    return 0;
}
