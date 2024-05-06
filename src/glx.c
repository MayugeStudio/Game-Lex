#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "text.h"

#define GLX_STACK_CAPACITY 1024
#define GLX_PROGRAM_CAPACITY 1024
#define GLX_EXECUTION_LIMIT 50

typedef int64_t Word;

typedef enum Err
{
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND,
} Err;

const char *err_as_cstr(Err err)
{
    switch (err) {
        case ERR_OK: return "ERR_OK";
        case ERR_STACK_OVERFLOW: return "ERR_STACK_OVERFLOW";
        case ERR_STACK_UNDERFLOW: return "ERR_STACK_UNDER";
        case ERR_DIV_BY_ZERO: return "ERR_DIV_BY_ZERO";
        case ERR_ILLEGAL_INST: return "ERR_ILLEGAL_INST";
        case ERR_ILLEGAL_INST_ACCESS: return "ERR_ILLEGAL_INST_ACCESS";
        case ERR_ILLEGAL_OPERAND: return "ERR_ILLEGAL_OPERAND";
        default:assert(0 && "err_as_cstr: unreachable");
    }
}

typedef enum Inst_Type
{
    INST_NOP = 0,
    INST_PUSH,
    INST_PLUS,
    INST_MINUS,
    INST_MULT,
    INST_DIV,
    INST_EQ,
    INST_JMP_IF,
    INST_JMP,
    INST_HALT,
    INST_WRITE,
    INST_DUP,
} Inst_Type;

typedef struct Inst
{
    Inst_Type type;
    int operand;
} Inst;

typedef struct GamVM
{
    Word stack[GLX_STACK_CAPACITY];
    Word stack_size;

    Inst program[GLX_PROGRAM_CAPACITY];
    Word ip;

    int halt;
    Word program_size;
} Glx_vm;

Err GLEX_execute_inst(Glx_vm *vm)
{
    if (vm->ip < 0 || vm->ip >= vm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }

    Inst inst = vm->program[vm->ip];

    switch (inst.type) {
        case INST_NOP:vm->ip += 1;
            break;
        case INST_PUSH:
            if (vm->stack_size >= GLX_STACK_CAPACITY) {
                return ERR_STACK_OVERFLOW;
            }
            vm->stack[vm->stack_size] = inst.operand;
            vm->stack_size += 1;
            vm->ip += 1;
            break;

        case INST_PLUS:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_size - 2] += vm->stack[vm->stack_size - 1];
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_MINUS:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_size - 2] -= vm->stack[vm->stack_size - 1];
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_MULT:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_size - 2] *= vm->stack[vm->stack_size - 1];
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_DIV:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            if (vm->stack[vm->stack_size - 1] == 0) {
                return ERR_DIV_BY_ZERO;
            }
            vm->stack[vm->stack_size - 2] /= vm->stack[vm->stack_size - 1];
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_EQ:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_size - 2] = vm->stack[vm->stack_size - 1] == vm->stack[vm->stack_size - 2];
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_JMP_IF:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }

            if (vm->stack[vm->stack_size - 1]) {
                vm->stack_size -= 1;
                vm->ip = inst.operand;
            }
            else {
                vm->ip++;
            }
            break;

        case INST_JMP:vm->ip = inst.operand;
            break;

        case INST_HALT:vm->halt = 1;
            break;

        case INST_WRITE:
            if (vm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            printf("%lld", vm->stack[vm->stack_size - 1]);
            vm->stack_size -= 1;
            vm->ip += 1;
            break;

        case INST_DUP:
            if (vm->stack_size - inst.operand <= 0) {
                return ERR_STACK_UNDERFLOW;
            }

            if (inst.operand < 0) {
                return ERR_ILLEGAL_OPERAND;
            }

            vm->stack[vm->stack_size] = vm->stack[vm->stack_size - 1 - inst.operand];

            vm->stack_size += 1;
            vm->ip += 1;
            break;

        default:return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

void
GLX_dump_stack(FILE *stream, Glx_vm *gm)
{
    fprintf(stream, "STACK: size = %zu, ip = %lld\n", gm->stack_size, gm->ip);
    if (gm->stack_size > 0) {
        for (Word i = 0; i < gm->stack_size; ++i) {
            fprintf(stream, "  %lld\n", gm->stack[i]);
        }
    }
    else {
        fprintf(stream, "  [empty]\n");
        return;
    }
}

void
GLX_save_program_as_bytecode(Inst *program, size_t program_size, const char *file_path)
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

void
GLX_load_bytecode(Glx_vm *gm, const char *file_path)
{
    FILE *f = fopen(file_path, "rb");
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

    assert(size % sizeof(gm->program[0]) == 0);
    assert((size_t)size <= GLX_PROGRAM_CAPACITY * sizeof(gm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    size_t program_size = fread(gm->program, sizeof(gm->program[0]), size / sizeof(gm->program[0]), f);

    gm->program_size = (Word)program_size;

    fclose(f);
}

GLX_text GLX_load_source_code(const char *file_path) {
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

Inst GAM_parse_line(GLX_text line)
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
            program[program_size] = GAM_parse_line(line);
            program_size += 1;
        }
    }
    return program_size;
}

Glx_vm gm = { 0 };

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

    gm.program_size = (Word)GLX_parse_source(source, gm.program, GLX_PROGRAM_CAPACITY);

    GLX_save_program_as_bytecode(gm.program, gm.program_size, output_file_path);

    return 0;
}
int main2(void)
{
    GLX_load_bytecode(&gm, "./fib.glb");
    for (int i = 0; i < GLX_EXECUTION_LIMIT && !gm.halt; ++i) {
        Err err = GLEX_execute_inst(&gm);
        GLX_dump_stack(stdout, &gm);
        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            exit(1);
        }
    }
    return 0;
}
