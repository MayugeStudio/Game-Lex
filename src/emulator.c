#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulator.h"

void GLX_load_bytecode(GLX_vm *gm, const char *file_path)
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

Err GLX_execute_inst(GLX_vm *vm)
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

Err GLX_execute_program(GLX_vm *vm, int limit)
{
    while (limit > 0 && !vm->halt) {
        Err err = GLX_execute_inst(vm);
        if (err != ERR_OK) {
            return err;
        }

        if (limit > 0) {
            limit -= 1;
        }
    }
    return ERR_OK;
}

void GLX_dump_stack(FILE *stream, GLX_vm *gm)
{
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
