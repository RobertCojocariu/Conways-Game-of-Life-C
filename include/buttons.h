#ifndef BUTTONS_H
#define BUTTONS_H
#include "constants.h"

typedef struct Button {
    SDL_Rect rect;
    SDL_Color color;
    SDL_Color borderColor;
    char * text;
    SDL_Texture * texture;


}Button;




#endif // !BUTTONS_H
