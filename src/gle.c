#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulator.h"
#include "shift_arg.h"

Gvm vm = { 0 };

void usage(FILE *stream, const char *program)
{
    fprintf(stream, "Usage: %s -i <input.glb>\n", program);
}

int main(int argc, char **argv)
{
    const char *program = shift_arg(&argc, &argv);
    char *input_file_path = NULL;

    while (argc > 0) {
        char *flag = shift_arg(&argc, &argv);

        if (strcmp(flag, "-i") == 0) {
            if (argc == 0) {
                usage(stderr, program);
                fprintf(stderr, "ERROR: No argument is provided for flag `%s`\n", flag);
                exit(1);
            }
            input_file_path = shift_arg(&argc, &argv);
        } else {
            usage(stderr, program);
            fprintf(stderr, "ERROR: Unknown flag `%s`\n", flag);
            exit(1);
        }
    }

    if (input_file_path == NULL) {
        usage(stderr, program);
        fprintf(stderr, "ERROR: Input is not provided\n");
        exit(1);
    }

    GLX_load_bytecode(&vm, input_file_path);
    Err err = GLX_execute_program(&vm, GLX_EXECUTION_LIMIT);
    if (err != ERR_OK) {
        usage(stderr, program);
        fprintf(stderr, "ERROR: %s", err_as_cstr(err));
        exit(1);
    }
    GLX_dump_stack(stdout, &vm);
    return 0;
}
