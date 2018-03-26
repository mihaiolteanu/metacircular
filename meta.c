#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    number_type,                /* The value can be directly converted to a number. */
    string_type,
    symbol_type,
    pair_type,                  /* Pointer to another cons */
    null_type,                  /* Used by nil object */
} object_type;

typedef struct {
    void *pointer;
    object_type type;
} object_; typedef object_ *object;

typedef struct {
    object car;
    object cdr;
} pair_; typedef pair_ *pair;

typedef struct {
    int value;
} number_; typedef number_ *number;

typedef struct {
    char *str;
} string_; typedef string_ *string;

object nil;
void print_object(object o);

void print_and_exit(char *str, object o) {
    printf("%s\n", str);
    /* print_object(o); */
    exit(1);
}

void *allocate(size_t size) {
    return malloc(size);
}

object new_object(void *pointer, object_type type) {
    object_ *o = allocate(sizeof(object_));
    o->pointer = pointer;
    o->type = type;
    return o;
}

object new_number(long long value) {
    number_ *number = allocate(sizeof(number_));
    number->value = value;
    return new_object(number, number_type);
}

object new_string(char *str) {
    size_t str_len = strlen(str);
    string_ *s = allocate(sizeof(string_));
    s->str = allocate(str_len);
    strcpy(s->str, str);
    return new_object(s, string_type);
}

object cons(object car, object cdr)
{
    pair_ *p = allocate(sizeof(pair_));
    p->car = car;
    p->cdr = cdr;
    return new_object(p, pair_type);
}

object_type type(object o) {
    return o->type;
}

object car(object o) {
    if (pair_type == type(o))
        return ((pair)o->pointer)->car;
    else
        print_and_exit("object is not of type pair", o);
}

object cdr(object o) {
    if (pair_type == type(o))
        return ((pair)o->pointer)->cdr;
    else
        print_and_exit("object is not of type pair", o);
}

void print_number(number number) {
    printf("%d", number->value);
}

void print_string(string s) {
    printf("%s", s->str);
}

/* shitty name; _low stands for the last function in the chain of printing object functions */
void print_object_low(object o) {
    switch(type(o)) {
    case number_type:
        print_number((number)o->pointer);
        break;
    case pair_type:
        print_object(o);
        break;
    case string_type:
        print_string((string)o->pointer);
        break;
    default:
        break;
    }
}

void print_object(object o) {
    if (pair_type == type(o)) {
        printf("(");
        print_object_low(car(o));
        printf(" ");
        print_object_low(cdr(o));
        printf(")");
    }
    else
        print_object_low(o);
}

void print_object_main(object o) {
    print_object(o);
    printf("\n");
}

int main(void) {
    /* Init the nil object */
    nil = new_object(NULL, null_type);
    object number5  = new_number( 5);
    object whatever = new_string("whatever");
    object number10 = new_number(10);
    object number15 = new_number(15);
    
    object o1 = cons(number5, whatever);
    object o2 = cons(number15, o1);

    print_object_main(o2);
    
    return 0;
}
