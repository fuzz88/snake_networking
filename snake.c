#include <assert.h>
#include <stdlib.h>

#include "snake.h"
#include "drawing.h"


Snake *init_snake(int x, int y) {
    Snake *snake = malloc(sizeof(Snake));   
    assert(snake != NULL);

    snake->body[0] = (Point){ .x = x+0, .y = y};
    snake->body[1] = (Point){ .x = x+1, .y = y};
    snake->body[2] = (Point){ .x = x+2, .y = y};

    snake->length = 3;
    snake->speed  = SNAKE_START_SPEED;

    snake->direction = (Point){.x = 1, .y = 0};

    return snake;
}

void draw_snake(Snake *snake) {
    for (size_t i = 0; i < snake->length; ++i) {
        draw_snake_segment(snake->body[i].x, snake->body[i].y);
    }
}

void move_snake(Snake *snake) {
    for (size_t i = 0; i < snake->length-1; ++i) {
        snake->body[i] = snake->body[i+1];
    }

    int next_x = snake->body[snake->length-1].x + snake->direction.x;
    int next_y = snake->body[snake->length-1].y + snake->direction.y;

    if (next_x > MAX_X) next_x = MIN_X;
    if (next_y > MAX_Y) next_y = MIN_Y;
    if (next_x < MIN_X) next_x = MAX_X;
    if (next_y < MIN_Y) next_y = MAX_Y;

    snake->body[snake->length-1].x = next_x;
    snake->body[snake->length-1].y = next_y;
}
