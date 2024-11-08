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
void enableSelectionMode(SDL_Renderer * renderer, int mouse_x, int mouse_y, SDL_Event * event);
int saveSchematic(void *sCells, int sW, int sH, Schematic *schematics, int *schemCount, SDL_Renderer *renderer, TTF_Font *font, int index, int textOffset);
void createSchematicPreview(Schematic *schematic, SDL_Renderer *renderer);
void createSchematicRect(Schematic *schematic, int index, int textOffset);
#endif // !SCHEMS_H 

