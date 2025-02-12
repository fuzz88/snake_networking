#ifndef GAME_H
#define GAME_H

#include <stddef.h>

#include "snake.h"

typedef struct _World {
    Snake   **snakes;
    size_t  snakes_count;
} World;

typedef struct _Player {
    size_t snake_idx;
    size_t score;
    char   *name;
} Player;


typedef struct _Game {
    World   *world;
    Player  **players;
    size_t  players_count;
} Game;


Game *init_game(char* player_name);

#endif
