#include "../include/schems.h"
#include "../include/constants.h"
#include <stdio.h>
int isSchemHovered(Schematic * schematic, int x, int y) {
    return x >= schematic->rect.x && x <= schematic->rect.x + schematic->rect.w && y >= schematic->rect.y && y <= schematic->rect.y + schematic->rect.h;
}
void drawSchematic(SDL_Renderer* renderer, Schematic* schematic, TTF_Font* font, float scaleX, float scaleY) {
    schematic->rect.w = SCALE_X(150); 
    schematic->rect.h = SCALE_Y(100);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &schematic->rect);

    int padding = 10;
    SDL_Rect previewRect;
    previewRect.x = schematic->rect.x + padding;
    previewRect.y = schematic->rect.y + padding;

    // calculations to fit the preview image in the card
    int max_preview_width = schematic->rect.w - 2 * padding;
    int max_preview_height = (schematic->rect.h / 2) - padding;
    int original_width, original_height;
    SDL_QueryTexture(schematic->preview, NULL, NULL, &original_width, &original_height);

    float aspect_ratio = (float)original_width / original_height;
    if (aspect_ratio > 1.5) {
        // image is wider, scale based on width
        previewRect.w = max_preview_width;
        previewRect.h = max_preview_width / aspect_ratio;
    } else {
        // image is taller, scale based on height
        previewRect.h = max_preview_height;
        previewRect.w = max_preview_height * aspect_ratio;
    }

    previewRect.x = schematic->rect.x + (schematic->rect.w - previewRect.w) / 2;
    previewRect.y = schematic->rect.y + padding;

    // render preview image 
    SDL_RenderCopy(renderer, schematic->preview, NULL, &previewRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &schematic->rect);
    
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

    if(strlen(schematic->name) > 17) {
        schematic->name[17] = '\0';
        schematic->name[16] = '.'; 
        schematic->name[15] = '.'; 
        schematic->name[14] = '.';
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, schematic->name, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &nameRect);
    SDL_DestroyTexture(textTexture);
}

char **get_txt_files(const char *directory_path, int *count) { // returns an array of strings containing the names of the .txt files in the directory
    int capacity = 10;   
    int txt_file_count = 0;
    struct dirent *entry;
    DIR *dir = opendir(directory_path);

    if (dir == NULL) {
        perror("Unable to open directory");
        *count = -1;
        return NULL;
    }

    char **txt_files = malloc(capacity * sizeof(char *));
    if (txt_files == NULL) {
        perror("Memory allocation failed");
        closedir(dir);
        *count = -1;
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // DT_REG = regular file
            const char *dot = strrchr(entry->d_name, '.');
            if (dot && strcmp(dot, ".txt") == 0) {
                if (txt_file_count >= capacity) { // if more then capacity, double the capacity
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

                txt_files[txt_file_count] = malloc(strlen(entry->d_name) + 1); // allocate memory for filename
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

void placeSchematic(SDL_Renderer * renderer, Schematic * schematic,  int mouse_x, int mouse_y, int placing, float scaleX, float scaleY)  {
    if (placing == 2) {
        int preview_x =(mouse_x - GRID_MARGIN_X) / GRID_SIZE;
        int preview_y =(mouse_y - GRID_MARGIN_Y) / GRID_SIZE;
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128); 
       
        // shadow of schem under mouse  

        for(int i = 0; i < schematic->sW; i++) {
            for(int j = 0; j < schematic->sH; j++) {
                if(schematic->cells[j][i] == 1) {
                    SDL_Rect cell = {
                        GRID_MARGIN_X + preview_x * GRID_SIZE + i * SCALE_X(GRID_SIZE), 
                        GRID_MARGIN_Y + preview_y * GRID_SIZE + j * SCALE_Y(GRID_SIZE),
                        SCALE_X(GRID_SIZE),
                        SCALE_Y(GRID_SIZE)
                    };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
    }
}

int loadSchematic(Schematic* schematic, TTF_Font* font, SDL_Renderer* renderer) {

    char name[50];
    
    if (fscanf(schematic->file, "%49s", name) != 1) {
        printf("Error: Invalid schematic name. Skipping schematic.\n");
        fclose(schematic->file);
        free(schematic->name);
        return 1;
    }

    schematic->name = malloc(strlen(name) + 1);
    if (schematic->name == NULL) {
        printf("Error: Memory allocation for schematic name failed\n");
        fclose(schematic->file);
        return 1;
    }
    strcpy(schematic->name, name);

    int schemWidth, schemHeight;
    if (fscanf(schematic->file, "%d %d", &schemWidth, &schemHeight) != 2 || schemWidth <= 0 || schemHeight <= 0) {
        printf("Error: Invalid schematic dimensions for '%s'. Skipping schematic.\n", schematic->name);
        fclose(schematic->file);
        free(schematic->name);
        return 1;
    }
    schematic->rect.w = schemWidth * 10;  
    schematic->rect.h = schemHeight * 10;
    
    schematic->sW = schemWidth; 
    schematic->sH = schemHeight;

    // allocate memory for cells
    schematic->cells = malloc(schemHeight * sizeof(int*));
    for (int i = 0; i < schemHeight; i++) {
        schematic->cells[i] = malloc(schemWidth * sizeof(int));
    }

    // load cell from the file
    for (int i = 0; i < schemHeight; i++) {
        for (int j = 0; j < schemWidth; j++) {
            if (fscanf(schematic->file, "%1d", &schematic->cells[i][j]) != 1) {
                printf("Error: Invalid cell data in '%s'. Skipping schematic.\n", schematic->name);
                fclose(schematic->file);

                for (int k = 0; k <= i; k++) {
                    free(schematic->cells[k]);
                }
                free(schematic->cells);
                free(schematic->name);
                return 1;
            }
        }
    }

    //surface for rendering the schematic preview
    createSchematicPreview(schematic, renderer);
    fclose(schematic->file);
    return 0;
}


int saveSchematic(int **sCells, int sW, int sH, Schematic *schematics, int *schemCount, SDL_Renderer *renderer, TTF_Font *font, int index, int textOffset, OverlayedLabel *label, float scaleX, float scaleY) { 

    int **cells = malloc(sH * sizeof(int *));

    for (int i = 0; i < sH; i++) {
        cells[i] = malloc(sW * sizeof(int));
    }

    for (int i = 0; i < sH; i++) {
        for (int j = 0; j < sW; j++) {
            cells[i][j] = sCells[i][j];
        }
    }


    char nameInput[50];
    printf("Enter schematic name: "); 

    //read the name from the text field 

    strcpy(nameInput, label->txt->text);
    printf("Name: %s\n", nameInput);

    Schematic newSchematic;
    newSchematic.name = malloc(strlen(nameInput) + 1);
    if (newSchematic.name == NULL) {
        printf("Error: Memory allocation for schematic name failed\n");
        return 0;
    }
    strcpy(newSchematic.name, nameInput);

    newSchematic.cells = cells;
    newSchematic.sW = sW;
    newSchematic.sH = sH;
    newSchematic.preview = NULL;

    schematics[(*schemCount)++] = newSchematic; // dereference schemCount and increment it

    char path[50]; 
    
    snprintf(path, sizeof(path), "schematics/%s.txt", newSchematic.name);
    newSchematic.file = fopen(path, "w");
    if (!newSchematic.file) {
        printf("Error: Could not open file for saving schematic\n");
        free(newSchematic.name);
        return 0;
    }
    
    // saving data into file 
    fprintf(newSchematic.file, "%s\n", newSchematic.name);
    fprintf(newSchematic.file, "%d %d\n", newSchematic.sW, newSchematic.sH);

    for (int i = 0; i < newSchematic.sH; i++) {
        for (int j = 0; j < newSchematic.sW; j++) {
            fprintf(newSchematic.file, "%d", newSchematic.cells[i][j]);
        }
        fprintf(newSchematic.file, "\n");  
    }

    fclose(newSchematic.file);
    newSchematic.file = fopen(path, "r");
    printf("\nSchematic '%s' saved successfully.\n", newSchematic.name);

    createSchematicRect(&newSchematic, index, textOffset, scaleX, scaleY); 
    createSchematicPreview(&newSchematic, renderer);

    printf("Schematic rect: %d %d %d %d\n", newSchematic.rect.x, newSchematic.rect.y, newSchematic.rect.w, newSchematic.rect.h);
    return 1;
}


void createSchematicRect(Schematic *schematic, int index, int textOffset, float scaleX, float scaleY)  {
    schematic->rect = (SDL_Rect) {
        SCALE_X(textOffset + (index % 3) * 160),
        SCALE_Y(GRID_MARGIN_Y + ((index % SCHEM_IN_PAGE) / 3) * 120 + 50),
        SCALE_X(150), 
        SCALE_Y(100)
    };
    printf("Schematic rect: %d %d %d %d\n", schematic->rect.x, schematic->rect.y, schematic->rect.w, schematic->rect.h);
}



void createSchematicPreview(Schematic *schematic, SDL_Renderer *renderer) {
    int sW = schematic->sW;
    int sH = schematic->sH;

    SDL_Surface* surface = SDL_CreateRGBSurface(0, sW * 10, sH * 10, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        printf("Error: Surface creation failed for '%s': %s\n", schematic->name, SDL_GetError());
        fclose(schematic->file);

        for (int i = 0; i < schematic->sH; i++) {
            free(schematic->cells[i]);
        }
        free(schematic->cells);
        free(schematic->name);
        schematic->preview = NULL;
        return;
    }

    // draw cells onto the surface
    SDL_Rect cellRect = { 0, 0, 10, 10 };
    for (int i = 0; i < schematic->sH; i++) {
        for (int j = 0; j < schematic->sW; j++) {
            if (schematic->cells[i][j] == 1) {
                cellRect.x = j * 10;
                cellRect.y = i * 10;
                SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 255, 255, 255));  
            }
        }
    }

    // convert surface to texture
    schematic->preview = SDL_CreateTextureFromSurface(renderer, surface);
    if (!schematic->preview) {
        printf("Error: Texture creation failed for '%s': %s\n", schematic->name, SDL_GetError());
    }
    SDL_FreeSurface(surface);
}


