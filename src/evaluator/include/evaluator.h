#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "base.h"

extern object eval(object expr, environment env);
extern void   multi_eval(object *objs, size_t count, environment env);

#endif
