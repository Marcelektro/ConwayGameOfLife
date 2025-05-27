#include "render.h"
#include <GL/glut.h>
#include <stdio.h>

#define GRID_FILL_R 10
#define GRID_FILL_G 20
#define GRID_FILL_B 30

#define GRID_BORDER_R 40
#define GRID_BORDER_G 40
#define GRID_BORDER_B 40

#define CELL_FILL_R 0.5
#define CELL_FILL_G 0.1
#define CELL_FILL_B 0.2


static GLfloat window_width, window_height;

static GLuint grid_texture;
static GLfloat grid_width, grid_height;

static float cell_width, cell_height;


static void gen_grid_texture() {
    const int tex_size = 16;
    unsigned char img[tex_size][tex_size][3]; // rgb img data

    for (int y = 0; y < tex_size; y++) {
        for (int x = 0; x < tex_size; x++) {
            // draw 1-px lines for the border
            if (x == 0 || y == 0) {
                img[y][x][0] = GRID_BORDER_R;
                img[y][x][1] = GRID_BORDER_G;
                img[y][x][2] = GRID_BORDER_B;
            } else {
                img[y][x][0] = GRID_FILL_R;
                img[y][x][1] = GRID_FILL_G;
                img[y][x][2] = GRID_FILL_B;
            }
        }
    }
    glGenTextures(1, &grid_texture);
    glBindTexture(GL_TEXTURE_2D, grid_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_size, tex_size, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void render_init(const GLfloat w_width, const GLfloat w_height, const GLfloat g_width, const GLfloat g_height) {
    window_width = w_width;
    window_height = w_height;
    grid_width = g_width;
    grid_height = g_height;
    cell_width = w_width / g_width;
    cell_height = w_height / g_height;
    glEnable(GL_TEXTURE_2D);
    gen_grid_texture();
}

void render_resize(const GLfloat new_width, const GLfloat new_height) {
    window_width = new_width;
    window_height = new_height;
    cell_width = (float) new_width / grid_width;
    cell_height = (float) new_height / grid_height;
    glViewport(0, 0, (int)new_width, (int)new_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, new_width, 0, new_height); // cartesian coords from (0,0) to (new_width, new_height)
    glMatrixMode(GL_MODELVIEW);
}

void render_draw(const Grid *g) {
    glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT);

    // draw grid as textured quad covering entire window
    glBindTexture(GL_TEXTURE_2D, grid_texture);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(grid_width, 0);
    glVertex2f(window_width, 0);
    glTexCoord2f(grid_width, grid_height);
    glVertex2f(window_width, window_height);
    glTexCoord2f(0, grid_height);
    glVertex2f(0, window_height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    // draw alive cells individually
    glColor3f(CELL_FILL_R, CELL_FILL_G, CELL_FILL_B);
    glBegin(GL_QUADS);
    for (int y = 0; y < g->height; y++) {
        for (int x = 0; x < g->width; x++) {
            if (!grid_get_at(g, x, y))
                continue;

            const float x0 = (float) x * cell_width;
            const float y0 = (float) y * cell_height;
            const float x1 = x0 + cell_width;
            const float y1 = y0 + cell_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
        }
    }
    glEnd();
}

void render_draw_ui(const int iterations, const int delay_ms) {
    char buf[64];
    glColor3f(1, 1, 1);

    glRasterPos2f(10, window_height - 20);
    snprintf(buf, sizeof(buf), "Iteration: %d", iterations);
    for (const char *chr = buf; *chr; chr++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *chr);

    glRasterPos2f(10, window_height - 40);
    snprintf(buf, sizeof buf, "Delay: %d ms", delay_ms);
    for (const char *chr = buf; *chr; chr++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *chr);
}
