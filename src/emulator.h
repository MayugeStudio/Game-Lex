#ifndef EMULATOR_H
#define EMULATOR_H

#include "glex.h"

void GLX_load_bytecode(GLX_vm *gm, const char *file_path);
Err GLEX_execute_inst(GLX_vm *vm);
void GLX_dump_stack(FILE *stream, GLX_vm *gm);

#endif //EMULATOR_H