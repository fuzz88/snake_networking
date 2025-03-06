#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#include "snake.h"
#include "drawing.h"

typedef struct World
{
    Snake **snakes;
    size_t snakes_count;
    Point apple;
} World;

typedef struct Player
{
    size_t snake_idx;
    uint32_t score;
    uint32_t id;
} Player;

typedef struct Game
{
    Screen *screen;
    World *world;
    Player **players;
    size_t players_count;
    bool winner;
    bool looser;
    pthread_mutex_t update_mutex;
} Game;

typedef struct DataPacket DataPacket;

Game *init_game();
void destroy_game(Game *game);
void add_player(Game *game, DataPacket *received);
bool update_player(Game *game, DataPacket *received);

#endif
