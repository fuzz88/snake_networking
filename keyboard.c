#include <stdlib.h>

#include "raylib.h"

#include "game.h"
#include "drawing.h"

bool drawGrid = false;

void process_keyboard_events(Game *game)
{
    switch (game->screen->state)
    {
    case GAME:
        if (IsKeyPressed(KEY_G))
            drawGrid = !drawGrid;
        if (drawGrid)
            draw_grid();

        if (IsKeyPressed(KEY_UP))
        {
            if (game->world->snakes[0]->direction.y != 1)
            {
                game->world->snakes[0]->direction.x = 0;
                game->world->snakes[0]->direction.y = -1;
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            if (game->world->snakes[0]->direction.y != -1)
            {
                game->world->snakes[0]->direction.x = 0;
                game->world->snakes[0]->direction.y = 1;
            }
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            if (game->world->snakes[0]->direction.x != 1)
            {
                game->world->snakes[0]->direction.x = -1;
                game->world->snakes[0]->direction.y = 0;
            }
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            if (game->world->snakes[0]->direction.x != -1)
            {
                game->world->snakes[0]->direction.x = 1;
                game->world->snakes[0]->direction.y = 0;
            }
        }
        break;
    case MENU:
        if (IsKeyPressed(KEY_UP))
        {
            game->screen->menu_idx = (++game->screen->menu_idx) % game->screen->menu_len;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            game->screen->menu_idx = (--game->screen->menu_idx) % game->screen->menu_len;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            switch (game->screen->menu_idx) {
                case 0:
                    game->screen->state = GAME;
                    break;
                case 1:
                    exit(0);
            }
        }

    default:
        break;
    }
}
