# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zrowitsch/src/raytracer-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zrowitsch/src/raytracer-c/build-debug

# Include any dependencies generated for this target.
include module_utilities/CMakeFiles/module_utilities.dir/depend.make

# Include the progress variables for this target.
include module_utilities/CMakeFiles/module_utilities.dir/progress.make

# Include the compile flags for this target's objects.
include module_utilities/CMakeFiles/module_utilities.dir/flags.make

module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.o: module_utilities/CMakeFiles/module_utilities.dir/flags.make
module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.o: ../module_utilities/exceptions.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_utilities.dir/exceptions.c.o   -c /home/zrowitsch/src/raytracer-c/module_utilities/exceptions.c

module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_utilities.dir/exceptions.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_utilities/exceptions.c > CMakeFiles/module_utilities.dir/exceptions.c.i

module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_utilities.dir/exceptions.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_utilities/exceptions.c -o CMakeFiles/module_utilities.dir/exceptions.c.s

module_utilities/CMakeFiles/module_utilities.dir/logger.c.o: module_utilities/CMakeFiles/module_utilities.dir/flags.make
module_utilities/CMakeFiles/module_utilities.dir/logger.c.o: ../module_utilities/logger.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object module_utilities/CMakeFiles/module_utilities.dir/logger.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_utilities.dir/logger.c.o   -c /home/zrowitsch/src/raytracer-c/module_utilities/logger.c

module_utilities/CMakeFiles/module_utilities.dir/logger.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_utilities.dir/logger.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_utilities/logger.c > CMakeFiles/module_utilities.dir/logger.c.i

module_utilities/CMakeFiles/module_utilities.dir/logger.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_utilities.dir/logger.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_utilities/logger.c -o CMakeFiles/module_utilities.dir/logger.c.s

module_utilities/CMakeFiles/module_utilities.dir/utilities.c.o: module_utilities/CMakeFiles/module_utilities.dir/flags.make
module_utilities/CMakeFiles/module_utilities.dir/utilities.c.o: ../module_utilities/utilities.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object module_utilities/CMakeFiles/module_utilities.dir/utilities.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_utilities.dir/utilities.c.o   -c /home/zrowitsch/src/raytracer-c/module_utilities/utilities.c

module_utilities/CMakeFiles/module_utilities.dir/utilities.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_utilities.dir/utilities.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_utilities/utilities.c > CMakeFiles/module_utilities.dir/utilities.c.i

module_utilities/CMakeFiles/module_utilities.dir/utilities.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_utilities.dir/utilities.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_utilities/utilities.c -o CMakeFiles/module_utilities.dir/utilities.c.s

# Object files for target module_utilities
module_utilities_OBJECTS = \
"CMakeFiles/module_utilities.dir/exceptions.c.o" \
"CMakeFiles/module_utilities.dir/logger.c.o" \
"CMakeFiles/module_utilities.dir/utilities.c.o"

# External object files for target module_utilities
module_utilities_EXTERNAL_OBJECTS =

module_utilities/libmodule_utilities.a: module_utilities/CMakeFiles/module_utilities.dir/exceptions.c.o
module_utilities/libmodule_utilities.a: module_utilities/CMakeFiles/module_utilities.dir/logger.c.o
module_utilities/libmodule_utilities.a: module_utilities/CMakeFiles/module_utilities.dir/utilities.c.o
module_utilities/libmodule_utilities.a: module_utilities/CMakeFiles/module_utilities.dir/build.make
module_utilities/libmodule_utilities.a: module_utilities/CMakeFiles/module_utilities.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library libmodule_utilities.a"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && $(CMAKE_COMMAND) -P CMakeFiles/module_utilities.dir/cmake_clean_target.cmake
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/module_utilities.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
module_utilities/CMakeFiles/module_utilities.dir/build: module_utilities/libmodule_utilities.a

.PHONY : module_utilities/CMakeFiles/module_utilities.dir/build

module_utilities/CMakeFiles/module_utilities.dir/clean:
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_utilities && $(CMAKE_COMMAND) -P CMakeFiles/module_utilities.dir/cmake_clean.cmake
.PHONY : module_utilities/CMakeFiles/module_utilities.dir/clean

module_utilities/CMakeFiles/module_utilities.dir/depend:
	cd /home/zrowitsch/src/raytracer-c/build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zrowitsch/src/raytracer-c /home/zrowitsch/src/raytracer-c/module_utilities /home/zrowitsch/src/raytracer-c/build-debug /home/zrowitsch/src/raytracer-c/build-debug/module_utilities /home/zrowitsch/src/raytracer-c/build-debug/module_utilities/CMakeFiles/module_utilities.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : module_utilities/CMakeFiles/module_utilities.dir/depend
