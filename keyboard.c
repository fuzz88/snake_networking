#include "raylib.h"

#include "game.h"
#include "drawing.h"

bool drawGrid = false;

void process_keyboard_events(Game *game) {
   if (IsKeyPressed(KEY_G)) drawGrid = !drawGrid; 
   if (drawGrid) draw_grid();

   if (IsKeyPressed(KEY_UP)) {
       if (game->world->snakes[0]->direction.y != 1) {
           game->world->snakes[0]->direction.x = 0;
           game->world->snakes[0]->direction.y = -1;
       }
   }
   if (IsKeyPressed(KEY_DOWN)) {
       if (game->world->snakes[0]->direction.y != -1) {
           game->world->snakes[0]->direction.x = 0;
           game->world->snakes[0]->direction.y = 1;
       }
   }

   if (IsKeyPressed(KEY_LEFT)) {
       if (game->world->snakes[0]->direction.x != 1) {
           game->world->snakes[0]->direction.x = -1;
           game->world->snakes[0]->direction.y = 0;
       }
   }
   if (IsKeyPressed(KEY_RIGHT)) {
       if (game->world->snakes[0]->direction.x != -1) {
           game->world->snakes[0]->direction.x = 1;
           game->world->snakes[0]->direction.y = 0;
       }
   }
}
