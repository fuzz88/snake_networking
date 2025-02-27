#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#include "snake.h"
#include "drawing.h"

typedef struct _World
{
    Snake **snakes;
    size_t snakes_count;
} World;

typedef struct _Player
{
    size_t snake_idx;
    size_t score;
    uint16_t id;
} Player;

typedef struct _Game
{
    Screen *screen;
    World *world;
    Player **players;
    size_t players_count;
    pthread_mutex_t update_mutex;
} Game;

typedef struct _DataPacket DataPacket;

Game *init_game();
void destroy_game(Game *game);
void add_player(Game *game, DataPacket *received);
bool update_player(Game *game, DataPacket *received);

#endif
