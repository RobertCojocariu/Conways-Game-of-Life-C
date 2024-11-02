#include "../include/schems.h"
#include "../include/constants.h"

char **get_txt_files(const char *directory_path, int *count) {
    int capacity = 10;   // Initial capacity for the filenames array
    int txt_file_count = 0;
    struct dirent *entry;
    DIR *dir = opendir(directory_path);

    // Check if the directory can be opened
    if (dir == NULL) {
        perror("Unable to open directory");
        *count = -1;
        return NULL;
    }

    // Allocate initial memory for file names
    char **txt_files = malloc(capacity * sizeof(char *));
    if (txt_files == NULL) {
        perror("Memory allocation failed");
        closedir(dir);
        *count = -1;
        return NULL;
    }

    // Iterate over each file in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file has a .txt extension
        if (entry->d_type == DT_REG) {  // Ensure it's a regular file
            const char *dot = strrchr(entry->d_name, '.');
            if (dot && strcmp(dot, ".txt") == 0) {
                // Expand the array if needed
                if (txt_file_count >= capacity) {
                    capacity *= 2;
                    char **temp = realloc(txt_files, capacity * sizeof(char *));
                    if (temp == NULL) {
                        perror("Memory reallocation failed");
                        closedir(dir);
                        *count = -1;
                        return NULL;
                    }
                    txt_files = temp;
                }

                // Allocate memory for the filename and copy it
                txt_files[txt_file_count] = malloc(strlen(entry->d_name) + 1);
                if (txt_files[txt_file_count] == NULL) {
                    perror("Memory allocation failed for filename");
                    closedir(dir);
                    *count = -1;
                    return NULL;
                }
                strcpy(txt_files[txt_file_count], entry->d_name);
                txt_file_count++;
            }
        }
    }

    closedir(dir);
    *count = txt_file_count;
    return txt_files;
}

void placeSchematic(SDL_Renderer * renderer, Schematic * schematic,  int mouse_x, int mouse_y, int placing) {
    if (placing == 2) {
        int preview_x = (mouse_x - GRID_MARGIN_X) / GRID_SIZE;
        int preview_y = (mouse_y - GRID_MARGIN_Y) / GRID_SIZE;
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);  // Set shadow color

        // Draw shadow of schematic at mouse position, every schem has a different size
    

        for(int i = 0; i < schematic->sW; i++) {
            for(int j = 0; j < schematic->sH; j++) {
                if(schematic->cells[j][i] == 1) {
                    SDL_Rect cell = {
                        GRID_MARGIN_X + (preview_x + i) * GRID_SIZE,
                        GRID_MARGIN_Y + (preview_y + j) * GRID_SIZE,
                        GRID_SIZE,
                        GRID_SIZE
                    };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
    }
}

int loadSchematic(Schematic* schematic, TTF_Font* font, SDL_Renderer* renderer) {
    // Read and store the schematic name
    char name[50];
    if (fscanf(schematic->file, "%49s", name) != 1) {
        printf("Error: Could not read schematic name. Skipping schematic.\n");
        fclose(schematic->file);
        return 1;
    }
    schematic->name = strdup(name);

    // Read schematic width and height, and initialize rect
    int schemWidth, schemHeight;
    if (fscanf(schematic->file, "%d %d", &schemWidth, &schemHeight) != 2 || schemWidth <= 0 || schemHeight <= 0) {
        printf("Error: Invalid schematic dimensions for '%s'. Skipping schematic.\n", schematic->name);
        fclose(schematic->file);
        free(schematic->name);
        return 1;
    }
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
            if (fscanf(schematic->file, "%1d", &schematic->cells[i][j]) != 1) {
                printf("Error: Invalid cell data in '%s'. Skipping schematic.\n", schematic->name);
                fclose(schematic->file);

                // Free allocated memory and return
                for (int k = 0; k <= i; k++) {
                    free(schematic->cells[k]);
                }
                free(schematic->cells);
                free(schematic->name);
                return 1;
            }
        }
    }

    // Create a surface for rendering the schematic preview
    SDL_Surface* surface = SDL_CreateRGBSurface(0, schematic->rect.w, schematic->rect.h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        printf("Error: Surface creation failed for '%s': %s\n", schematic->name, SDL_GetError());
        fclose(schematic->file);
        
        // Free allocated memory
        for (int i = 0; i < schemHeight; i++) {
            free(schematic->cells[i]);
        }
        free(schematic->cells);
        free(schematic->name);
        return 1;
    }

    // Draw cells onto the surface
    SDL_Rect cellRect = { 0, 0, 10, 10 }; 
    for (int i = 0; i < schemHeight; i++) {
        for (int j = 0; j < schemWidth; j++) {
            if (schematic->cells[i][j] == 1) {
                cellRect.x = j * 10;
                cellRect.y = i * 10;
                SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 255, 255, 255));  
            }
        }
    }

    // Convert surface to texture
    schematic->preview = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    fclose(schematic->file);
    return 0;
}
