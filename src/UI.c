#include "../include/constants.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include "../include/UI.h"
#include "../include/schems.h"


void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawGrid(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 255 / 5, 255 / 5, 255 / 5, 100);
    //draw grid_width by grid_height grid 
    for (int i = 0; i < GRID_WIDTH; ++i) {
        SDL_RenderDrawLine(renderer, GRID_MARGIN_X + i * GRID_SIZE, GRID_MARGIN_Y, GRID_MARGIN_X + i * GRID_SIZE, GRID_MARGIN_Y + GRID_HEIGHT * GRID_SIZE);
    }
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(renderer, GRID_MARGIN_X, GRID_MARGIN_Y + i * GRID_SIZE, GRID_MARGIN_X + GRID_WIDTH * GRID_SIZE, GRID_MARGIN_Y + i * GRID_SIZE);
    }

    // Draw border 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect( renderer, &(SDL_Rect){GRID_MARGIN_X, GRID_MARGIN_Y, GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE});
}

void drawFilledCells(SDL_Renderer * renderer, int cells[GRID_WIDTH][GRID_HEIGHT]) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (cells[i][j]) {
                SDL_RenderFillRect(renderer, &(SDL_Rect){GRID_MARGIN_X + i * GRID_SIZE, GRID_MARGIN_Y + j * GRID_SIZE, GRID_SIZE, GRID_SIZE});
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
        //draw it in the middle slighlty smaller than the button         
        SDL_Rect rect = {button->rect.x + button->rect.w / 2 - button->rect.w / 4, button->rect.y + button->rect.h / 2 - button->rect.h / 4, button->rect.w / 2, button->rect.h / 2};
        SDL_RenderCopy(renderer, button->texture, NULL, &rect);

    }

    else {
        //draw it in the center 
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



void drawTextField(TextField *txt, SDL_Renderer * renderer, TTF_Font * font) {
    txt->rect.w = 200;
    txt->rect.h = 50;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &txt->rect);

    int text_width, text_height;
    TTF_SizeText(font, txt->text, &text_width, &text_height);


    renderString(renderer, font, txt->rect.x + txt->rect.w / 2 - text_width / 2, txt->rect.y + txt->rect.h / 2 - text_height / 2, txt->text);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &txt->rect);
}

void drawOverlayedLabel(SDL_Renderer * renderer, OverlayedLabel * label, TTF_Font * font) {
    if(label->active == 0) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, label->bg.r, label->bg.g, label->bg.b, label->bg.a); 
    SDL_RenderFillRect(renderer, &label->rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderDrawRect(renderer, &label->rect);
    if(label->txt) {
        // move textfield to the middle 
        label->txt->rect.x = label->rect.x + label->rect.w / 2 - label->txt->rect.w / 2;
        label->txt->rect.y = label->rect.y + 50;

        drawTextField(label->txt, renderer, font);
    }
    if(label->btn1) {
        //move button to the bottom left 
        label->btn1->rect.x = label->rect.x + 10;
        label->btn1->rect.y = label->rect.y + label->rect.h - 60;

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

        // Handle text input with delay for held keys
        if (event->type == SDL_KEYDOWN) {
            Uint32 currentTime = SDL_GetTicks();
            char add = '\0'; // Default empty char

            // Process backspace
            if (event->key.keysym.sym == SDLK_BACKSPACE) {
                if (strlen(textField->text) > 1) {
                    textField->text[strlen(textField->text) - 1] = '\0';
                }
                lastKeyPressTime = currentTime; // Reset the timer for backspace
            } else {
                // If it's a new key press or we meet the repeat delay
                if (currentTime - lastKeyPressTime >= repeatDelay || lastKeyPressTime == 0) {
                    lastKeyPressTime = currentTime; // Update last key press time

                    // Handle alphanumeric and space characters
                    SDL_Keycode key = event->key.keysym.sym;
                    if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
                        if (key == SDLK_SPACE) {
                            add = ' ';
                        } else if (key >= SDLK_a && key <= SDLK_z) {
                            add = 'a' + (key - SDLK_a); // Maps SDLK_a to 'a', SDLK_b to 'b', etc.
                        } else if (key >= SDLK_0 && key <= SDLK_9) {
                            add = '0' + (key - SDLK_0); // Maps SDLK_0 to '0', SDLK_1 to '1', etc.
                        }

                    }else switch (key) {
                        case SDLK_MINUS: add = '-'; break;
                        case SDLK_UNDERSCORE: add = '_'; break;
                        case SDLK_PERIOD: add = '.'; break;
                    }
                    

                    // Add char to field if valid
                    int len = strlen(textField->text);
                    if (add != '\0' && len < sizeof(textField->text) - 1) {
                        textField->text[len] = add;
                        textField->text[len + 1] = '\0';
                    }
                }
            }
        } else if (event->type == SDL_KEYUP) {
            lastKeyPressTime = 0; // Reset the delay timer on key release
        }
    }
}

