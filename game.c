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

    g->players = malloc(sizeof(Player*) * 10);
    assert(g->players != NULL);
    
    Player *p = malloc(sizeof(Player));
    p->id = rand() % UINT16_MAX;
    p->score = 0;
    p->snake_idx = 0;

    Snake *s = init_snake(rand()%(MAX_X-4), rand()%MAX_Y);

    World *w = malloc(sizeof(World));
    w->snakes = malloc(sizeof(Snake*) * 10);
    assert(w->snakes != NULL);
    w->snakes[0] = s;
    w->snakes_count = 0;

    g->world = w;
    g->players[0] = p;
    g->players_count = 1;

    pthread_mutex_init(&g->freeze_mutex, NULL); 

    return g;
}
