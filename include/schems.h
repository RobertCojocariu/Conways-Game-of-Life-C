#ifndef SCHEMS_H 
#define SCHEMS_H 
#include "constants.h"
#include <dirent.h>
typedef struct Schematic {
    SDL_Rect rect; 
    FILE * file;
    SDL_Texture * preview;
    int **cells;
    char * name;
    int sW; 
    int sH;
}Schematic;
char **get_txt_files(const char *directory_path, int *count);
void placeSchematic(SDL_Renderer * renderer, Schematic * schematic,  int mouse_x, int mouse_y, int placing);
int loadSchematic(Schematic* schematic, TTF_Font* font, SDL_Renderer* renderer);
#endif // !SCHEMS_H 

