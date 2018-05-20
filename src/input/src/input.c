#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
static object extend(object tail) {
    object o = cons_empty();
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
        o = extend(tail);
        _parse(o, core);
        _parse(o, rest);
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
                o = extend(tail);
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
    if (*start != '(')
        /* For expressions like "5" or "myvar" */
        return object_from_token(start);
    /* For expressions with conses, like "(myf 4)" */
    char *end = start + strlen(start) - 1;
    _parse(tail, start);
    return car(car(head));
}
