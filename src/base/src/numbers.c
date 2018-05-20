#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "base_common.h"

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
    return is_equal_type(type(o), Tnumber);
}

int number_value(object o) {
    if (is_number(o))
        return ((number)car(o))->value;
}
