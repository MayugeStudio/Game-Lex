#include <stdio.h>
#include <stdlib.h>
#include "text.h"
#include "assembler.h"

GLX_vm vm = { 0 };

int main(int argc, char **argv)
{
    const char *program_name = argv[0];
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input.gla> <output.glb>\n", program_name);
        fprintf(stderr, "ERROR: expected input and output\n");
        exit(1);
    }

    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];

    GLX_text source = GLX_load_source_code(input_file_path);

    vm.program_size = (Word)GLX_parse_source(source, vm.program, GLX_PROGRAM_CAPACITY);

    GLX_save_program_as_bytecode(vm.program, vm.program_size, output_file_path);

    return 0;
}
