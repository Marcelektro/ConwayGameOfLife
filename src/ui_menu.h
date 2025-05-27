#ifndef UI_H
#define UI_H

#include "rules.h"


void ui_init(Rules *rules, int *delay_ms, bool *paused);

void ui_draw_options();

void ui_mouse_options(int button, int state, int x, int y);
void ui_mouse_motion(int x, int y);

#endif //UI_H
