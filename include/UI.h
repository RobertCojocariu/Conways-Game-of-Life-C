#ifndef UI_H
#define UI_H 
#include "constants.h"
#include "buttons.h"

typedef struct TextField {
    SDL_Rect rect;
    char text[15];
    int focused;


}TextField;

typedef struct OverlayedLabel {
    int active;
    SDL_Rect rect;
    SDL_Color bg;
    char * text;
    TextField * txt;
    Button * btn1;
    Button * btn2;

}OverlayedLabel;



void renderString(SDL_Renderer * renderer, TTF_Font *font, int x, int y, const char * text);
void drawGrid(SDL_Renderer * renderer, int windowWidth, int windowHeight);
void drawFilledCells(SDL_Renderer * renderer, int cells[GRID_WIDTH][GRID_HEIGHT], int windowWidth, int windowHeight);
void drawButton(SDL_Renderer * renderer, Button * button, TTF_Font * font);
int isHovered(Button * button, int x, int y);
int isTextFieldHovered(TextField *txt, int x, int y);
void drawTextField(TextField *txt, SDL_Renderer * renderer, TTF_Font * font, int scaleX, int scaleY);
void drawOverlayedLabel(SDL_Renderer * renderer, OverlayedLabel * label, TTF_Font * font, float scaleX, float scaleY);
void handleTextField(TextField *textField, SDL_Event *event, Uint32 repeatDelay, Uint32 lastKeyPressTime);
void drawTutorial(SDL_Renderer * renderer, OverlayedLabel *lbl, TTF_Font * font, float scaleX, float scaleY);
void CalculateNearest16_9Resolution(int screenWidth, int screenHeight, int* targetWidth, int* targetHeight);

#endif // !UI_H


