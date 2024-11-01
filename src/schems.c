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

                // if (placing == 2) {
                //     int preview_x = (mouse_x - GRID_MARGIN_X) / grid_size;
                //     int preview_y = (mouse_y - GRID_MARGIN_Y) / grid_size;
                //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);  // Set shadow color
                //
                //     // Draw shadow of schematic at mouse position
                //     for (int i = 0; i < 4; i++) {
                //         for (int j = 0; j < 4; j++) {
                //             if (schematic1.cells[j][i] == 1) {
                //                 SDL_Rect cell = {
                //                     GRID_MARGIN_X + (preview_x + i) * grid_size,
                //                     GRID_MARGIN_Y + (preview_y + j) * grid_size,
                //                     grid_size,
                //                     grid_size
                //                 };
                //                 SDL_RenderFillRect(renderer, &cell);
                //             }
                //         }
                //     }
                // }
//the caps words are already defined in constants.h
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
