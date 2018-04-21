#include "unity.h"
#include "evaluator.h"

/* void test_eval_definition() { */
/*     /\* construct and eval (define speed 100) *\/ */
/*     object def = new_identifier("define"); */
/*     object speed = new_identifier("speed"); */
/*     object kmph = new_number(100); */
/*     object def_expr = cons(def, cons(speed, cons(kmph, nil))); */
/*     environment env = extend_environment(null_environment); */
/*     eval(def_expr, env); */

/*     symbol s = find_symbol("speed", env); */
/*     TEST_ASSERT_NOT_NULL(s); */
/*     TEST_ASSERT_EQUAL(kmph, symbol_object(s)); */
/* } */
