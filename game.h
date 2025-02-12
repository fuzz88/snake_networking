#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#include "snake.h"

typedef struct _World {
    Snake   **snakes;
    size_t  snakes_count;
} World;

typedef struct _Player {
    size_t snake_idx;
    size_t score;
    uint16_t id;
} Player;


typedef struct _Game {
    World   *world;
    Player  **players;
    size_t  players_count;
    pthread_mutex_t freeze_mutex;
} Game;


Game *init_game();

#endif
