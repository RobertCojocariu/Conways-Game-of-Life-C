#ifndef CONSTANTS_H
#define CONSTANTS_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __APPLE__ //macOS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#define width 1000
#define height 800
#define UI_WIDTH 450

#define GRID_SIZE 10//default size of cell 

#define GRID_WIDTH 120//cells 
#define GRID_HEIGHT 70 //cells


#define GRID_MARGIN_X 100 //offset 
#define GRID_MARGIN_Y 100 //offset

#define FONT_PATH "./PixelFont7-G02A.ttf"

#define ZOOM_STEP 1
#define SCHEM_IN_PAGE 15

#define MAX_SCHEMATICS 100

#define SCALE_X(value) ((int)((value) * scaleX))
#define SCALE_Y(value) ((int)((value) * scaleY))

#define TARGET_ASPECT_RATIO_X 16
#define TARGET_ASPECT_RATIO_Y 9

#define  BASE_FONT_SIZE 24 
#define  BASE_FONT_SIZE_SMALL 18
#endif // ! CONSTANTS
