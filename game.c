#include <assert.h>
#include <stdlib.h>

#include "game.h"
#include "snake.h"


Game *init_game(char* player_name) {
    Game *g = malloc(sizeof(Game));
    assert(g != NULL);

    g->players = malloc(sizeof(Player*) * 10);
    assert(g->players != NULL);
    
    Player *p = malloc(sizeof(Player));
    p->name = player_name;
    p->score = 0;
    p->snake_idx = 0;

    Snake *s = init_snake(10, 10);

    World *w = malloc(sizeof(World));
    w->snakes = malloc(sizeof(Snake*) * 10);
    assert(w->snakes != NULL);
    w->snakes[0] = s;
    w->snakes_count = 0;

    g->world = w;
    g->players[0] = p;
    g->players_count = 0;

    return g;
}
