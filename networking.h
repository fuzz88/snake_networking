#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdint.h>

#include "point.h"
#include "game.h"


typedef struct DataPacket
{
    uint32_t packet_type;
    uint32_t player_id;
    uint32_t data_len;
    Point *data;
} DataPacket;

void start_networking(Game *game);

#endif