#include "networking.h"
#include "drawing.h"
#include "game.h"


int main(void)
{
    Game *game = init_game();
    start_networking(game);
    main_drawing_loop(game);
    destroy_game(game);

    return 0;
}
