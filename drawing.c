#include "raylib.h"

#include "defines.h"
#include "drawing.h"
#include "keyboard.h"
#include "snake.h"
#include "game.h"


void draw_grid() {
    for (int cell_x = GRID_MIN_X; cell_x < GRID_MAX_X; ++cell_x) {
        DrawLine(
                cell_x * CELL_WIDTH, 0,
                cell_x * CELL_WIDTH, SCREEN_HEIGHT,
                GRID_COLOR);
    }
    for (int cell_y = GRID_MIN_Y; cell_y < GRID_MAX_Y; ++cell_y) {
        DrawLine(
                0, cell_y * CELL_HEIGHT,
                SCREEN_WIDTH, cell_y * CELL_HEIGHT,
                GRID_COLOR);                
    }
}

static inline void fill_cell(int x, int y, Color fill_color) {
    DrawRectangle(
            x * CELL_WIDTH, y * CELL_HEIGHT,
            CELL_WIDTH - 1, CELL_HEIGHT - 1,
            fill_color);
}

void draw_snake_segment(int x, int y) {
    fill_cell(x, y, SNAKE_COLOR);
}

void clear_snake_segment(int x, int y) {
    fill_cell(x, y, BG_COLOR);
}

void drawing_loop(Game *game) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake network");
    SetTargetFPS(60);

    size_t framesCounter = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        framesCounter++;
        ClearBackground(BG_COLOR);

        process_keyboard_events(game);

        draw_snake(game->world->snakes[0]);

        size_t frames_per_tick = TARGET_FPS / game->world->snakes[0]->speed;
        if (framesCounter % frames_per_tick == 0) {
            framesCounter = 0;
            move_snake(game->world->snakes[0]);
        }

        EndDrawing();
    }

    CloseWindow();
}
