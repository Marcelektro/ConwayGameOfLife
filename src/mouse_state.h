#ifndef MOUSE_STATE_H
#define MOUSE_STATE_H

#include <stdbool.h>
#include <time.h>

#define MOUSE_MOVEMENT_TIMEOUT 2.0 // seconds

typedef struct {
    int x, y;
    bool inside;
    time_t last_move;
} MouseState;


#endif // MOUSE_STATE_H
