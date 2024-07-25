#include <SDL2/SDL_events.h>

#include <stdio.h>

#include <stdlib.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#include <string.h>

#define width 1000
#define height 800
#define UI_WIDTH 450

#define GRID_SIZE 15

#define LETTERX 5
#define LETTERY 6

#define DIGITOFFSET 7
#define SYMBOLOFFSET 14

#include <SDL2/SDL.h>

#include <string.h>

#include <stdlib.h>

void renderString(SDL_Renderer * renderer, SDL_Texture * imageTexture, int x, int y, const char * text, float SCALE, float separation) {

    char * textCopy = strdup(text);
    if (textCopy == NULL) {
        return;
    }
    char symbols[] = "?!.,:;\"()'/%-+=<>@#_";
    for (int i = 0; textCopy[i] != '\0'; i++) {
        if (textCopy[i] >= '0' && textCopy[i] <= '9') {
            SDL_Rect srcRect = {
                (LETTERX + 1) * (textCopy[i] - '0'),
                DIGITOFFSET,
                LETTERX,
                LETTERY
            };

            SDL_Rect destRect = {
                x + i * LETTERX * SCALE * separation,
                y,
                LETTERX * SCALE,
                LETTERY * SCALE
            };
            SDL_RenderCopy(renderer, imageTexture, & srcRect, & destRect);
        } else if (textCopy[i] >= 'A' && textCopy[i] <= 'Z' || textCopy[i] >= 'a' && textCopy[i] <= 'z') {
            if (textCopy[i] >= 'a' && textCopy[i] <= 'z') {
                textCopy[i] -= 32;
            }
            SDL_Rect srcRect = {
                (LETTERX + 1) * (textCopy[i] - 'A'),
                0,
                LETTERX,
                LETTERY
            };

            SDL_Rect destRect = {
                x + i * LETTERX * SCALE * separation,
                y,
                LETTERX * SCALE,
                LETTERY * SCALE
            };
            SDL_RenderCopy(renderer, imageTexture, & srcRect, & destRect);
        } else {
            for (int j = 0; j < strlen(symbols); j++) {
                if (textCopy[i] == symbols[j]) {
                    SDL_Rect srcRect = {
                        (LETTERX + 1) * j,
                        SYMBOLOFFSET,
                        LETTERX,
                        LETTERY
                    };

                    SDL_Rect destRect = {
                        x + i * LETTERX * SCALE * separation,
                        y,
                        LETTERX * SCALE,
                        LETTERY * SCALE
                    };
                    SDL_RenderCopy(renderer, imageTexture, & srcRect, & destRect);
                }
            }
        }
    }

    free(textCopy);
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

int main(int argc, char ** argv) {
    //==================================================================================================== INITIALIZATIONS
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create SDL window
    SDL_Window * window = SDL_CreateWindow("Conway's game of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width + UI_WIDTH, height, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Load image
    SDL_Surface * imageSurface = IMG_Load("image.png");
    if (!imageSurface) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create texture from surface
    SDL_Texture * imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface); // Free the loaded surface
    if (!imageTexture) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //==================================================================================================== GAME LOOP
    int cells[width / GRID_SIZE][height / GRID_SIZE] = {0};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    short running = 1;
    short placing = 1;
    int generation = 0;

    SDL_Event event;
    int mouseDown = 0; 
    int delay = 1000;
    int speed = 1;

    while (running) {
        while (SDL_PollEvent( & event)) {
            
            if (event.type == SDL_QUIT) {
                running = 0;
            }


            if ((event.type == SDL_MOUSEBUTTONDOWN) && placing) {
                int x, y;
                SDL_GetMouseState( & x, & y);
                printf("Mouse clicked at (%d, %d)\n", x, y);

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
                SDL_GetMouseState( & x, & y);

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

        if (!placing) {
            generation += logic(cells);
            SDL_Delay(delay);

        }

        //==================================================================================================== RENDERING

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the grid
        drawGrid(renderer);

        // Draw filled cells
        drawFilledCells(renderer, cells);

        // Texts 
        float scale = 3;
        char GenerationString[50];

        renderString(renderer, imageTexture, width + 40, 100, "Game of Life", scale * 1.5, 1.3);

        sprintf(GenerationString, "Generation : %d", generation);
        renderString(renderer, imageTexture, width + 40, 200, GenerationString, scale, 1.3);

        if (placing) {
            renderString(renderer, imageTexture, width + 40, height / 2 - 25, "Placing...", scale, 1.3);
        } else {
            renderString(renderer, imageTexture, width + 40, height / 2 - 25, "Running...", scale, 1.3);
        }

        renderString(renderer, imageTexture, width + 40, height / 2, "LMB to place", scale, 1.3);
        renderString(renderer, imageTexture, width + 40, height / 2 + 25, "RMB to remove", scale, 1.3);
        renderString(renderer, imageTexture, width + 40, height / 2 + 50, "SPACE to run", scale, 1.3);
        renderString(renderer, imageTexture, width + 40, height / 2 + 75, "A / D FOR SPEED", scale, 1.3);

        char SpeedString[50];
        sprintf(SpeedString, "Speed : %d", speed);
        renderString(renderer, imageTexture, width + 40, height / 2 + 150, SpeedString, scale, 1.3);

        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    //==================================================================================================== CLEANUP
    // Release resources
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
