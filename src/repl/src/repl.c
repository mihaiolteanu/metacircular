#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "base.h"
#include "input.h"
#include "evaluator.h"

int main(int argc, char **argv) {
    environment env = extend_environment(null_environment, nil, nil);
    install_primitive_procedures(env);

    while (true) {
        char *str = readline ("> ");
        object o = parse(str);
        object res = eval(o, env);
        char *res_str = strfy(res);
        printf("%s\n", res_str);
    }
    return 0;
}
