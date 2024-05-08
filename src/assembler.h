#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "glex.h"
#include "text.h"

void GLX_save_program_as_bytecode(Inst *program, size_t program_size, const char *file_path);
GLX_text GLX_load_source_code(const char *file_path);
void GLX_parse_source_into_instructions(GLX_vm *vm, Gasm *gasm, GLX_text source);

#endif //ASSEMBLER_H
