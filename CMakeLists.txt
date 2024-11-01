
cmake_minimum_required(VERSION 3.10)

project(Conways-Game-of-Life-C)

include_directories(include)

# List all your source files here
set(SOURCES
    src/main.c
    src/UI.c
    src/schems.c
    # Add any other .c files you have here
)

add_executable(Conways-Game-of-Life-C ${SOURCES})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

#link sdl2 
target_link_libraries(Conways-Game-of-Life-C SDL2) 
target_link_libraries(Conways-Game-of-Life-C SDL2_image) 
target_link_libraries(Conways-Game-of-Life-C SDL2_ttf)
