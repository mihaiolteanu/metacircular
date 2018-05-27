#ifndef BASE_COMMON_H
#define BASE_COMMON_H

typedef enum {
    Tcons_cell,
    Tnumber,
    Tquote,
    Tprocedure,
    Tprimitive,
    Tsymbol,
    Tbool,
    Tnull,
} Tobject;

extern Tobject type(object o);
extern bool    is_equal_type(Tobject t1, Tobject t2);
extern object  new_object(Tobject T, void *slot_1, void *slot_2);

#endif
    
