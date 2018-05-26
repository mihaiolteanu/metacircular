#ifndef INPUT_H
#define INPUT_H

#include <stdlib.h>
#include "base.h"

/* Creates an object based on expressions of type (+ 3 4) */
extern object parse(char *expr);
extern object *parse_file(char *path, size_t *count);
#endif
