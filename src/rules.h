#ifndef RULES_H
#define RULES_H

#include <stdbool.h>

/**
 * survive[n] = true, if a live cell with n neighbors survives
 * birth[n]   = true, if a dead cell with n neighbors becomes alive
 */
typedef struct {
    bool survive[9];
    bool birth[9];
} Rules;


// Conway "23/3"
void rules_init_conway(Rules *r);


// Parse string (survive/birth) into a rule
bool rules_parse(Rules *r, const char *str);

#endif //RULES_H
