#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include "game.h"
#include "snake.h"
#include "defines.h"
#include "networking.h"


Game *init_game() {
    srand(time(0));

    Game *g = malloc(sizeof(Game));
    assert(g != NULL);

    g->players = malloc(sizeof(Player*) * MAX_PLAYERS);
    assert(g->players != NULL);
    
    Player *p = malloc(sizeof(Player));

    p->id = rand() % UINT16_MAX;
    p->score = 0;
    p->snake_idx = 0;

    Snake *s = init_snake(rand()%(MAX_X-4), rand()%MAX_Y);

    World *w = malloc(sizeof(World));
    assert(w != NULL);

    w->snakes = malloc(sizeof(Snake*) * MAX_SNAKES);
    assert(w->snakes != NULL);

    w->snakes[0] = s;
    w->snakes_count = 1;

    g->world = w;
    g->players[0] = p;
    g->players_count = 1;

    pthread_mutex_init(&g->update_mutex, NULL); 

    return g;
}

void add_player(Game *game, DataPacket *received) {
    pthread_mutex_lock(&game->update_mutex);
        
    assert(game->players_count + 1 < MAX_PLAYERS);

    Player *p = malloc(sizeof(Player));
    p->id = received->player_id;
    p->score = 0;
    p->snake_idx = game->world->snakes_count;
    game->world->snakes_count++;

    Snake *snake = malloc(sizeof(Snake));
    snake->length = received->length;
    memcpy(snake->body, received->snake, received->length * sizeof(SnakeSegment));
    
    game->world->snakes[p->snake_idx] = snake;
    game->players[game->players_count] = p;
    game->players_count++;

    pthread_mutex_unlock(&game->update_mutex);
}

void destroy_game(Game *game) {
    pthread_mutex_destroy(&game->update_mutex);

    free(game->world->snakes);
    free(game->world);
    for (size_t i; i< game->players_count; ++i) {
        free(game->players[i]);
    }
    free(game->players);
    free(game);
}
