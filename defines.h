#ifndef DEFINES_H
#define DEFINES_H

#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600
#define CELL_WIDTH          20
#define CELL_HEIGHT         20

#define GRID_MAX_X          SCREEN_WIDTH / CELL_WIDTH 
#define GRID_MAX_Y          SCREEN_HEIGHT / CELL_HEIGHT
#define GRID_MIN_X          0
#define GRID_MIN_Y          0

#define MAX_X               GRID_MAX_X - 1 
#define MAX_Y               GRID_MAX_Y - 1 
#define MIN_X               0
#define MIN_Y               0

#define BG_COLOR            (Color){0x13, 0x13, 0x13, 0xFF}
#define GRID_COLOR          (Color){0xAF, 0xAF, 0xAF, 0xFF}
#define SNAKE_COLOR         (Color){0x10, 0x9F, 0x10, 0xFF}

#define SNAKE_MAX_LENGTH    50
#define TARGET_FPS          60
#define SNAKE_START_SPEED   5

#define MAX_PLAYERS         10
#define MAX_SNAKES          10

#define SERVER_IP           "127.0.0.1"
#define SERVER_PORT         9000

#define WINDOW_TITLE        "snake network"

#endif