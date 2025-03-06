#include <stdio.h>

#include "raylib.h"

#include "defines.h"
#include "drawing.h"
#include "keyboard.h"
#include "snake.h"
#include "game.h"

void draw_grid()
{
    for (int cell_x = GRID_MIN_X; cell_x < GRID_MAX_X; ++cell_x)
    {
        DrawLine(
            cell_x * CELL_WIDTH, 0,
            cell_x * CELL_WIDTH, SCREEN_HEIGHT,
            GRID_COLOR);
    }
    for (int cell_y = GRID_MIN_Y; cell_y < GRID_MAX_Y; ++cell_y)
    {
        DrawLine(
            0, cell_y * CELL_HEIGHT,
            SCREEN_WIDTH, cell_y * CELL_HEIGHT,
            GRID_COLOR);
    }
}

static inline void fill_cell(int x, int y, Color fill_color)
{
    DrawRectangle(
        x * CELL_WIDTH, y * CELL_HEIGHT,
        CELL_WIDTH - 1, CELL_HEIGHT - 1,
        fill_color);
}

void draw_snake_segment(int x, int y)
{
    fill_cell(x, y, SNAKE_COLOR);
}

void clear_snake_segment(int x, int y)
{
    fill_cell(x, y, BG_COLOR);
}

void draw_apple(Point p) {
    fill_cell(p.x, p.y, RED);
}

void draw_score(Game *game) {
    char text[50];
    for (size_t i = 0; i < game->players_count; i++ ) {
        sprintf((char*)&text, "Score: %d", game->players[i]->score);
        DrawText(text, 30, 30 + 20 * i, 12, BLUE);
    }

}

void game_screen(Game *game, size_t *framesCounter)
{

    pthread_mutex_lock(&game->update_mutex);

    for (size_t i = 0; i < game->world->snakes_count; ++i)
    {
        draw_snake(game->world->snakes[i]);
    }

    if (game->world->apple.x != 0) { // single coord check. init empty apple as {0, 0}, any other coords is received from server
        draw_apple(game->world->apple);
    }

    draw_score(game);

    pthread_mutex_unlock(&game->update_mutex);

    size_t frames_per_tick = TARGET_FPS / game->world->snakes[0]->speed;

    if (*framesCounter % frames_per_tick == 0)
    {
        *framesCounter = 0;
        move_snake(game);
    }
}

void menu_screen(Game *game)
{
    switch (game->screen->menu_idx) {
        case 0:
            DrawText("START", 100, 100, 32, SNAKE_COLOR);
            DrawText("EXIT", 100, 140, 32, GRID_COLOR);
            break;
        case 1:
            DrawText("START", 100, 100, 32, GRID_COLOR);
            DrawText("EXIT", 100, 140, 32, SNAKE_COLOR);
            break;
    }
}

void main_drawing_loop(Game *game)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    SetTargetFPS(60);
    size_t framesCounter = 0;

    Screen screen = (Screen){.state = MENU, .menu_idx = 0, .menu_len = 2};
    game->screen = &screen;

    while (!WindowShouldClose())
    {
        framesCounter++;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        process_keyboard_events(game);

        switch (screen.state)
        {
        case GAME:
            game_screen(game, &framesCounter);
            break;
        case MENU:
            menu_screen(game);
        default:
            break;
        }

        EndDrawing();
    }
    CloseWindow();
}
