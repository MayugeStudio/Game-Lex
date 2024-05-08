#ifndef GLEX_H
#define GLEX_H

#include <stdint.h>
#include "text.h"

#define GLX_STACK_CAPACITY 1024
#define GLX_PROGRAM_CAPACITY 1024
#define GLX_EXECUTION_LIMIT 50
#define GLX_LABEL_CAPACITY 1024
#define DEFERRED_OPERANDS_CAPACITY 1024

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
    Word operand;
} Inst;

typedef struct GamVM
{
    Word stack[GLX_STACK_CAPACITY];
    Word stack_size;

    Inst program[GLX_PROGRAM_CAPACITY];
    Word ip;

    int halt;
    Word program_size;
} Gvm;
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

typedef struct
{
    GLX_text name;
    Word addr;
} Label;

typedef struct
{
    Word addr;
    GLX_text label;
} Declared_Addr;

typedef struct
{
    Label labels[GLX_LABEL_CAPACITY];
    size_t labels_size;

    Declared_Addr declared_addresses[DEFERRED_OPERANDS_CAPACITY];
    size_t declared_addresses_size;
} Gasm;

Word gasm_find_label_addr(const Gasm *gasm, GLX_text name);
void gasm_push_label(Gasm *gasm, GLX_text name, Word addr);
void gasm_push_declared_addr(Gasm *gasm, Word addr, GLX_text label);

const char *err_as_cstr(Err err);

#endif //GLEX_H
