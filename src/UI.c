#include "../include/constants.h"
#include <SDL2/SDL_render.h>
#include "../include/UI.h"


void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawGrid(SDL_Renderer * renderer, int grid_size) {



    SDL_SetRenderDrawColor(renderer, 255 / 5, 255 / 5, 255 / 5, 100);

    //draw in a GRID_SPACE_X x GRID_SPACE_Y (this size has to remain still) grid as many cells of GRID_SIZE x GRID_SIZE as possible
    int possible_cells_x = GRID_SPACE_X / grid_size; 
    int possible_cells_y = GRID_SPACE_Y / grid_size;

    for (int i = 0; i < possible_cells_x; ++i) {
        SDL_RenderDrawLine(renderer, GRID_MARGIN_X + i * grid_size, GRID_MARGIN_Y, GRID_MARGIN_X + i * grid_size, GRID_MARGIN_Y + GRID_SPACE_Y);
    }
    for (int i = 0; i < possible_cells_y; ++i) {
        SDL_RenderDrawLine(renderer, GRID_MARGIN_X, GRID_MARGIN_Y + i * grid_size, GRID_MARGIN_X + GRID_SPACE_X, GRID_MARGIN_Y + i * grid_size);
    }

    //border 

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderDrawRect(renderer, &(SDL_Rect){GRID_MARGIN_X, GRID_MARGIN_Y, GRID_SPACE_X, GRID_SPACE_Y});



}

void drawFilledCells(SDL_Renderer * renderer, int cells[GRID_WIDTH][GRID_HEIGHT], int grid_size) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int possible_cells_x = GRID_SPACE_X / grid_size;
    int possible_cells_y = GRID_SPACE_Y / grid_size;

    for (int i = 0; i < possible_cells_x; ++i) {
        for (int j = 0; j < possible_cells_y; ++j) {
            if (cells[i][j]) {
                SDL_Rect rect = {GRID_MARGIN_X + i * grid_size, GRID_MARGIN_Y + j * grid_size, grid_size, grid_size};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}
