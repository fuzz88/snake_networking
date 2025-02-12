#include "raylib.h"

#include "defines.h"
#include "drawing.h"
#include "game.h"


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake network");
    SetTargetFPS(60);

    unsigned int framesCounter;

    bool drawGrid = false;
    
    Game *game = init_game("Player1");

    while (!WindowShouldClose())
    {
        framesCounter++;

        BeginDrawing();
        ClearBackground(BG_COLOR);

        if (IsKeyPressed(KEY_G)) drawGrid = !drawGrid;
        if (drawGrid) draw_grid();

        if (IsKeyPressed(KEY_UP)) {
            if (game->world->snakes[0]->direction.y != 1) {
                game->world->snakes[0]->direction.x = 0;
                game->world->snakes[0]->direction.y = -1;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (game->world->snakes[0]->direction.y != -1) {
                game->world->snakes[0]->direction.x = 0;
                game->world->snakes[0]->direction.y = 1;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (game->world->snakes[0]->direction.x != 1) {
                game->world->snakes[0]->direction.x = -1;
                game->world->snakes[0]->direction.y = 0;
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (game->world->snakes[0]->direction.x != -1) {
                game->world->snakes[0]->direction.x = 1;
                game->world->snakes[0]->direction.y = 0;
            }
        }

        draw_snake(game->world->snakes[0]);

        if (framesCounter % (TARGET_FPS / game->world->snakes[0]->speed) == 0) {
            framesCounter = 0;
            move_snake(game->world->snakes[0]);
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
