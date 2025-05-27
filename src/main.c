#include <stdio.h>

#include <GL/glut.h>
#include <stdbool.h>
#include "rules.h"
#include "grid.h"
#include "render.h"
#include "ui_menu.h"
#include "shape_editor.h"

#define WIN_W 1280
#define WIN_H 720
#define GRID_W 100
#define GRID_H 60

static Grid *grid;
static Rules rules;
static int delay_ms = 100;
static bool paused = false;
static int iterations = 0;

static int win_main, options_win, shape_editor_win;


void options_force_redraw();


/***************\
|* Main Window *|
\***************/

void timer_main([[maybe_unused]] int value) {
    if (!paused) {
        grid_perform_step(grid, &rules);
        iterations++;
    }
    glutSetWindow(win_main);
    glutPostRedisplay();

    glutTimerFunc(delay_ms, timer_main, 0);
}


void main_display_func() {
    glutSetWindow(win_main);
    render_draw(grid);
    render_draw_ui(iterations, delay_ms);
    glutSwapBuffers();
}

void main_reshape_func(const int w, const int h) {
    render_resize((float)w, (float)h);
}

// mouse toggle cell or place shape
void main_mouse_func(const int button, const int state, const int x, const int y) {
    if (state != GLUT_DOWN)
        return;

    const int window_width = glutGet(GLUT_WINDOW_WIDTH);
    const int window_height = glutGet(GLUT_WINDOW_HEIGHT);

    const int gx = x * GRID_W / window_width;
    const int gy = (window_height - y) * GRID_H / window_height;

    if (button != GLUT_LEFT_BUTTON && button != GLUT_RIGHT_BUTTON)
        return;

    const bool set_alive = button == GLUT_LEFT_BUTTON;

    bool shape_has_anything = false;
    for (int i = 0; i < SHAPE_BUILDER_GRID_W * SHAPE_BUILDER_GRID_H; i++) {
        if (shape[i]) {
            shape_has_anything = true;
            break;
        }
    }

    if (shape_editor_win && shape_has_anything) {
        // set shape cells
        for (int sy = 0; sy < SHAPE_BUILDER_GRID_H; sy++) {
            for (int sx = 0; sx < SHAPE_BUILDER_GRID_W; sx++) {
                if (shape[sy * SHAPE_BUILDER_GRID_W + sx]) {
                    grid_set_at(grid, gx + sx, gy + sy, set_alive);
                }
            }
        }
    } else {
        const bool current_state = grid_get_at(grid, gx, gy);
        grid_set_at(grid, gx, gy, !current_state);
    }

    glutPostRedisplay();
}

void main_keyboard_func(const unsigned char key, [[maybe_unused]] int x, [[maybe_unused]] int y) {
    switch (key) {
        case ' ':
        case 'p':
            paused = !paused;
            options_force_redraw();
            break;
        case 'r':
            grid_reset_cells(grid);
            iterations = 0;
            break;
        case 27: // ESC
            glutDestroyWindow(win_main);
            if (options_win) glutDestroyWindow(options_win);
            if (shape_editor_win) glutDestroyWindow(shape_editor_win);
            exit(0);
        default: break;
    }
}

/******************\
|* Options Window *|
\******************/

void options_display_func() {
    glutSetWindow(options_win);
    glClear(GL_COLOR_BUFFER_BIT);
    ui_draw_options();
    glutSwapBuffers();
}

void options_reshape_func(const int w, const int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void options_force_redraw() {
    glutSetWindow(options_win);
    glutPostRedisplay();
}


/**********************\
|* Shape Editor Window *|
\**********************/

void shape_display_func() {
    glutSetWindow(shape_editor_win);
    shape_draw();
    glutSwapBuffers();
}

void shape_reshape_func(const int w, const int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void shape_mouse_func(const int b, const int s, const int x, const int y) {
    shape_mouse(b, s, x, y);
}


/********\
|* Main *|
\********/

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Main window
    glutInitWindowSize(WIN_W, WIN_H);
    win_main = glutCreateWindow("Conway's Game of Life");
    grid = grid_create(GRID_W, GRID_H);

    // rules_init_conway(&rules);
    rules_parse(&rules, "23/3");


    // init 3 windows
    render_init(WIN_W, WIN_H, GRID_W, GRID_H);
    ui_init(&rules, &delay_ms, &paused);
    shape_init();

    // Main window
    glutDisplayFunc(main_display_func);
    glutReshapeFunc(main_reshape_func);
    glutMouseFunc(main_mouse_func);
    glutKeyboardFunc(main_keyboard_func);
    glutTimerFunc(delay_ms, timer_main, 0);

    // Options window
    glutInitWindowSize(380, 400);
    options_win = glutCreateWindow("Options");
    glutDisplayFunc(options_display_func);
    glutReshapeFunc(options_reshape_func);
    glutMouseFunc(ui_mouse_options);
    glutMotionFunc(ui_mouse_motion);

    // Shape editor window
    glutInitWindowSize(300, 300);
    shape_editor_win = glutCreateWindow("Shape Builder");
    glutDisplayFunc(shape_display_func);
    glutReshapeFunc(shape_reshape_func);
    glutMouseFunc(shape_mouse_func);

    glutMainLoop();
    return 0;
}
