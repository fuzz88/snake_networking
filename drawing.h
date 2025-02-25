#ifndef DRAWING_H
#define DRAWING_H
#include <stddef.h>

#include "raylib.h"

typedef struct _Game Game;

enum SCREEN_STATE
{
    GAME,
    MENU,
};

typedef struct _Screen
{
    enum SCREEN_STATE state;
    size_t menu_idx;
    size_t menu_len;
} Screen;

void draw_grid();
void draw_snake_segment(int x, int y);
void main_drawing_loop(Game *game);

#endif
