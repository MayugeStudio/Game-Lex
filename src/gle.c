#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

GLX_vm vm = { 0 };

int main(void)
{
    GLX_load_bytecode(&vm, "./fib.glb");
    for (int i = 0; i < GLX_EXECUTION_LIMIT && !vm.halt; ++i) {
        Err err = GLEX_execute_inst(&vm);
        GLX_dump_stack(stdout, &vm);
        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            exit(1);
        }
    }
    return 0;
}
