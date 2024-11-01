#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "../include/constants.h"
#include "../include/UI.h"
#include "../include/buttons.h"
#include "../include/schems.h"

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

    TTF_Font *schemFont = TTF_OpenFont(FONT_PATH, 18);
    if (!schemFont) {
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
    int mouse_x, mouse_y; 

    int text_offset = GRID_MARGIN_X + GRID_WIDTH * GRID_SIZE + 40;


    //Initialize buttons 
    Button play = {
        (SDL_Rect) {GRID_MARGIN_X+50+20 , height + 20 , 50, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "Play",
        IMG_LoadTexture(renderer, "./assets/play.png")
    };

    Button decSpeed = { 
        (SDL_Rect) {GRID_MARGIN_X , height + 20 , 50, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "-",
        NULL
    };

    Button incSpeed = {
        (SDL_Rect) {GRID_MARGIN_X+50+50+20+20 , height + 20 , 50, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "+",
        NULL
    };

    Button statusLabel = {
        (SDL_Rect) {GRID_MARGIN_X + (GRID_WIDTH * GRID_SIZE / 2 - 20 - 200), height + 20, 200, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "Placing...",
        NULL
    };

    Button generationLabel = {
        (SDL_Rect) {GRID_MARGIN_X + (GRID_WIDTH * GRID_SIZE /2 + 20), height + 20, 200, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "Generation : 0",
        NULL
    };

    Button speedLabel = {
        (SDL_Rect) {GRID_MARGIN_X  , height + 80, 150+40, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "Speed : 10",
        NULL
    };

    Button clear = {
        (SDL_Rect) {GRID_MARGIN_X + GRID_WIDTH * GRID_SIZE -190, height + 20, 190, 50},
        (SDL_Color) {0,0, 0, 255},
        (SDL_Color) {255,255,255, 255},
        "Clear",
        NULL
    };
    /////

    ///// schematics 
    
    //load the schematics 
    int schemCount = 0; 
    char **schemFiles = get_txt_files("./schematics", &schemCount); 
    if(schemFiles == NULL) {
        printf("Error loading schematics\n");
        return 1;
    } 

    for(int i = 0; i < schemCount; i++) {
        printf("%s\n", schemFiles[i]);
    }
       
    Schematic schematics[schemCount];

    //concatenate path to ./schematics/ 
    char path[] = "./schematics/"; 
    
    for(int i = 0; i < schemCount; i++) {

        schematics[i].rect = (SDL_Rect) {
            text_offset,
            GRID_MARGIN_Y + i * 50 *2,
            200,
            50};


        char *fullPath = malloc(strlen(path) + strlen(schemFiles[i]) + 1);
        strcpy(fullPath, path);
        strcat(fullPath, schemFiles[i]);
        schematics[i].file = fopen(fullPath, "r");
        schematics[i].name = schemFiles[i];
        schematics[i].preview = NULL;

        loadSchematic(&schematics[i], schemFont, renderer);

        free(fullPath);

    }
    
        
    //////
    
    int indexOfSchemPressed = -1; 

    while (running) {
        // Process events without delay
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            
            SDL_GetMouseState(&mouse_x, &mouse_y);

            if (((event.type == SDL_MOUSEBUTTONDOWN) && placing==1) || (event.type == SDL_MOUSEMOTION && placing==1 && mouseDown)) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if(x >= GRID_MARGIN_X && x < GRID_MARGIN_X + GRID_WIDTH * grid_size && y >= GRID_MARGIN_Y && y < GRID_MARGIN_Y + GRID_HEIGHT * grid_size) { //in bound
                    int rx = (x - GRID_MARGIN_X) / grid_size;
                    int ry = (y - GRID_MARGIN_Y) / grid_size;

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        cells[rx][ry] = 1;
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        cells[rx][ry] = 0;
                    }
                    mouseDown = 1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                mouseDown = 0;
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

            if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(isHovered(&play, mouse_x, mouse_y)) {
                    placing = !placing;

                }

                else if(isHovered(&decSpeed, mouse_x, mouse_y)) {
                    if (speed > 1) {
                        speed--;
                        delay = 1000 / speed;
                    }
                } 
                else if(isHovered(&incSpeed, mouse_x, mouse_y)) {
                    if (speed < 30) {
                        speed++;
                        delay = 1000 / speed;
                    }
                }
                else if(isHovered(&clear, mouse_x, mouse_y)) {
                    memset(cells, 0, sizeof(cells));
                    generation = 0;
                    placing = 1;
                }
                
                for(int i = 0; i < schemCount; i++) {
                    if(isSchemHovered(&schematics[i], mouse_x, mouse_y)) {
                        placing = 2; 
                        indexOfSchemPressed = i;
                    }
                }

                
                if (placing == 2) {
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        placing = 1;
                        break;
                    }
                    if (mouse_x < GRID_MARGIN_X || mouse_x >= GRID_MARGIN_X + GRID_WIDTH * grid_size ||
                        mouse_y < GRID_MARGIN_Y || mouse_y >= GRID_MARGIN_Y + GRID_HEIGHT * grid_size) {
                        break;
                    }

                    int preview_x = (mouse_x - GRID_MARGIN_X) / grid_size;
                    int preview_y = (mouse_y - GRID_MARGIN_Y) / grid_size;
                    
                    if (preview_x < 0 || preview_x >= GRID_WIDTH || preview_y < 0 || preview_y >= GRID_HEIGHT) {
                        break;
                    }
                    for(int i = 0; i < schematics[indexOfSchemPressed].sW; i++) {
                        for(int j = 0; j < schematics[indexOfSchemPressed].sH; j++) {
                            if(schematics[indexOfSchemPressed].cells[j][i] == 1) {
                                cells[preview_x + i][preview_y + j] = 1;
                            }
                        }
                    }
                    
                }

                




            }

        }

        if (!placing && SDL_GetTicks() - lastUpdateTime > delay) {
            generation += logic(cells);
            lastUpdateTime = SDL_GetTicks();
        }

        if(placing){
             play.texture = IMG_LoadTexture(renderer, "./assets/play.png");
             statusLabel.text = "Placing...";
        }
        else {
            play.texture = IMG_LoadTexture(renderer, "./assets/pause.png");
            statusLabel.text = "Running...";
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        drawFilledCells(renderer, cells);


        renderString(renderer, font, GRID_MARGIN_X, GRID_MARGIN_Y/2, "Conway's Game of Life");

        char GenerationString[50];
        sprintf(GenerationString, "Generation : %d", generation);
        generationLabel.text = GenerationString;

        char SpeedString[50];
        sprintf(SpeedString, "Speed : %d", speed);
        speedLabel.text = SpeedString;


       
       //new button 
        drawButton(renderer, &play, font);
        drawButton(renderer, &decSpeed, font);
        drawButton(renderer, &incSpeed, font);
        drawButton(renderer, &clear, font);
        //fake labels
        drawButton(renderer, &statusLabel, font);
        drawButton(renderer, &generationLabel, font); 
        drawButton(renderer, &speedLabel, font);
    
        //draw schematic 
        for(int i = 0; i < schemCount; i++) {
            drawSchematic(renderer, &schematics[i],schemFont);
        }

                // if (placing == 2) {
                //     int preview_x = (mouse_x - GRID_MARGIN_X) / grid_size;
                //     int preview_y = (mouse_y - GRID_MARGIN_Y) / grid_size;
                //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);  // Set shadow color
                //
                //     // Draw shadow of schematic at mouse position
                //     for (int i = 0; i < 4; i++) {
                //         for (int j = 0; j < 4; j++) {
                //             if (schematic1.cells[j][i] == 1) {
                //                 SDL_Rect cell = {
                //                     GRID_MARGIN_X + (preview_x + i) * grid_size,
                //                     GRID_MARGIN_Y + (preview_y + j) * grid_size,
                //                     grid_size,
                //                     grid_size
                //                 };
                //                 SDL_RenderFillRect(renderer, &cell);
                //             }
                //         }
                //     }
                // }
        if(placing == 2) {
            placeSchematic(renderer, &schematics[indexOfSchemPressed], mouse_x, mouse_y, placing);
        }

        SDL_RenderPresent(renderer);
    }



    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    TTF_CloseFont(font);
    TTF_CloseFont(schemFont);
    TTF_Quit();
    return 0;
}
