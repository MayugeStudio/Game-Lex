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

    return (GLX_text) {
        .count = n,
        .data = buffer,
    };
}

void GLX_parse_source_into_instructions(GLX_vm *vm, Gasm *gasm, GLX_text source)
{
    vm->program_size = 0;
    while (source.count > 0) {
        assert(vm->program_size < GLX_PROGRAM_CAPACITY);
        GLX_text line = text_trim(text_chop_by_delim(&source, '\n'));

        if (line.count > 0 && *line.data != '#') {
            GLX_text inst_name = text_chop_by_delim(&line, ' ');
            GLX_text operand = text_trim(text_chop_by_delim(&line, '#'));

            if (inst_name.count > 0 && inst_name.data[inst_name.count - 1] == ':') {
                GLX_text label = {
                    .count = inst_name.count - 1,
                    .data = inst_name.data,
                };
                gasm_push_label(gasm, label, vm->program_size);
            }
            else if (text_eq(inst_name, text_cstr_as_text("push"))) {
                line = text_trim(line);
                vm->program[vm->program_size] = (Inst) {
                    .type = INST_PUSH,
                    .operand = text_to_int(operand)
                };
                vm->program_size += 1;
            }
            else if (text_eq(inst_name, text_cstr_as_text("dup"))) {
                vm->program[vm->program_size] = (Inst) {
                    .type = INST_DUP,
                    .operand = text_to_int(operand)
                };
                vm->program_size += 1;
            }
            else if (text_eq(inst_name, text_cstr_as_text("plus"))) {
                vm->program[vm->program_size] = (Inst) {
                    .type = INST_PLUS
                };
                vm->program_size += 1;
            }
            else if (text_eq(inst_name, text_cstr_as_text("jmp"))) {
                gasm_push_deferred_operand(gasm, vm->program_size, operand);
                vm->program[vm->program_size] = (Inst) {
                    .type = INST_JMP,
                };
                vm->program_size += 1;
            }
            else if (text_eq(inst_name, text_cstr_as_text("halt"))) {
                vm->program[vm->program_size] = (Inst) {
                    .type = INST_HALT
                };
                vm->program_size += 1;
            }
            else {
                fprintf(stderr, "ERROR: unknown instruction `%.*s`\n", (int) inst_name.count, inst_name.data);
                exit(1);
            }
        }
    }

    for (size_t i = 0; i < gasm->deferred_operands_size; ++i) {
        Word addr = gasm_find_label_addr(gasm, gasm->deferred_operands[i].label);
        vm->program[gasm->deferred_operands[i].addr].operand = addr;
    }

}
