typedef enum {
    Tcons_cell,
    Tnumber,
    Tnull,
} Tobject;

typedef struct object__ {
    Tobject T;                  /* object type */
    void *value;
    struct object__ *car;
    struct object__ *cdr;
} object_;
typedef object_ *object;

extern object cons(object car, object cdr);
extern object car(object o);
extern object cdr(object o);
extern object new_number(int value);
extern void object_init(void);
