#include "ui_menu.h"

#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#define TEXT_COLOR 1.0f, 1.0f, 1.0f

const int SLIDER_X0 = 20, SLIDER_X1 = 280, SLIDER_Y = 350;
const float DOT_R = 8; // dot radius in pixels

const float MAX_SLIDER_VALUE = 500.0f; // max delay in ms

const int BOX_SIZE = 24;
const int BOX_MARGIN = 32;
const int UI_BASE_X = 20;
const int RULES_SURVIVE_OFFSET_Y = SLIDER_Y - 100;
const int RULES_BIRTH_OFFSET_Y = SLIDER_Y - 180;


static Rules *g_rules;
static int *g_delay;
static bool *g_paused;

static bool dragging_slider = false;

// take data from main.c
void ui_init(Rules *rules, int *delay_ms, bool *paused) {
    g_rules = rules;
    g_delay = delay_ms;
    g_paused = paused;
}

void draw_text(const float x, const float y, const char *text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i]; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    }
}

void ui_draw_options() {
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // slider
    glColor3f(TEXT_COLOR);
    draw_text(20, SLIDER_Y + 20, "Delay (ms):");

    glLineWidth(2.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2i(SLIDER_X0, SLIDER_Y);
    glVertex2i(SLIDER_X1, SLIDER_Y);
    glEnd();

    const float slider_pct = (float) *g_delay / MAX_SLIDER_VALUE;
    const int slider_dx = SLIDER_X0 + (int) ((SLIDER_X1 - SLIDER_X0) * slider_pct);
    glColor3f(0.2f, 0.2f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(slider_dx, SLIDER_Y);

    for (int angle = 0; angle <= 360; angle += 30) {
        const float rad = (float) angle * (float) M_PI / 180.0f;
        glVertex2i(slider_dx + (int) (DOT_R * cosf(rad)), SLIDER_Y + (int) (DOT_R * sinf(rad)));
    }
    glEnd();

    char buf[32];
    snprintf(buf, sizeof buf, "%d ms", *g_delay);
    draw_text(SLIDER_X1 + 10, SLIDER_Y + 5, buf);


    glColor3f(TEXT_COLOR);
    draw_text(20, SLIDER_Y - 50, "Survive if neighbors:");
    draw_text(20, SLIDER_Y - 130, "Birth if neighbors:");


    for (int n = 0; n < 9; n++) {
        const int x0 = UI_BASE_X + n * BOX_MARGIN;
        // survive box
        glColor3f(TEXT_COLOR);
        glBegin(GL_LINE_LOOP);
        glVertex2i(x0, RULES_SURVIVE_OFFSET_Y);
        glVertex2i(x0 + BOX_SIZE, RULES_SURVIVE_OFFSET_Y);
        glVertex2i(x0 + BOX_SIZE, RULES_SURVIVE_OFFSET_Y + BOX_SIZE);
        glVertex2i(x0, RULES_SURVIVE_OFFSET_Y + BOX_SIZE);
        glEnd();
        if (g_rules->survive[n]) {
            glColor3f(0, 0.6f, 0);
            glBegin(GL_QUADS);
            glVertex2i(x0 + 2, RULES_SURVIVE_OFFSET_Y + 2);
            glVertex2i(x0 + BOX_SIZE - 2, RULES_SURVIVE_OFFSET_Y + 2);
            glVertex2i(x0 + BOX_SIZE - 2, RULES_SURVIVE_OFFSET_Y + BOX_SIZE - 2);
            glVertex2i(x0 + 2, RULES_SURVIVE_OFFSET_Y + BOX_SIZE - 2);
            glEnd();
        }
        // number
        char c = '0' + n;
        glColor3f(TEXT_COLOR);
        draw_text(x0 + 8, RULES_SURVIVE_OFFSET_Y + 30, (char[]){c, 0});

        // birth box
        glColor3f(TEXT_COLOR);
        glBegin(GL_LINE_LOOP);
        glVertex2i(x0, RULES_BIRTH_OFFSET_Y);
        glVertex2i(x0 + BOX_SIZE, RULES_BIRTH_OFFSET_Y);
        glVertex2i(x0 + BOX_SIZE, RULES_BIRTH_OFFSET_Y + BOX_SIZE);
        glVertex2i(x0, RULES_BIRTH_OFFSET_Y + BOX_SIZE);
        glEnd();
        if (g_rules->birth[n]) {
            glColor3f(0.6f, 0, 0);
            glBegin(GL_QUADS);
            glVertex2i(x0 + 2, RULES_BIRTH_OFFSET_Y + 2);
            glVertex2i(x0 + BOX_SIZE - 2, RULES_BIRTH_OFFSET_Y + 2);
            glVertex2i(x0 + BOX_SIZE - 2, RULES_BIRTH_OFFSET_Y + BOX_SIZE - 2);
            glVertex2i(x0 + 2, RULES_BIRTH_OFFSET_Y + BOX_SIZE - 2);
            glEnd();
        }
        glColor3f(TEXT_COLOR);
        draw_text(x0 + 8, RULES_BIRTH_OFFSET_Y + 30, (char[]){c, 0});
    }

    // Pause/Reset hints
    glColor3f(TEXT_COLOR);
    draw_text(20, SLIDER_Y - 230, "[p] Pause/Resume");
    draw_text(20, SLIDER_Y - 250, "[r] Reset grid");
}

bool in_slider_dot(const int x, const int y) {
    const float slider_pct = (float) *g_delay / MAX_SLIDER_VALUE;
    const int slider_dx = SLIDER_X0 + (int) ((SLIDER_X1 - SLIDER_X0) * slider_pct);
    const int dx0 = slider_dx - DOT_R, dx1 = slider_dx + DOT_R;
    const int dy0 = SLIDER_Y - DOT_R, dy1 = SLIDER_Y + DOT_R;

    return x >= dx0 &&
           x <= dx1 &&
           y >= dy0 &&
           y <= dy1;
}

bool in_slider_line(const int x, const int y) {
    return x >= SLIDER_X0 && x <= SLIDER_X1 && fabs(y - SLIDER_Y) <= DOT_R + 10;
}

void update_delay_from_x(int x) {
    if (x < SLIDER_X0) x = SLIDER_X0;
    if (x > SLIDER_X1) x = SLIDER_X1;
    const float t = (float) (x - SLIDER_X0) / (SLIDER_X1 - SLIDER_X0);
    *g_delay = (int) (t * MAX_SLIDER_VALUE + 0.5f);
}

void ui_mouse_options(const int button, const int state, const int x, const int y) {
    const int win_h = glutGet(GLUT_WINDOW_HEIGHT);
    const int yy = win_h - y;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {

            // slider dot click
            if (in_slider_dot(x, yy)) {
                dragging_slider = true;
                return;
            }
            // slider line click
            if (in_slider_line(x, yy)) {
                update_delay_from_x(x);
                dragging_slider = true;
                glutPostRedisplay();
                return;
            }

            // rule box clicks
            for (int n = 0; n < 9; n++) {
                const int rx = UI_BASE_X + n * BOX_MARGIN;

                // survive row
                if (x >= rx && x <= rx + BOX_SIZE && yy >= RULES_SURVIVE_OFFSET_Y && yy <= RULES_SURVIVE_OFFSET_Y + BOX_SIZE) {
                    g_rules->survive[n] = !g_rules->survive[n];
                    glutPostRedisplay();
                    return;
                }

                // birth row
                if (x >= rx && x <= rx + BOX_SIZE && yy >= RULES_BIRTH_OFFSET_Y && yy <= RULES_BIRTH_OFFSET_Y + BOX_SIZE) {
                    g_rules->birth[n] = !g_rules->birth[n];
                    glutPostRedisplay();
                    return;
                }
            }
        } else {
            // mouse up
            dragging_slider = false;
        }
    }
}

void ui_mouse_motion(const int x, [[maybe_unused]] const int y) {
    if (!dragging_slider)
        return;
    update_delay_from_x(x);
    glutPostRedisplay();
}
