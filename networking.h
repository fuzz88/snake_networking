#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdint.h>

#include "point.h"
#include "game.h"

typedef struct DataPacket
{
    uint16_t player_id;
    uint16_t snake_length;
    SnakeSegment *snake;
} DataPacket;

void start_networking(Game *game);

#endif