#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>

typedef struct
{
    size_t count;
    const char *data;
} GLX_text;

GLX_text text_cstr_as_text(const char *cstr);
GLX_text text_trim_left(GLX_text text);
GLX_text text_trim_right(GLX_text text);
GLX_text text_trim(GLX_text text);
GLX_text text_chop_by_delim(GLX_text *text, char delim);
int text_eq(GLX_text a, GLX_text b);
int text_to_int(GLX_text text);

#endif //TEXT_H
