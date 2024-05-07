#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "glex.h"
#include "text.h"

void GLX_save_program_as_bytecode(Inst *program, size_t program_size, const char *file_path);
GLX_text GLX_load_source_code(const char *file_path);
Inst GLX_parse_line(GLX_text line);
size_t GLX_parse_source(GLX_text source, Inst *program, size_t program_capacity);

#endif //ASSEMBLER_H
