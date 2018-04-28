#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "base.h"

char* strrstr(const char *haystack, const char *needle)
{
    char *r = NULL;

    if (!needle[0])
        return (char*)haystack + strlen(haystack);
    while (1) {
        char *p = strstr(haystack, needle);
        if (!p)
            return r;
        r = p;
        haystack = p + 1;
    }
}

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

void extract(char *token, char *start, char *end) {
    snprintf(token, 100, "%.*s", (int)(end - start), start);
}

static object _parse(char *input) {
    object result, this, next;
    char *start;
    char *token = malloc(100);
    char *end = input;
    char *ws = " \t\r\n";
    
    input += strspn(input, ws);
    if (*input == '(') {
        char *p1, *p2, *rest;
        rest = malloc(100);
        p1 = strstr(input, "(");
        if (p1) {
            p2 = strrstr(p1, ")");
            if (p2) {
                snprintf(rest, 100, "%.*s", p2 - p1 - 1, p1 + 1);
                if (strlen(p2) > 1)
                    /* closed parens was not the last element to be parsed */
                    /* String to be parsed is of the form ((myf) 1 2) */
                    result = cons(_parse(rest), _parse(p2 + 1));
                else
                    /* String to be parsed is of the form (myf 1 2) */
                    result = _parse(rest);
                free(rest);
            }
        }
    }
    else {
        lex(end, &start, &end);
        if (start != NULL)
        {
            extract(token, start, end);
            this = object_from_token(token);
            result = cons(this, _parse(end));
            /* next = parse(end); */
            /* if (nil != next) */
            /*     result = cons(this, next); */
            /* else */
            /*     result = this; */
        }
        else
            result = nil;
    }
    free(token);
    return result;
}

object parse(char *input) {
    if (*input != '(')
        /* For expressions like "5" or "myvar" */
        return object_from_token(input);
    /* For expressions with conses, like "(myf 4)" */
    return _parse(input);
}
