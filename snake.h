#ifndef SNAKE_H
#define SNAKE_H

#include "defines.h"
#include "point.h"

typedef Point SnakeSegment;

typedef struct _Snake{
    SnakeSegment body[SNAKE_MAX_LENGTH];
    int length; 
    int speed;
    Point direction;
} Snake;

Snake* init_snake(int x, int y);
void draw_snake(Snake* snake);
void move_snake(Snake* snake);

#endif
