#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define width 1000
#define height 800
#define UI_WIDTH 450

#define GRID_SIZE 15

#define LETTERX 5
#define LETTERY 6

#define DIGITOFFSET 7
#define SYMBOLOFFSET 14

#define FONT_PATH "./PixelFont7-G02A.ttf"

#include <string.h>

#include <stdlib.h>

void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text) {
    SDL_Color color = {
        255,
        255,
        255,
        255
    };
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {
        x,
        y,
        surface -> w,
        surface -> h
    };

    SDL_RenderCopy(renderer, texture, NULL, & rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawGrid(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 255 / 5, 255 / 5, 255 / 5, 100);
    for (int i = 0; i < width; i += GRID_SIZE) {
        SDL_RenderDrawLine(renderer, i, 0, i, height);
    }
    for (int i = 0; i < height; i += GRID_SIZE) {
        SDL_RenderDrawLine(renderer, 0, i, width, i);
    }
}

void drawFilledCells(SDL_Renderer * renderer, int cells[width / GRID_SIZE][height / GRID_SIZE]) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < width / GRID_SIZE; ++i) {
        for (int j = 0; j < height / GRID_SIZE; ++j) {
            if (cells[i][j]) {
                SDL_Rect rect = {
                    i * GRID_SIZE,
                    j * GRID_SIZE,
                    GRID_SIZE,
                    GRID_SIZE
                };
                SDL_RenderFillRect(renderer, & rect);
            }
        }
    }
}

int countNeighbours(int x, int y, int grid[width / GRID_SIZE][height / GRID_SIZE]) {
    int n = 0;
    int neighborOffsets[8][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0 },          {1, 0 },
        {-1, 1 }, {0, 1 }, {1, 1 }
    };

    for (int i = 0; i < 8; i++) {
        int newX = x + neighborOffsets[i][0];
        int newY = y + neighborOffsets[i][1];

        if (newX >= 0 && newX < width / GRID_SIZE && newY >= 0 && newY < height / GRID_SIZE) {
            if (grid[newX][newY] == 1) {
                n++;
            }
        }
    }

    return n;
}

int logic(int grid[width / GRID_SIZE][height / GRID_SIZE]) {
    int newGrid[width / GRID_SIZE][height / GRID_SIZE] = {
        0
    };
    int moved = 0;

    for (int i = 0; i < width / GRID_SIZE; i++) {
        for (int j = 0; j < height / GRID_SIZE; j++) {
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
    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width + UI_WIDTH, height, SDL_WINDOW_OPENGL);
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

    int cells[width / GRID_SIZE][height / GRID_SIZE] = {0};
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

    while (running) {
        // Process events without delay
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if ((event.type == SDL_MOUSEBUTTONDOWN) && placing) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int rx = x / GRID_SIZE;
                int ry = y / GRID_SIZE;

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
                int rx = x / GRID_SIZE;
                int ry = y / GRID_SIZE;

                if (event.motion.state & SDL_BUTTON_LMASK) {
                    cells[rx][ry] = 1;
                } else if (event.motion.state & SDL_BUTTON_RMASK) {
                    cells[rx][ry] = 0;
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
        }

        if (!placing && SDL_GetTicks() - lastUpdateTime > delay) {
            generation += logic(cells);
            lastUpdateTime = SDL_GetTicks();
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        drawFilledCells(renderer, cells);

        renderString(renderer, font, width + 40, 100, "Game of Life");

        char GenerationString[50];
        sprintf(GenerationString, "Generation : %d", generation);
        renderString(renderer, font, width + 40, 200, GenerationString);

        renderString(renderer, font, width + 40, height / 2 - 25, placing ? "Placing..." : "Running...");
        renderString(renderer, font, width + 40, height / 2, "LMB to place");
        renderString(renderer, font, width + 40, height / 2 + 50, "RMB to remove");
        renderString(renderer, font, width + 40, height / 2 + 100, "Space to toggle");
        renderString(renderer, font, width + 40, height / 2 + 200, "A/D to change speed");

        char SpeedString[50];
        sprintf(SpeedString, "Speed : %d", speed);
        renderString(renderer, font, width + 40, height / 2 + 150, SpeedString);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
