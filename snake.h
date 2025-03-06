#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

#include "defines.h"
#include "point.h"

typedef struct Game Game;

typedef Point Point;

typedef struct Snake
{
    Point body[SNAKE_MAX_LENGTH];
    uint8_t length;
    uint8_t speed;
    Point direction;
} Snake;

Snake *init_snake(int x, int y);
void draw_snake(Snake *snake);
void move_snake(Game *game);

#endif
