#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

static void append_ch(char **res, char new) {
    **res = new;
    *res = *res + 1;
}

static void _strfy(object exp, char *base, char **res) {
    if (null_object(exp)) {
        /* Remove the extra space, if available, e.g. (2 3) instead of (2 3 ) */
        if (*(*res - 1) == ' ')
            *res = *res - 1;
        append_ch(res, ')');
    }
    else if (is_bool(exp)) {
        append_ch(res, '#');
        if (is_true(exp))
            append_ch(res, 't');
        else
            append_ch(res, 'f');
    }
    else if (is_cons(exp)) {
        if (is_cons(car(exp)))
            append_ch(res, '(');
        _strfy(car(exp), base, res);
        /* Add punctuation between objects. */
        if (is_cons(cdr(exp)) || is_quoted(cdr(exp)))
            append_ch(res, ' ');
        else
            /* Don't add punctuation before closing parens */
            if (!null_object(cdr(exp))) 
                *res += sprintf(*res, "%s", " . ");
        _strfy(cdr(exp), base, res);
    }
    else if (is_quoted(exp)) {
        append_ch(res, '\'');
        _strfy(quote_body(exp), base, res);
    }
    else if (is_procedure(exp)) {
        strcpy(*res, "<procedure> ");
        *res += strlen("<procedure> ");
        _strfy(formal_args_procedure(exp), base, res);
        _strfy(body_procedure(exp), base, res);
    }
    else {
        bool add_closing_parens = false;
        if ((*(*res - 1) == ' ') && (*(*res - 2) == '.'))
            /* Number or symbol is in the cdr position */
            add_closing_parens = true;
        if (is_number(exp))
            *res += sprintf(*res, "%d", number_value(exp));
        else if (is_symbol(exp)) {
            strcpy(*res, symbol_name(exp));
            *res += strlen(symbol_name(exp));
        }
        if (true == add_closing_parens)
            append_ch(res, ')');
    }
}

char *strfy(object exp) {
    char *result = malloc(100);
    char *base = result;
    if (null_object(exp)) {
        *result++ = '(';
        *result++ = ')';
        return base;
    }
    if (is_cons(exp))
        *result++ = '(';
    _strfy(exp, base, &result);
    *result = '\0';
    return base;
}
