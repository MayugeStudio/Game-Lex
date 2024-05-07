#include <assert.h>
#include "shift.h"

char *shift_arg(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}
