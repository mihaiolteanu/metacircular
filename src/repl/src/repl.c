#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "base.h"
#include "input.h"
#include "evaluator.h"

int main(int argc, char **argv) {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);

    /* Load config file */
    size_t objs_count;
    object *objs = parse_file("/home/mihai/projects/sicp/metacircular_in_c/src/repl/src/config.scm", &objs_count);
    multi_eval(objs, objs_count, env);
    free(objs);

    while (true) {
        char *str = readline ("> ");
        object o = parse(str);
        object res = eval(o, env);
        char *res_str = strfy(res);
        printf("%s\n", res_str);
    }
    return 0;
}
