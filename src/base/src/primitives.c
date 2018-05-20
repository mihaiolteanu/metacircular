#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "base_common.h"

/* §§§ Primitive Procedures. */
typedef object (*proc)(object body);
typedef struct {
    char *name;
    proc  proc;
} primitive_procedure_;
typedef primitive_procedure_ *primitive_procedure;

static char *primitive_procedure_name(object procedure) {
    return ((primitive_procedure)car(procedure))->name;
}

static proc primitive_procedure_proc(object procedure) {
    return ((primitive_procedure)car(procedure))->proc;
}

object new_primitive_procedure(char *name, proc proc) {
    primitive_procedure pp = malloc(sizeof(primitive_procedure_));
    pp->name = name;
    pp->proc = proc;
    return new_object(Tprimitive, (void *)pp, NULL);
}

static object add_numbers(object body) {
    int value = 0;
    while (nil != body) {
        value += number_value(car(body));
        body = cdr(body);
    }
    return new_number(value);
}

static object substract_numbers(object body) {
    int value = 0;
    if (nil != body) {
        value = number_value(car(body));
        body = cdr(body);
        while (nil != body) {
            value -= number_value(car(body));
            body = cdr(body);
        }
    }
    return new_number(value);
}

static object cons_primitive(object body) {
    unsigned int len = length(body);
    if (len < 2) {
        exit(1);
    } else {
        return body;
    }
}

static object car_primitive(object body) {
    if (is_cons(body))
        /* Primitive procedures receive a list of arguments on which to operate; car, operating
           on lists, receives a list of it's argument, that is ((2 3)), for example. */
        return car(car(body));
    exit(1);
}

static object cdr_primitive(object body) {
    if (is_cons(body))
        return cadr(car(body));
    exit(1);
}

static primitive_procedure_ primitive_procedures[] = {
    {"+", add_numbers},
    {"-", substract_numbers},
    /* {"*", multiply_numbers}, */
    /* {"/", divide_numbers}, */
    {"cons", cons_primitive},
    {"car", car_primitive},
    {"cdr", cdr_primitive},
};

static unsigned int primitive_procedures_count =
    sizeof(primitive_procedures)/sizeof(primitive_procedures[0]);

static primitive_procedure find_primitive_procedure(object procedure) {
    primitive_procedure primitive;
    char *name = primitive_procedure_name(procedure);
    for (int i=0; i<primitive_procedures_count; i++) {
        primitive = &primitive_procedures[i];
        if (0 == strcmp(primitive->name, name))
            return primitive;
    }
    /* No primitive procedure with that name found, or the object is not a symbol */
    return NULL;
}

void install_primitive_procedures(environment env) {
    primitive_procedure pp;
    for (int i=0; i<primitive_procedures_count; i++) {
        pp = &primitive_procedures[i];
        object ppobject = new_primitive_procedure(pp->name, pp->proc);
        define(pp->name, ppobject, env);
    }
}

bool is_primitive_procedure(object procedure) {
    return is_equal_type(type(procedure), Tprimitive);
}

bool is_compound_procedure(object procedure) {
    return is_equal_type(type(procedure), Tprocedure);
}

object apply_primitive_procedure(object procedure, object args) {
    proc p = primitive_procedure_proc(procedure);
    return p(args);
}
