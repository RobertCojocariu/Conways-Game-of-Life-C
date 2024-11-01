#include "../include/constants.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>
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

int isSchemHovered(Schematic * schematic, int x, int y) {
    return x >= schematic->rect.x && x <= schematic->rect.x + schematic->rect.w && y >= schematic->rect.y && y <= schematic->rect.y + schematic->rect.h;
}



void drawSchematic(SDL_Renderer* renderer, Schematic* schematic, TTF_Font* font) {
    // Set fixed size for the schematic card
    schematic->rect.w = 100;
    schematic->rect.h = 100;

    // Background color for the card
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &schematic->rect);

    // Padding for preview area
    int padding = 10;
    SDL_Rect previewRect;
    previewRect.x = schematic->rect.x + padding;
    previewRect.y = schematic->rect.y + padding;

    // Aspect-ratio calculations
    int max_preview_width = schematic->rect.w - 2 * padding;
    int max_preview_height = (schematic->rect.h / 2) - padding;
    int original_width, original_height;
    SDL_QueryTexture(schematic->preview, NULL, NULL, &original_width, &original_height);

    float aspect_ratio = (float)original_width / original_height;
    if (aspect_ratio > 1) {
        // Image is wider, scale based on width
        previewRect.w = max_preview_width;
        previewRect.h = max_preview_width / aspect_ratio;
    } else {
        // Image is taller, scale based on height
        previewRect.h = max_preview_height;
        previewRect.w = max_preview_height * aspect_ratio;
    }

    // Center preview within the top half of the card
    previewRect.x = schematic->rect.x + (schematic->rect.w - previewRect.w) / 2;
    previewRect.y = schematic->rect.y + padding;

    // Render the preview image
    SDL_RenderCopy(renderer, schematic->preview, NULL, &previewRect);

    // Draw the border around the schematic card
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &schematic->rect);

    // Render the name centered in the bottom half
    int text_width, text_height;
    TTF_SizeText(font, schematic->name, &text_width, &text_height);
    SDL_Rect nameRect = {
        schematic->rect.x + (schematic->rect.w - text_width) / 2,
        schematic->rect.y + schematic->rect.h - padding - text_height,
        text_width,
        text_height
    };

    // Render the name
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, schematic->name, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &nameRect);
    SDL_DestroyTexture(textTexture);
}

void loadSchematic(Schematic* schematic, TTF_Font* font, SDL_Renderer* renderer) {
    // Read and store the schematic name
    char name[50];
    fscanf(schematic->file, "%s", name);
    schematic->name = strdup(name);

    // Read schematic width and height, and initialize rect
    int schemWidth, schemHeight;
    fscanf(schematic->file, "%d %d", &schemWidth, &schemHeight);
    schematic->rect.w = schemWidth * 10;  // Example: cell size 10
    schematic->rect.h = schemHeight * 10;
    
    schematic->sW = schemWidth; 
    schematic->sH = schemHeight;
    // Allocate memory for cells
    schematic->cells = (int**)malloc(schemHeight * sizeof(int*));
    for (int i = 0; i < schemHeight; i++) {
        schematic->cells[i] = (int*)malloc(schemWidth * sizeof(int));
    }

    // Load cell data from the file
    for (int i = 0; i < schemHeight; i++) {
        for (int j = 0; j < schemWidth; j++) {
            fscanf(schematic->file, "%1d", &schematic->cells[i][j]);
        }
    }

    // Optional: Create a preview texture based on loaded cells
    SDL_Surface* surface = SDL_CreateRGBSurface(0, schematic->rect.w, schematic->rect.h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        printf("Surface creation error: %s\n", SDL_GetError());
        return;
    }

    // Draw cells onto the surface
    SDL_Rect cellRect = { 0, 0, 10, 10 };  // Each cell is 10x10 pixels
    for (int i = 0; i < schemHeight; i++) {
        for (int j = 0; j < schemWidth; j++) {
            if (schematic->cells[i][j] == 1) {
                cellRect.x = j * 10;
                cellRect.y = i * 10;
                SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 255, 255, 255));  // White cells
            }
        }
    }


    // Convert surface to texture
    schematic->preview = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    fclose(schematic->file);
    

}
