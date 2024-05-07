//
// Created by eyebrow on 2024/05/07.
//

#ifndef GLEX_H
#define GLEX_H

#include <stdint.h>

#define GLX_STACK_CAPACITY 1024
#define GLX_PROGRAM_CAPACITY 1024
#define GLX_EXECUTION_LIMIT 50

typedef int64_t Word;

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
} GLX_vm;
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

const char *err_as_cstr(Err err);

#endif //GLEX_H
