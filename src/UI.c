#include "../include/constants.h"
#include <stdio.h>
#include "../include/UI.h"
#include "../include/schems.h"


void CalculateNearest16_9Resolution(int screenWidth, int screenHeight, int* targetWidth, int* targetHeight) {
    // Calculate the maximum width and height for 16:9
    int max16_9Width = screenWidth;
    int max16_9Height = (max16_9Width * 9) / 16;

    // If the height exceeds the screen height, adjust the width
    if (max16_9Height > screenHeight) {
        max16_9Height = screenHeight;
        max16_9Width = (max16_9Height * 16) / 9;
    }

    // Set the calculated width and height
    *targetWidth = max16_9Width;
    *targetHeight = max16_9Height;
}

void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void drawGrid(SDL_Renderer * renderer, int windowWidth, int windowHeight) {
    // Calculate scaling factors
    float scaleX = windowWidth / 1920.0f;
    float scaleY = windowHeight / 1080.0f;
    
    int scaledGridSizeX = GRID_SIZE * scaleX;
    int scaledGridSizeY = GRID_SIZE * scaleY;
    int scaledGridMarginX = GRID_MARGIN_X * scaleX;
    int scaledGridMarginY = GRID_MARGIN_Y * scaleY;
    int scaledGridWidth = GRID_WIDTH * scaledGridSizeX;
    int scaledGridHeight = GRID_HEIGHT * scaledGridSizeY;

    SDL_SetRenderDrawColor(renderer, 255 / 5, 255 / 5, 255 / 5, 100);
    
    // Draw vertical lines
    for (int i = 0; i < GRID_WIDTH; ++i) {
        SDL_RenderDrawLine(renderer,
            scaledGridMarginX + i * scaledGridSizeX,
            scaledGridMarginY,
            scaledGridMarginX + i * scaledGridSizeX,
            scaledGridMarginY + scaledGridHeight
        );
    }

    // Draw horizontal lines
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(renderer,
            scaledGridMarginX,
            scaledGridMarginY + i * scaledGridSizeY,
            scaledGridMarginX + scaledGridWidth,
            scaledGridMarginY + i * scaledGridSizeY
        );
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &(SDL_Rect){
        scaledGridMarginX,
        scaledGridMarginY,
        scaledGridWidth,
        scaledGridHeight
    });
}

void drawFilledCells(SDL_Renderer * renderer, int cells[GRID_WIDTH][GRID_HEIGHT], int windowWidth, int windowHeight) {
    // Calculate scaling factors
    float scaleX = windowWidth / 1920.0f;
    float scaleY = windowHeight / 1080.0f;

    int scaledGridSizeX = GRID_SIZE * scaleX;
    int scaledGridSizeY = GRID_SIZE * scaleY;
    int scaledGridMarginX = GRID_MARGIN_X * scaleX;
    int scaledGridMarginY = GRID_MARGIN_Y * scaleY;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (cells[i][j]) {
                SDL_Rect cellRect = {
                    scaledGridMarginX + i * scaledGridSizeX,
                    scaledGridMarginY + j * scaledGridSizeY,
                    scaledGridSizeX,
                    scaledGridSizeY
                };
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }
}


void drawButton(SDL_Renderer * renderer, Button * button, TTF_Font * font) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_SetRenderDrawColor(renderer, button->borderColor.r, button->borderColor.g, button->borderColor.b, button->borderColor.a); 
    SDL_RenderDrawRect(renderer, &button->rect);
    if(button->texture) {
        SDL_Rect rect = {button->rect.x + button->rect.w / 2 - button->rect.w / 4, button->rect.y + button->rect.h / 2 - button->rect.h / 4, button->rect.w / 2, button->rect.h / 2};
        SDL_RenderCopy(renderer, button->texture, NULL, &rect);

    }

    else { // render text if no texture
        int text_width, text_height; 
        TTF_SizeText(font, button->text, &text_width, &text_height);
        renderString(renderer, font, button->rect.x + button->rect.w / 2 - text_width / 2, button->rect.y + button->rect.h / 2 - text_height / 2, button->text);
        
    }

}

int isHovered(Button * button, int x, int y) {
    return x >= button->rect.x && x <= button->rect.x + button->rect.w && y >= button->rect.y && y <= button->rect.y + button->rect.h;
}


int isTextFieldHovered(TextField *txt, int x, int y) {
    return x >= txt->rect.x && x <= txt->rect.x + txt->rect.w && y >= txt->rect.y && y <= txt->rect.y + txt->rect.h;
}



void drawTextField(TextField *txt, SDL_Renderer * renderer, TTF_Font * font, int scaleX, int scaleY) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &txt->rect);

    int text_width, text_height;
    TTF_SizeText(font, txt->text, &text_width, &text_height);


    renderString(renderer, font, txt->rect.x + txt->rect.w / 2 - text_width / 2, txt->rect.y + txt->rect.h / 2 - text_height / 2, txt->text);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &txt->rect);
}

void drawOverlayedLabel(SDL_Renderer * renderer, OverlayedLabel * label, TTF_Font * font, float scaleX, float scaleY) { 
    if(label->active == 0) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, label->bg.r, label->bg.g, label->bg.b, label->bg.a); 
    SDL_RenderFillRect(renderer, &label->rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderDrawRect(renderer, &label->rect);
    if(label->txt) {
        // move textfield to the middle 
        // label->txt->rect.x = label->rect.x + label->rect.w / 2 - label->txt->rect.w / 2;
        label->txt->rect.x =(label->rect.x + label->rect.w / 2 - label->txt->rect.w / 2);
        label->txt->rect.y =label->rect.y + SCALE_Y(50);

        drawTextField(label->txt, renderer, font, scaleX, scaleY);
    }
    if(label->btn1) {
        if(label->txt == NULL && label->btn2 == NULL) { // if there is only one button
            //move button to the middle 
            label->btn1->rect.x = label->rect.x + label->rect.w / 2 - label->btn1->rect.w / 2;
            label->btn1->rect.y = label->rect.y + label->rect.h - 60;
        }
        else {
        //move button to the bottom left 
        label->btn1->rect.x = label->rect.x + 10;
        label->btn1->rect.y = label->rect.y + label->rect.h - 60;
        }
        drawButton(renderer, label->btn1, font);
        
    }
    if(label->btn2) {
        //move button to the bottom right 
        label->btn2->rect.x = label->rect.x + label->rect.w - 10 - label->btn2->rect.w; 
        label->btn2->rect.y = label->rect.y + label->rect.h - 60;

        drawButton(renderer, label->btn2, font);
    }
    if(label->text) {
        //in the top middle 
        int text_width, text_height;
        TTF_SizeText(font, label->text, &text_width, &text_height);
        renderString(renderer, font, label->rect.x + label->rect.w / 2 - text_width / 2, label->rect.y + 10, label->text);
    }

}



void handleTextField(TextField *textField, SDL_Event *event, Uint32 repeatDelay, Uint32 lastKeyPressTime) {
    if (textField->focused) {

        if (event->type == SDL_KEYDOWN) {
            Uint32 currentTime = SDL_GetTicks();
            char add = '\0';

            if (event->key.keysym.sym == SDLK_BACKSPACE) { // delete key
                if (strlen(textField->text) > 1) {
                    textField->text[strlen(textField->text) - 1] = '\0';
                }
                lastKeyPressTime = currentTime; 
            } else {
                if (currentTime - lastKeyPressTime >= repeatDelay || lastKeyPressTime == 0) {
                    lastKeyPressTime = currentTime; 

                    // alphanumeric input
                    SDL_Keycode key = event->key.keysym.sym;
                    if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
                        // if (key == SDLK_SPACE) {
                        //     add = ' ';
                        if (key >= SDLK_a && key <= SDLK_z) {
                            add = 'a' + (key - SDLK_a); // maps SDLK_a to 'a', SDLK_b to 'b', ... 
                        } else if (key >= SDLK_0 && key <= SDLK_9) {
                            add = '0' + (key - SDLK_0); // maps SDLK_0 to '0', SDLK_1 to '1', ...
                        }

                    }else switch (key) { //other keys
                        case SDLK_MINUS: add = '-'; break; 
                        case SDLK_UNDERSCORE: add = '_'; break;
                        case SDLK_PERIOD: add = '.'; break;
                    }
                    

                    
                    int len = strlen(textField->text);
                    if (add != '\0' && len < sizeof(textField->text) - 1) {
                        textField->text[len] = add;
                        textField->text[len + 1] = '\0';
                    }
                }
            }
        } else if (event->type == SDL_KEYUP) {
            lastKeyPressTime = 0;
        }
    }
}

void drawTutorial(SDL_Renderer * renderer, OverlayedLabel *lbl, TTF_Font * font, float scaleX, float scaleY) { 
    if(lbl->active == 0) {
        return;
    }
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 80, "1. Left click on the grid to place cells");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 110, "2. Right click on the grid to remove cells");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 140, "3. SPACE or buttons to start/stop simulation");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 170, "4. A / D or buttons to change the speed");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 200, "5. Click on a schematic to select it");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 230, "6. Use the ~ button to enter schematic selection mode");
    // renderString(renderer, font, lbl->rect.x + 230, lbl->rect.y + 260, "7. Use the ? button to open this menu");
    //draw them centerd and scaled 
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(80), "1. Left click on the grid to place cells");
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(110), "2. Right click on the grid to remove cells");
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(140), "3. SPACE or buttons to start/stop simulation"); 
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(170), "4. A / D or buttons to change the speed"); 
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(200), "5. Click on a schematic to select it");
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(230), "6. Use the ~ button to enter schematic selection mode");
    renderString(renderer, font, lbl->rect.x + SCALE_X(230), lbl->rect.y + SCALE_Y(260), "7. Use the ? button to open this menu");



}
