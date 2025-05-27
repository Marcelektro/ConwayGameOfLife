#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include "rules.h"

/**
 * Grid representation in the form of a 1d array of cells, accessed by
 * going through 2d coordinates (x,y).
 */
typedef struct {
    int width, height;

    bool *cells;        // current state
    bool *cells_next;   // next state
} Grid;


Grid* grid_create(int width, int height);
void grid_destroy(Grid *g);

bool grid_get_at(const Grid *g, int x, int y);
void grid_set_at(const Grid *g, int x, int y, bool alive);
void grid_reset_cells(const Grid *g);

void grid_perform_step(const Grid *g, const Rules *rules);

#endif //GRID_H
