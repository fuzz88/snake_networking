#include "raylib.h"


#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define CELL_WIDTH      20
#define CELL_HEIGHT     20


#define GRID_MAX_X      SCREEN_WIDTH / CELL_WIDTH 
#define GRID_MAX_Y      SCREEN_HEIGHT / CELL_HEIGHT
#define GRID_MIN_X      1
#define GRID_MIN_Y      1

#define MAX_X           GRID_MAX_X - 3 
#define MAX_Y           GRID_MAX_Y - 3 
#define MIN_X           0
#define MIN_Y           0

#define BG_COLOR        (Color){0x13, 0x13, 0x13, 0xFF}
#define GRID_COLOR      (Color){0xAF, 0xAF, 0xAF, 0xFF}
#define SNAKE_COLOR     (Color){0x10, 0x9F, 0x10, 0xFF}

void draw_grid() {
    for (int cell_x = GRID_MIN_X; cell_x < GRID_MAX_X; ++cell_x) {
        DrawLine(cell_x * CELL_WIDTH, CELL_WIDTH, cell_x * CELL_WIDTH, SCREEN_HEIGHT - CELL_HEIGHT, GRID_COLOR);
    }
    for (int cell_y = GRID_MIN_Y; cell_y < GRID_MAX_Y; ++cell_y) {
        DrawLine(CELL_HEIGHT, cell_y * CELL_HEIGHT, SCREEN_WIDTH - CELL_WIDTH, cell_y * CELL_HEIGHT, GRID_COLOR);                
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

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake network");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BG_COLOR);
        draw_grid();
        EndDrawing();
        draw_snake_segment(0, 0);
        draw_snake_segment(MAX_X, MAX_Y);
        draw_snake_segment(0, MAX_Y);
        draw_snake_segment(MAX_X, 0);

        clear_snake_segment(MAX_X, MAX_Y);
    }
    
    CloseWindow();

    return 0;
}
