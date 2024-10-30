#ifndef UI_H
#define UI_H 
#include "constants.h"

void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text);

void drawGrid(SDL_Renderer * renderer, int grid_size); 


void drawFilledCells(SDL_Renderer * renderer, int cells[GRID_WIDTH][GRID_HEIGHT], int grid_size);


#endif // !UI_H


