#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "base.h"

extern void number_test(object o, int expected);
extern void identifier_test(object o, char *expected);
extern void null_object_test(object o);
extern void not_null_object_test(object o);
extern void test_parse_number(void);

#endif
