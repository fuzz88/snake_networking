#include "raylib.h"
#include <stdlib.h>
#include <assert.h>


#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define CELL_WIDTH      20
#define CELL_HEIGHT     20

#define GRID_MAX_X      SCREEN_WIDTH / CELL_WIDTH 
#define GRID_MAX_Y      SCREEN_HEIGHT / CELL_HEIGHT
#define GRID_MIN_X      0
#define GRID_MIN_Y      0

#define MAX_X           GRID_MAX_X - 3 
#define MAX_Y           GRID_MAX_Y - 3 
#define MIN_X           0
#define MIN_Y           0

#define BG_COLOR        (Color){0x13, 0x13, 0x13, 0xFF}
#define GRID_COLOR      (Color){0xAF, 0xAF, 0xAF, 0xFF}
#define SNAKE_COLOR     (Color){0x10, 0x9F, 0x10, 0xFF}

#define SNAKE_MAX_LENGTH    50
#define TARGET_FPS          60


void draw_grid() {
    for (int cell_x = GRID_MIN_X; cell_x < GRID_MAX_X; ++cell_x) {
        DrawLine(cell_x * CELL_WIDTH, 0, cell_x * CELL_WIDTH, SCREEN_HEIGHT, GRID_COLOR);
    }
    for (int cell_y = GRID_MIN_Y; cell_y < GRID_MAX_Y; ++cell_y) {
        DrawLine(0, cell_y * CELL_HEIGHT, SCREEN_WIDTH, cell_y * CELL_HEIGHT, GRID_COLOR);                
    }
}

static inline void fill_cell(int x, int y, Color fill_color) {
    DrawRectangle((x + 1) * CELL_WIDTH, (y + 1) * CELL_HEIGHT, CELL_WIDTH - 1, CELL_HEIGHT - 1, fill_color);
}

void draw_snake_segment(int x, int y) {
    fill_cell(x, y, SNAKE_COLOR);
}

void clear_snake_segment(int x, int y) {
    fill_cell(x, y, BG_COLOR);
}

struct Point {
    int x;
    int y;
};

typedef struct Point Point;

typedef Point SnakeSegment;

struct Snake{
    SnakeSegment body[SNAKE_MAX_LENGTH];
    int length; 
    int speed;
    Point direction;
};

typedef struct Snake Snake;

Snake* init_snake(int x, int y) {
    Snake* snake = malloc(sizeof(Snake));   
    assert(snake != NULL);

    snake->body[0] = (Point){ .x = x+0, .y = y};
    snake->body[1] = (Point){ .x = x+1, .y = y};
    snake->body[2] = (Point){ .x = x+2, .y = y};

    snake->length = 3;
    snake->speed  = 2;

    snake->direction = (Point){.x = 1, .y = 0};

    return snake;
}

void draw_snake(Snake* snake) {
    for (int i = 0; i < snake->length; ++i) {
        draw_snake_segment(snake->body[i].x, snake->body[i].y);
    }
}

void move_snake(Snake* snake) {
    for (int i = 0; i < snake->length-1; ++i) {
        snake->body[i] = snake->body[i+1];
    }

    int next_x = snake->body[snake->length-1].x + snake->direction.x;
    int next_y = snake->body[snake->length-1].y + snake->direction.y;
    if (next_x >= MAX_X) next_x = MIN_X;
    if (next_y >= MAX_Y) next_y = MIN_Y;
    if (next_x <= MIN_X) next_x = MAX_X;
    if (next_y <= MIN_Y) next_y = MAX_Y;
    snake->body[snake->length-1].x = next_x;
    snake->body[snake->length-1].y = next_y;

}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake network");
    SetTargetFPS(60);

    unsigned int framesCounter;

    bool drawGrid = false;
    
    Snake* snake = init_snake(10, 10);

    while (!WindowShouldClose())
    {
        framesCounter++;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        if (IsKeyPressed(KEY_G)) drawGrid = !drawGrid;
        if (drawGrid) draw_grid();

        if (IsKeyPressed(KEY_UP)) {
            if (snake->direction.y != 1) {
                snake->direction.x = 0;
                snake->direction.y = -1;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (snake->direction.y != -1) {
                snake->direction.x = 0;
                snake->direction.y = 1;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (snake->direction.x != 1) {
                snake->direction.x = -1;
                snake->direction.y = 0;
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (snake->direction.x != -1) {
                snake->direction.x = 1;
                snake->direction.y = 0;
            }
        }

        draw_snake(snake);

        if (framesCounter % (TARGET_FPS / snake->speed) == 0) {
            framesCounter = 0;
            move_snake(snake);
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
