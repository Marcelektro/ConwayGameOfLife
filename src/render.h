#ifndef RENDER_H
#define RENDER_H

#include <GL/gl.h>

#include "grid.h"
#include "mouse_state.h"

void render_init(GLfloat w_width, GLfloat w_height, GLfloat g_width, GLfloat g_height, MouseState *mouse_state);

void render_resize(GLfloat new_width, GLfloat new_height);

void render_draw(const Grid *g);

void render_draw_ui(int iterations, int delay_ms);

#endif //RENDER_H
