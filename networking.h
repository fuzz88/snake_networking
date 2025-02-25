#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdint.h>

#include "point.h"
#include "game.h"

typedef struct _DataPacket
{
    uint16_t player_id;
    uint16_t length;
    SnakeSegment *snake;
} DataPacket;

void *client_func(void *data);
void start_networking(Game *game);

#endif