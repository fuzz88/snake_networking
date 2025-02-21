#include <stdint.h>

#include "point.h"
#include "game.h"

typedef struct _DataPacket {
    uint16_t    player_id;
    Point       direction; 
} DataPacket;


void serialize(const DataPacket *data, char *buffer);
void deserialize(const char *buffer, DataPacket *data);
void *client_func(void *data);
void start_networking(Game *game);