//
// Created by eyebrow on 2024/05/07.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

void GLX_save_program_as_bytecode(Inst *program, size_t program_size, const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    fwrite(program, sizeof(program[0]), program_size, f);

    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    fclose(f);
}

GLX_text GLX_load_source_code(const char *file_path)
{
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    long size = ftell(f);

    if (size < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    char *buffer = malloc(size);

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }


    size_t n = fread(buffer, sizeof(char), size, f);

    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    fclose(f);

    return (GLX_text){
        .count = n,
        .data = buffer,
    };
}

Inst GLX_parse_line(GLX_text line)
{
    line = text_trim_left(line);
    GLX_text inst_name = text_chop_by_delim(&line, ' ');
    if (text_eq(inst_name, text_cstr_as_text("push"))) {
        line = text_trim(line);
        int operand = text_to_int(line);
        return (Inst){ .type = INST_PUSH, .operand = operand };
    }
    else if (text_eq(inst_name, text_cstr_as_text("dup"))) {
        line = text_trim(line);
        int operand = text_to_int(line);
        return (Inst){ .type = INST_DUP, .operand = operand };
    }
    else if (text_eq(inst_name, text_cstr_as_text("plus"))) {
        return (Inst){ .type = INST_PLUS };
    }
    else if (text_eq(inst_name, text_cstr_as_text("jmp"))) {
        line = text_trim(line);
        int operand = text_to_int(line);
        return (Inst){ .type = INST_JMP, .operand = operand };
    }
    else if (text_eq(inst_name, text_cstr_as_text("halt"))) {
        return (Inst){ .type = INST_HALT };
    }
    else {
        fprintf(stderr, "ERROR: unknown instruction `%.*s`\n", (int)inst_name.count, inst_name.data);
        exit(1);
    }
}

size_t GLX_parse_source(GLX_text source, Inst *program, size_t program_capacity)
{
    size_t program_size = 0;
    while (source.count > 0) {
        assert(program_size < program_capacity);
        GLX_text line = text_trim(text_chop_by_delim(&source, '\n'));
        if (line.count > 0) {
            program[program_size] = GLX_parse_line(line);
            program_size += 1;
        }
    }
    return program_size;
}
