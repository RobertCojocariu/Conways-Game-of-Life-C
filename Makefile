
# Compiler and flags
CC = gcc
CFLAGS = -Iinclude $(shell sdl2-config --cflags) -w -g
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

# Source files and executable name
SOURCES = src/main.c src/UI.c src/schems.c
EXECUTABLE = Conways-Game-of-Life-C

# Build target
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

# Clean target
clean:
	rm -f $(EXECUTABLE)
