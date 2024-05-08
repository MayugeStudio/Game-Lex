#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "text.h"


GLX_text text_cstr_as_text(const char *cstr) {
    return (GLX_text) {
        .count = strlen(cstr),
        .data = cstr,
    };
}

GLX_text text_trim_left(GLX_text text) {
    size_t i = 0;
    while (i < text.count && isspace(text.data[i])) {
        i += 1;
    }

    return (GLX_text) {
        .count =  text.count - i,
        .data = text.data + i,
    };
}

GLX_text text_trim_right(GLX_text text) {
    size_t i = 0;
    while (i < text.count && isspace(text.data[text.count - 1 - i])) {
        i += 1;
    }

    return (GLX_text) {
        .count = text.count - i,
        .data = text.data,
    };
}

GLX_text text_trim(GLX_text text) {
    return text_trim_right(text_trim_left(text));
}

GLX_text text_chop_by_delim(GLX_text *text, char delim) {
    size_t i = 0;
    while (i < text->count && text->data[i] != delim) {
        i += 1;
    }

    GLX_text result = {
        .count = i,
        .data = text->data,
    };

    if (i < text->count) {
        // Skip delimiter
        text->count -= i + 1;
        text->data += i + 1;
    } else {
        // Doesn't skip delimiter
        text->count -= i;
        text->data += i;
    }

    return result;
}

int text_to_int(GLX_text text) {
    int result = 0;

    for (size_t i = 0; i < text.count && isdigit(*text.data); ++i) {
        result = result * 10 + text.data[i] - '0';
    }

    return result;
}

int text_eq(GLX_text a, GLX_text b) {
    if (a.count != b.count) {
        return 0;
    } else {
        // TODO: Can't handle a = {data = loop, count = 4}, b = {data = loop:, count = 4} pattern
        return memcmp(a.data, b.data, a.count) == 0;
    }
}
