# Conways Game of Life, made in C
This is a GUI representation of Conway's game of life made in C with the SDL2 library

## Description

By selecting the grid cells with the mouse you can spawn live cells, to remove them use RMB.\
To start or pause the simulation use the SPACEBAR.\
By using A and D you can change the speed.


You can also use Q to quit the program.

## Compilation

To install the library you can follow SDL's [wiki](https://wiki.libsdl.org/SDL2/Installation).

To compile and run the program using gcc

```bash
gcc main.c -o main -lSDL2main -lSDL2 -lSDL2_image && ./main
```

