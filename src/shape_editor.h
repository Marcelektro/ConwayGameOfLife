#ifndef SHAPE_EDITOR_H
#define SHAPE_EDITOR_H

#include <stdbool.h>

#define SHAPE_BUILDER_GRID_W 10
#define SHAPE_BUILDER_GRID_H 10

extern bool shape[SHAPE_BUILDER_GRID_W * SHAPE_BUILDER_GRID_H];


void shape_init();

void shape_draw();

void shape_mouse(int button, int state, int x, int y);

#endif //SHAPE_EDITOR_H
