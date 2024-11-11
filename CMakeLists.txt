
cmake_minimum_required(VERSION 3.10)

project(Conways-Game-of-Life-C)

# Include directories
include_directories(include)

# Specify source files
set(SOURCES
    src/main.c
    src/UI.c
    src/schems.c
)

add_executable(Conways-Game-of-Life-C ${SOURCES})

# Find SDL2 libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Link SDL2 libraries
target_include_directories(Conways-Game-of-Life-C PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(Conways-Game-of-Life-C PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
