#ifndef UI_H
#define UI_H 
#include "constants.h"
#include "buttons.h"
#include "schems.h"


void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text);

void drawGrid(SDL_Renderer * renderer); 

void drawFilledCells(SDL_Renderer *renderer, int cells[GRID_WIDTH][GRID_HEIGHT]);

void drawButton(SDL_Renderer * renderer, Button * button, TTF_Font * font);

int isHovered(Button * button, int x, int y);

void drawSchematic(SDL_Renderer * renderer, Schematic * schematic, TTF_Font * font);
void loadSchematic(Schematic * schematic, TTF_Font * font, SDL_Renderer * renderer);
int isSchemHovered(Schematic * schematic, int x, int y);
#endif // !UI_H


