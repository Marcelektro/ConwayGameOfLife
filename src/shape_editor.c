#include "shape_editor.h"
#include <GL/glut.h>
#include <string.h>

#define GRID_FILL 0.07f
#define BORDER_COLOR 0.4f

bool shape[SHAPE_BUILDER_GRID_W * SHAPE_BUILDER_GRID_H];

void shape_init() {
    memset(shape, 0, sizeof(shape));
}

void shape_draw() {
    glClearColor(GRID_FILL, GRID_FILL, GRID_FILL, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    const int window_width = glutGet(GLUT_WINDOW_WIDTH);
    const int window_height = glutGet(GLUT_WINDOW_HEIGHT);

    const float cell_width = (float) window_width / SHAPE_BUILDER_GRID_W;
    const float cell_height = (float) window_height / SHAPE_BUILDER_GRID_H;

    // draw grid
    glColor3f(BORDER_COLOR, BORDER_COLOR, BORDER_COLOR);
    for (int i = 0; i <= SHAPE_BUILDER_GRID_W; i++) {
        glBegin(GL_LINES);
        glVertex2f((float)i * cell_width, 0);
        glVertex2f((float)i * cell_width, (float)window_height);
        glEnd();
    }
    for (int i = 0; i <= SHAPE_BUILDER_GRID_H; i++) {
        glBegin(GL_LINES);
        glVertex2f(0, (float)i * cell_height);
        glVertex2f((float)window_width, (float)i * cell_height);
        glEnd();
    }

    // draw alive
    glColor3f(0.5f, 0.1f, 0.2f);
    glBegin(GL_QUADS);
    for (int y = 0; y < SHAPE_BUILDER_GRID_H; y++) {
        for (int x = 0; x < SHAPE_BUILDER_GRID_W; x++) {
            if (shape[y * SHAPE_BUILDER_GRID_W + x]) {
                const float x0 = (float)x * cell_width;
                const float y0 = (float)y * cell_height;

                glVertex2f(x0, y0);
                glVertex2f(x0 + cell_width, y0);
                glVertex2f(x0 + cell_width, y0 + cell_height);
                glVertex2f(x0, y0 + cell_height);
            }
        }
    }
    glEnd();
}

void shape_mouse(const int button, const int state, const int x, const int y) {
    if (state != GLUT_DOWN || button != GLUT_LEFT_BUTTON)
        return;

    const int w = glutGet(GLUT_WINDOW_WIDTH);
    const int h = glutGet(GLUT_WINDOW_HEIGHT);

    const int xi = x / (w / SHAPE_BUILDER_GRID_W);
    const int yi = (h - y) / (h / SHAPE_BUILDER_GRID_H);

    if (xi >= 0 && xi < SHAPE_BUILDER_GRID_W && yi >= 0 && yi < SHAPE_BUILDER_GRID_H) {
        shape[yi * SHAPE_BUILDER_GRID_W + xi] = !shape[yi * SHAPE_BUILDER_GRID_W + xi];
        glutPostRedisplay();
    }
}
