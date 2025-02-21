#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#include "game.h"
#include "snake.h"
#include "defines.h"


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
    w->snakes_count = 0;

    g->world = w;
    g->players[0] = p;
    g->players_count = 1;

    pthread_mutex_init(&g->freeze_mutex, NULL); 

    return g;
}

void destroy_game(Game *game) {
    pthread_mutex_destroy(&game->freeze_mutex);

    free(game->world->snakes);
    free(game->world);
    for (size_t i; i< game->players_count; ++i) {
        free(game->players[i]);
    }
    free(game->players);
    free(game);
}
