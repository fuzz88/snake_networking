#include "raylib.h"


#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define CELL_WIDHT      20
#define CELL_HEIGHT     20

#define BG_COLOR        0x131313FF
#define GRID_COLOR      0xAFAFAFFF

void draw_grid(){
    Color line_color = (Color)GetColor(GRID_COLOR);
    for (int cell_x = 0; cell_x < SCREEN_WIDTH / CELL_WIDHT; ++cell_x) {
        DrawLine(cell_x * CELL_WIDHT, 0, cell_x * CELL_WIDHT, SCREEN_HEIGHT, line_color);
    }
    for (int cell_y = 0; cell_y < SCREEN_HEIGHT / CELL_HEIGHT; ++cell_y) {
        DrawLine(0, cell_y * CELL_HEIGHT, SCREEN_WIDTH, cell_y * CELL_HEIGHT, line_color);                
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake network");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color)GetColor(BG_COLOR));
        draw_grid();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
