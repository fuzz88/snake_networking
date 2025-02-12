#ifndef DRAWING_H
#define DRAWING_H

#include "raylib.h"

#include "game.h"


void draw_grid();
static inline void fill_cell(int x, int y, Color fill_color);
void draw_snake_segment(int x, int y);
void clear_snake_segment(int x, int y);
void drawing_loop(Game *game);

#endif
