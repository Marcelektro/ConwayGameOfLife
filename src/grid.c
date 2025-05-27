#include "grid.h"
#include <stdlib.h>
#include <string.h>


Grid *grid_create(const int width, const int height) {
    Grid *g = malloc(sizeof(Grid));
    g->width = width;
    g->height = height;

    g->cells = calloc(width * height, sizeof(bool));
    g->cells_next = calloc(width * height, sizeof(bool));

    return g;
}

void grid_destroy(Grid *g) {
    free(g->cells);
    free(g->cells_next);
    free(g);
}


static int index_at(const Grid *g, int x, int y) {
    // handle warping around edges
    x = (x + g->width) % g->width;
    y = (y + g->height) % g->height;
    return y * g->width + x;
}

bool grid_get_at(const Grid *g, const int x, const int y) {
    return g->cells[index_at(g, x, y)];
}

void grid_set_at(const Grid *g, const int x, const int y, const bool alive) {
    g->cells[index_at(g, x, y)] = alive;
}

void grid_reset_cells(const Grid *g) {
    memset(g->cells, 0, g->width * g->height);
}


void grid_perform_step(const Grid *g, const Rules *rules) {
    for (int y = 0; y < g->height; y++) {
        for (int x = 0; x < g->width; x++) {

            int alive_neighbours = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (!(dx || dy)) // skip self
                        continue;

                    if (grid_get_at(g, x + dx, y + dy))
                        alive_neighbours++;
                }
            }
            const bool self = grid_get_at(g, x, y);

            // determine next state depending on self alive state and neighbour count from the rules
            g->cells_next[index_at(g, x, y)] = self ? rules->survive[alive_neighbours] : rules->birth[alive_neighbours];
        }
    }
    // next state becomes current
    memcpy(g->cells, g->cells_next, g->width * g->height);
}
