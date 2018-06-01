#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "base.h"

int lex(char *str, char **start, char **end)
{
    const char *ws = " \t\r\n";
    const char *delim = "() \t\r\n";
    const char *prefix = "()'`";

    str += strspn(str, ws);
    if (str[0] == '\0') {
        *start = *end = NULL;
        return 1;
    }
    *start = str;
    if (strchr(prefix, str[0]) != NULL)
        *end = *start + 1;
    else
        *end = *start + strcspn(str, delim);
    return 0;
}

/* Add a new cons cell to the input object, on it's car slot, if that's empty, or on it's cdr
 * slot, if that's empty. Return the original input if none of the slots are empty */
static object extend(object tail, bool quoted) {
    object o;
    if (quoted)
        o = empty_quote();
    else
        o = cons_empty();
    if (is_car_empty(tail))
        add_car(tail, o);
    else if (is_cdr_empty(tail))
        add_cdr(tail, o);
    return o;
}

static char *ws = " \t\r\n";

/* Remove parens around the input, and return the contents and the remaining string after the
 * parens, e.g. If input is "(+ 2 3) 4", core will be "+ 2 3" and rest will be "4" */
static void remove_parens(char *input, char **core, char **rest) {
    char *p1, *p2;
    int opened = 1;
    *core = malloc(strlen(input)); /* String between the main parens */
    p2 = p1 = input;
    /* Find the closing parens */
    while (opened > 0) {
        p2++;
        if (*p2 == '(')
            opened++;
        else if (*p2 == ')')
            opened--;
    }
    snprintf(*core, strlen(input), "%.*s", p2 - p1 - 1, p1 + 1);
    *rest = p2+1;               /* Return the remaining string after the closing parens */
    
}

void extract(char *token, char *start, char *end) {
    snprintf(token, 100, "%.*s", (int)(end - start), start);
}

static bool dot = false;
static void _parse(object tail, char *input) {
    object o;
    input += strspn(input, ws);
    if (strlen(input) == 0) {
        /* Only add the empty list (nil) is the cons is not dotted */
        if (is_cdr_empty(tail))
            add_cdr(tail, nil);
    }
    else if (*input == '(') {
        char *core, *rest;
        remove_parens(input, &core, &rest);
        o = extend(tail, false);
        _parse(o, core);
        _parse(o, rest);
    }
    else if (*input == '\'') {
        o = extend(tail, true);
        _parse(o, ++input);
    }
    else {
        char *end = input;
        char *start, *token;
        token = malloc(100);
        lex(end, &start, &end);
        if (start != NULL) {
            extract(token, start, end);
            object tko = object_from_token(token);            
            if (dot) {
                add_cdr(tail, tko);
                dot = false;    /* Get ready for the next parsing */
            }
            else if (0 == strcmp(token, ".")) {
                dot = true;
                _parse(tail, ++input);
            }
            else {
                o = extend(tail, false);
                add_car(o, tko);
                _parse(o, end);
            }
        }
        else
            ;
        free(token);
    }
}

object parse(char *start) {
    object tail = cons_empty();
    object head = tail;
    object result;
    if ((*start != '(') && (*start != '\''))
        /* For expressions like "5" or "myvar" */
        return object_from_token(start);
    /* For expressions with conses, like "(myf 4)" */
    _parse(tail, start);
    if (*start == '(') {
        result = car(car(head));
        char *str = strfy(result);
        /* Make possible the definition of functions without using lambda's. */
        if (is_car_name(result, "define") &&
            is_cons(cadr(result))) {
            object fname = caadr(result);
            object fargs = cdr(cadr(result));
            object body = caddr(result);
            /* Transform the definition into a lambda definition */
            result =
                cons(object_from_token("define"),
                     cons(fname,
                          cons(cons(object_from_token("lambda"), cons(fargs, cons(body, nil))),
                               nil)));
        }
    }
    else
        result = car(head);     /* For quoted objects */
    return result;
}

#define MAXS 256
#define MAX_EXP 2000
object *parse_file(char *path, size_t *count) {
    object *objs = malloc(10*sizeof(object));
    object *objs_base = objs;
    object o;
    *count = 0;
    char line[MAXS] = {0};
    char exp[MAX_EXP] = {0};
    char *exp_p = exp;
    FILE *file;
    file = fopen(path, "r");
    while (fgets (line, MAXS, file) != NULL) {
        static int opened = 0;
        char *p = line;
        char *pp;
        size_t len = strlen (line);
        /* Remove newline or carriage return */
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = 0;
        /* Remove redundant spaces */
        while (isspace (*p) && isspace(*(p+1)))
            p++;
        /* skip comments and blank lines */
        if ((*p == ';' && *(p+1) == ';') || !*p)
            continue;

        pp = p;
        while(*pp != '\0') {
            /* printf ("%c ", *pp); */
            if (*pp == '(')
                opened++;
            if (*pp == ')')
                opened--;
            pp++;
        }

        /* Fill the expression */
        snprintf(exp_p, MAX_EXP, "%s", p);
        exp_p += strlen(p);

        if (opened  == 0) {
            o = parse(exp);
            *objs = o;          /* Add to the resulting objexts */
            objs++;
            (*count)++;
            exp_p = exp;        /* Get ready for the next expression. */
        }
    }
    return objs_base;
}
