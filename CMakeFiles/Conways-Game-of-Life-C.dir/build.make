# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C

# Include any dependencies generated for this target.
include CMakeFiles/Conways-Game-of-Life-C.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Conways-Game-of-Life-C.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Conways-Game-of-Life-C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Conways-Game-of-Life-C.dir/flags.make

CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o: CMakeFiles/Conways-Game-of-Life-C.dir/flags.make
CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o: src/main.c
CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o: CMakeFiles/Conways-Game-of-Life-C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o -MF CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o.d -o CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o -c /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/main.c

CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/main.c > CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.i

CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/main.c -o CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.s

CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o: CMakeFiles/Conways-Game-of-Life-C.dir/flags.make
CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o: src/UI.c
CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o: CMakeFiles/Conways-Game-of-Life-C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o -MF CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o.d -o CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o -c /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/UI.c

CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/UI.c > CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.i

CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/src/UI.c -o CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.s

# Object files for target Conways-Game-of-Life-C
Conways__Game__of__Life__C_OBJECTS = \
"CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o" \
"CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o"

# External object files for target Conways-Game-of-Life-C
Conways__Game__of__Life__C_EXTERNAL_OBJECTS =

Conways-Game-of-Life-C: CMakeFiles/Conways-Game-of-Life-C.dir/src/main.c.o
Conways-Game-of-Life-C: CMakeFiles/Conways-Game-of-Life-C.dir/src/UI.c.o
Conways-Game-of-Life-C: CMakeFiles/Conways-Game-of-Life-C.dir/build.make
Conways-Game-of-Life-C: CMakeFiles/Conways-Game-of-Life-C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Conways-Game-of-Life-C"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Conways-Game-of-Life-C.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Conways-Game-of-Life-C.dir/build: Conways-Game-of-Life-C
.PHONY : CMakeFiles/Conways-Game-of-Life-C.dir/build

CMakeFiles/Conways-Game-of-Life-C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Conways-Game-of-Life-C.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Conways-Game-of-Life-C.dir/clean

CMakeFiles/Conways-Game-of-Life-C.dir/depend:
	cd /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C /home/robert/Documents/C/game_of_life/Conways-Game-of-Life-C/CMakeFiles/Conways-Game-of-Life-C.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Conways-Game-of-Life-C.dir/depend

