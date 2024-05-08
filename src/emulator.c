#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulator.h"

void GLX_load_bytecode(Gvm *gvm, const char *file_path)
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

    assert(size % sizeof(gvm->program[0]) == 0);
    assert((size_t) size <= GLX_PROGRAM_CAPACITY * sizeof(gvm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: `%s`\n", file_path, strerror(errno));
        exit(1);
    }

    size_t program_size = fread(gvm->program, sizeof(gvm->program[0]), size / sizeof(gvm->program[0]), f);

    gvm->program_size = (Word) program_size;

    fclose(f);
}

Err GLX_execute_inst(Gvm *gvm)
{
    if (gvm->ip < 0 || gvm->ip >= gvm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }

    Inst inst = gvm->program[gvm->ip];

    switch (inst.type) {
        case INST_NOP:gvm->ip += 1;
            break;
        case INST_PUSH:
            if (gvm->stack_size >= GLX_STACK_CAPACITY) {
                return ERR_STACK_OVERFLOW;
            }
            gvm->stack[gvm->stack_size] = inst.operand;
            gvm->stack_size += 1;
            gvm->ip += 1;
            break;

        case INST_PLUS:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            gvm->stack[gvm->stack_size - 2] += gvm->stack[gvm->stack_size - 1];
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_MINUS:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            gvm->stack[gvm->stack_size - 2] -= gvm->stack[gvm->stack_size - 1];
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_MULT:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            gvm->stack[gvm->stack_size - 2] *= gvm->stack[gvm->stack_size - 1];
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_DIV:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            if (gvm->stack[gvm->stack_size - 1] == 0) {
                return ERR_DIV_BY_ZERO;
            }
            gvm->stack[gvm->stack_size - 2] /= gvm->stack[gvm->stack_size - 1];
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_EQ:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            gvm->stack[gvm->stack_size - 2] = gvm->stack[gvm->stack_size - 1] == gvm->stack[gvm->stack_size - 2];
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_JMP_IF:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }

            if (gvm->stack[gvm->stack_size - 1]) {
                gvm->stack_size -= 1;
                gvm->ip = inst.operand;
            }
            else {
                gvm->ip++;
            }
            break;

        case INST_JMP:gvm->ip = inst.operand;
            break;

        case INST_HALT:gvm->halt = 1;
            break;

        case INST_WRITE:
            if (gvm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }
            printf("%lld", gvm->stack[gvm->stack_size - 1]);
            gvm->stack_size -= 1;
            gvm->ip += 1;
            break;

        case INST_DUP:
            if (gvm->stack_size - inst.operand <= 0) {
                return ERR_STACK_UNDERFLOW;
            }

            if (inst.operand < 0) {
                return ERR_ILLEGAL_OPERAND;
            }

            gvm->stack[gvm->stack_size] = gvm->stack[gvm->stack_size - 1 - inst.operand];

            gvm->stack_size += 1;
            gvm->ip += 1;
            break;

        default:return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

Err GLX_execute_program(Gvm *gvm, int limit)
{
    while (limit != 0 && !gvm->halt) {
        Err err = GLX_execute_inst(gvm);
        if (err != ERR_OK) {
            return err;
        }
        if (limit > 0) {
            limit -= 1;
        }
    }
    return ERR_OK;
}

void GLX_dump_stack(FILE *stream, Gvm *gvm)
{
    fprintf(stream, "[STACK]:\n");
    if (gvm->stack_size > 0) {
        for (Word i = 0; i < gvm->stack_size; ++i) {
            fprintf(stream, "  %lld\n", gvm->stack[i]);
        }
    }
    else {
        fprintf(stream, "  [empty]\n");
        return;
    }
}
