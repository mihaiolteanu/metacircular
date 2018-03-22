#include <stdio.h>
#include <stdlib.h>

typedef enum {
    number_type,                /* The value can be directly converted to a number. */
    cons_cell_type,             /* Pointer to another cons */
} object_type;

typedef struct {
    long value;
} number_object;

typedef struct {
    void *car;
    object_type car_type;
    void *cdr;
    object_type cdr_type;
} cons_cell;

void *car(cons_cell *cell) {
    return cell->car;
}

object_type car_type(cons_cell *cell) {
    return cell->car_type;
}

void *cdr(cons_cell *cell) {
    return cell->cdr;
}

object_type cdr_type(cons_cell *cell) {
    return cell->cdr_type;
}

cons_cell cons(void *car, object_type car_type,
               void *cdr, object_type cdr_type)
{
    cons_cell cell = {car, car_type, cdr, cdr_type};
    return cell;
}

void print_number(void *cell) {
    printf("%d", (int)cell);
}

void print_cell(void *cell, object_type type) {
    switch(type) {
    case number_type:
        print_number(cell);
        break;
    case cons_cell_type:
        print_cons(cell);
        break;
    default:
        break;
    }
}

void print_cons(cons_cell *cell) {
    printf("(");
    print_cell(car(cell), car_type(cell));
    printf(" ");
    print_cell(cdr(cell), cdr_type(cell));
    printf(")");
}

void print_cons_main(cons_cell *cell) {
    print_cons(cell);
    printf("\n");
}

int main(void) {
    cons_cell cell = cons((void *)5, number_type,
                          (void *)10, number_type);
    cons_cell cell2 = cons((void *)15, number_type,
                           (void *)&cell, cons_cell_type);
    print_cons_main(&cell2);
    
    return 0;
}
