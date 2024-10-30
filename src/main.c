#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "../include/constants.h"
#include "../include/UI.h"


int countNeighbours(int x, int y, int grid[GRID_WIDTH][GRID_HEIGHT]) {
    int n = 0;
    int neighborOffsets[8][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0 },          {1, 0 },
        {-1, 1 }, {0, 1 }, {1, 1 }
    };

    for (int i = 0; i < 8; i++) {
        int newX = x + neighborOffsets[i][0];
        int newY = y + neighborOffsets[i][1];

        if (newX >= 0 && newX < GRID_WIDTH && newY >= 0 && newY < GRID_HEIGHT) {
            if (grid[newX][newY] == 1) {
                n++;
            }
        }
    }

    return n;
}

int logic(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    int newGrid[GRID_WIDTH][GRID_HEIGHT] = {0};
    int moved = 0;

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            int neighbors = countNeighbours(i, j, grid);

            if (grid[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    newGrid[i][j] = 0;
                    moved = 1;
                } else {
                    newGrid[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    newGrid[i][j] = 1;
                    moved = 1;
                }
            }
        }
    }

    memcpy(grid, newGrid, sizeof(newGrid));
    return moved;
}

int main(int argc, char **argv) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window and renderer
    //make fullscreen 
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int cells[GRID_WIDTH][GRID_HEIGHT] = {0};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    short running = 1;
    short placing = 1;
    int generation = 0;

    SDL_Event event;
    int mouseDown = 0;
    int speed = 10;
    int delay = 1000 / speed;

    Uint32 lastUpdateTime = SDL_GetTicks();
    
    int grid_size = GRID_SIZE;
    
    int zoom_x = GRID_WIDTH / 2; 
    int zoom_y = GRID_HEIGHT / 2;
    while (running) {
        // Process events without delay
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if ((event.type == SDL_MOUSEBUTTONDOWN) && placing) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int rx = (x - GRID_MARGIN_X) / grid_size;
                int ry = (y - GRID_MARGIN_Y) / grid_size;

                if (event.button.button == SDL_BUTTON_LEFT) {
                    cells[rx][ry] = 1;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    cells[rx][ry] = 0;
                }
                mouseDown = 1;
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                mouseDown = 0;
            }

            if (event.type == SDL_MOUSEMOTION && placing && mouseDown) {
                int x, y;
                SDL_GetMouseState(&x, &y);
               
                int rx = (x - GRID_MARGIN_X) / grid_size;
                int ry = (y - GRID_MARGIN_Y) / grid_size;

                if(rx >= 0 && rx < GRID_WIDTH && ry >= 0 && ry < GRID_HEIGHT) {
                    if (event.motion.state & SDL_BUTTON_LMASK) {
                        cells[rx][ry] = 1;
                    } else if (event.motion.state & SDL_BUTTON_RMASK) {
                        cells[rx][ry] = 0;
                    }
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        break;
                    case SDLK_SPACE:
                        placing = !placing;
                        break;
                    case SDLK_a:
                        if (speed > 1) {
                            speed--;
                            delay = 1000 / speed;
                        }
                        break;
                    case SDLK_d:
                        if (speed < 30) {
                            speed++;
                            delay = 1000 / speed;
                        }
                        break;
                }
            }
            if(event.type == SDL_MOUSEWHEEL){
                if(event.wheel.y > 0){
                    if(grid_size < 10){
                        grid_size += ZOOP_STEP;
                    }
                }else{
                    if(grid_size > 1){
                        grid_size -= ZOOP_STEP;
                    }
                }
            
                SDL_GetMouseState(&zoom_x, &zoom_y);
                zoom_x = (zoom_x - GRID_MARGIN_X) / grid_size;
                zoom_y = (zoom_y - GRID_MARGIN_Y) / grid_size;
            }
        }

        if (!placing && SDL_GetTicks() - lastUpdateTime > delay) {
            generation += logic(cells);
            lastUpdateTime = SDL_GetTicks();
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer,grid_size);
        drawFilledCells(renderer, cells,grid_size);

        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, 100, "Game of Life");

        char GenerationString[50];
        sprintf(GenerationString, "Generation : %d", generation);
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, 200, GenerationString);

        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2 - 25, placing ? "Placing..." : "Running...");
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2, "LMB to place");
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2 + 25, "RMB to erase");
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2 + 100, "Space to toggle");
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2 + 200, "A/D to change speed");

        char SpeedString[50];
        sprintf(SpeedString, "Speed : %d", speed);
        renderString(renderer, font, GRID_MARGIN_X + GRID_SPACE_X + 40, height / 2 + 150, SpeedString);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}
