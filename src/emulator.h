#ifndef EMULATOR_H
#define EMULATOR_H

#include "glex.h"

void GLX_load_bytecode(Gvm *gvm, const char *file_path);
Err GLX_execute_inst(Gvm *gvm);
Err GLX_execute_program(Gvm *gvm, int limit);
void GLX_dump_stack(FILE *stream, Gvm *gvm);

#endif //EMULATOR_H
