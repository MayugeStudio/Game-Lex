#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "glex.h"

Word gasm_find_label_addr(const Gasm *gasm, GLX_text name)
{
    for (size_t i = 0; i < gasm->labels_size; ++i) {
        if (text_eq(name, gasm->labels[i].name)) {
            return gasm->labels[i].addr;
        }
    }
    fprintf(stderr, "ERROR: label `%.*s` does not exists\n", (int) name.count, name.data);
    exit(1);
}
void gasm_push_label(Gasm *gasm, const GLX_text name, const Word addr)
{
    assert(gasm->labels_size < GLX_LABEL_CAPACITY);
    gasm->labels[gasm->labels_size] = (Label) { .name = name, .addr = addr };
    gasm->labels_size += 1;
}

void gasm_push_declared_addr(Gasm *gasm, Word addr, GLX_text label)
{
    assert(gasm->declared_addresses_size < DEFERRED_OPERANDS_CAPACITY);
    gasm->declared_addresses[gasm->declared_addresses_size] = (Declared_Addr) { .addr = addr, .label = label };
    gasm->declared_addresses_size += 1;
}

const char *err_as_cstr(Err err)
{
    switch (err) {
        case ERR_OK: return "ERR_OK";
        case ERR_STACK_OVERFLOW: return "ERR_STACK_OVERFLOW";
        case ERR_STACK_UNDERFLOW: return "ERR_STACK_UNDERFLOW";
        case ERR_DIV_BY_ZERO: return "ERR_DIV_BY_ZERO";
        case ERR_ILLEGAL_INST: return "ERR_ILLEGAL_INST";
        case ERR_ILLEGAL_INST_ACCESS: return "ERR_ILLEGAL_INST_ACCESS";
        case ERR_ILLEGAL_OPERAND: return "ERR_ILLEGAL_OPERAND";
        default:assert(0 && "err_as_cstr: unreachable");
    }
}
