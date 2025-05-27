#include "rules.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 23/3 (Conway's Game of Life)
void rules_init_conway(Rules *r) {
    for (int i = 0; i < 9; i++) {
        r->survive[i] = i == 2 || i == 3;
        r->birth[i] = i == 3;
    }
}

/**
 * Parse rules string in the format "survive/birth", e.g. "23/3".
 * Returns true on success, false on failure.
 */
bool rules_parse(Rules *r, const char *str) {
    char *s = strdup(str); // work on a copy
    char *p = strchr(s, '/');

    if (!p) {
        free(s);
        return false;
    }

    for (int i = 0; i < 9; i++) {
        r->survive[i] = false;
        r->birth[i] = false;
    }

    for (const char *c = s; *c; c++) {
        if (isdigit(*c))
            r->survive[*c - '0'] = true;
    }

    *p = 0; // replace sep with '\0'
    p++;
    for (const char *c = p; *c; c++) {
        if (isdigit(*c))
            r->birth[*c - '0'] = true;
    }

    free(s);
    return true;
}
