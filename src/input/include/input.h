#ifndef INPUT_H
#define INPUT_H

#include "object.h"

/* Creates an object based on expressions of type (+ 3 4) */
extern object parse_expression(char *expr);

#endif
