#include <string.h>
#include <stdbool.h>
#include "object.h"
#include "input.h"

/* Stolen from https://stackoverflow.com/questions/31798297/tokenizing-an-s-expression-in-c */
static int lex(char *str, char **start, char **end)
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

static object object_from_token(char *token) {
    object o;
    if (is_number(token))
        o = new_number(atoi(token));
    if (is_identifier(token))
    if (is_string(token))
        o = new_string(token);
    else
        o = new_string(token);
    return o;
}

object parse_expression(char *input) {
    char *lex_string;           /* will contain the whole remaining string */
    char *token;
    object o;
    char *p = input;
    
    lex(p, &lex_string, &p);
    while(lex_string) {
        /* Get the actual token */
        snprintf(token, 100, "%.*s", (int)(p - lex_string), lex_string);
        o = object_from_token(token);
        if (0 == strcmp(token, "("))
            
        lex(p, &lex_string, &p);
    }
    
}
