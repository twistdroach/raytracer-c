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
include module_shapes/CMakeFiles/module_shapes.dir/depend.make

# Include the progress variables for this target.
include module_shapes/CMakeFiles/module_shapes.dir/progress.make

# Include the compile flags for this target's objects.
include module_shapes/CMakeFiles/module_shapes.dir/flags.make

module_shapes/CMakeFiles/module_shapes.dir/sphere.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/sphere.c.o: ../module_shapes/sphere.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object module_shapes/CMakeFiles/module_shapes.dir/sphere.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/sphere.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/sphere.c

module_shapes/CMakeFiles/module_shapes.dir/sphere.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/sphere.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/sphere.c > CMakeFiles/module_shapes.dir/sphere.c.i

module_shapes/CMakeFiles/module_shapes.dir/sphere.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/sphere.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/sphere.c -o CMakeFiles/module_shapes.dir/sphere.c.s

module_shapes/CMakeFiles/module_shapes.dir/shape.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/shape.c.o: ../module_shapes/shape.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object module_shapes/CMakeFiles/module_shapes.dir/shape.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/shape.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/shape.c

module_shapes/CMakeFiles/module_shapes.dir/shape.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/shape.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/shape.c > CMakeFiles/module_shapes.dir/shape.c.i

module_shapes/CMakeFiles/module_shapes.dir/shape.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/shape.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/shape.c -o CMakeFiles/module_shapes.dir/shape.c.s

module_shapes/CMakeFiles/module_shapes.dir/testshape.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/testshape.c.o: ../module_shapes/testshape.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object module_shapes/CMakeFiles/module_shapes.dir/testshape.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/testshape.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/testshape.c

module_shapes/CMakeFiles/module_shapes.dir/testshape.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/testshape.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/testshape.c > CMakeFiles/module_shapes.dir/testshape.c.i

module_shapes/CMakeFiles/module_shapes.dir/testshape.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/testshape.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/testshape.c -o CMakeFiles/module_shapes.dir/testshape.c.s

module_shapes/CMakeFiles/module_shapes.dir/plane.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/plane.c.o: ../module_shapes/plane.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object module_shapes/CMakeFiles/module_shapes.dir/plane.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/plane.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/plane.c

module_shapes/CMakeFiles/module_shapes.dir/plane.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/plane.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/plane.c > CMakeFiles/module_shapes.dir/plane.c.i

module_shapes/CMakeFiles/module_shapes.dir/plane.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/plane.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/plane.c -o CMakeFiles/module_shapes.dir/plane.c.s

module_shapes/CMakeFiles/module_shapes.dir/cube.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/cube.c.o: ../module_shapes/cube.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object module_shapes/CMakeFiles/module_shapes.dir/cube.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/cube.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/cube.c

module_shapes/CMakeFiles/module_shapes.dir/cube.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/cube.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/cube.c > CMakeFiles/module_shapes.dir/cube.c.i

module_shapes/CMakeFiles/module_shapes.dir/cube.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/cube.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/cube.c -o CMakeFiles/module_shapes.dir/cube.c.s

module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.o: ../module_shapes/cylinder.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/cylinder.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/cylinder.c

module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/cylinder.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/cylinder.c > CMakeFiles/module_shapes.dir/cylinder.c.i

module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/cylinder.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/cylinder.c -o CMakeFiles/module_shapes.dir/cylinder.c.s

module_shapes/CMakeFiles/module_shapes.dir/cone.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/cone.c.o: ../module_shapes/cone.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object module_shapes/CMakeFiles/module_shapes.dir/cone.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/cone.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/cone.c

module_shapes/CMakeFiles/module_shapes.dir/cone.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/cone.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/cone.c > CMakeFiles/module_shapes.dir/cone.c.i

module_shapes/CMakeFiles/module_shapes.dir/cone.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/cone.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/cone.c -o CMakeFiles/module_shapes.dir/cone.c.s

module_shapes/CMakeFiles/module_shapes.dir/group.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/group.c.o: ../module_shapes/group.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object module_shapes/CMakeFiles/module_shapes.dir/group.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/group.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/group.c

module_shapes/CMakeFiles/module_shapes.dir/group.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/group.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/group.c > CMakeFiles/module_shapes.dir/group.c.i

module_shapes/CMakeFiles/module_shapes.dir/group.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/group.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/group.c -o CMakeFiles/module_shapes.dir/group.c.s

module_shapes/CMakeFiles/module_shapes.dir/triangle.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/triangle.c.o: ../module_shapes/triangle.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object module_shapes/CMakeFiles/module_shapes.dir/triangle.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/triangle.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/triangle.c

module_shapes/CMakeFiles/module_shapes.dir/triangle.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/triangle.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/triangle.c > CMakeFiles/module_shapes.dir/triangle.c.i

module_shapes/CMakeFiles/module_shapes.dir/triangle.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/triangle.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/triangle.c -o CMakeFiles/module_shapes.dir/triangle.c.s

module_shapes/CMakeFiles/module_shapes.dir/csg.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/csg.c.o: ../module_shapes/csg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object module_shapes/CMakeFiles/module_shapes.dir/csg.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/csg.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/csg.c

module_shapes/CMakeFiles/module_shapes.dir/csg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/csg.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/csg.c > CMakeFiles/module_shapes.dir/csg.c.i

module_shapes/CMakeFiles/module_shapes.dir/csg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/csg.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/csg.c -o CMakeFiles/module_shapes.dir/csg.c.s

module_shapes/CMakeFiles/module_shapes.dir/bound.c.o: module_shapes/CMakeFiles/module_shapes.dir/flags.make
module_shapes/CMakeFiles/module_shapes.dir/bound.c.o: ../module_shapes/bound.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object module_shapes/CMakeFiles/module_shapes.dir/bound.c.o"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/module_shapes.dir/bound.c.o   -c /home/zrowitsch/src/raytracer-c/module_shapes/bound.c

module_shapes/CMakeFiles/module_shapes.dir/bound.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/module_shapes.dir/bound.c.i"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zrowitsch/src/raytracer-c/module_shapes/bound.c > CMakeFiles/module_shapes.dir/bound.c.i

module_shapes/CMakeFiles/module_shapes.dir/bound.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/module_shapes.dir/bound.c.s"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zrowitsch/src/raytracer-c/module_shapes/bound.c -o CMakeFiles/module_shapes.dir/bound.c.s

# Object files for target module_shapes
module_shapes_OBJECTS = \
"CMakeFiles/module_shapes.dir/sphere.c.o" \
"CMakeFiles/module_shapes.dir/shape.c.o" \
"CMakeFiles/module_shapes.dir/testshape.c.o" \
"CMakeFiles/module_shapes.dir/plane.c.o" \
"CMakeFiles/module_shapes.dir/cube.c.o" \
"CMakeFiles/module_shapes.dir/cylinder.c.o" \
"CMakeFiles/module_shapes.dir/cone.c.o" \
"CMakeFiles/module_shapes.dir/group.c.o" \
"CMakeFiles/module_shapes.dir/triangle.c.o" \
"CMakeFiles/module_shapes.dir/csg.c.o" \
"CMakeFiles/module_shapes.dir/bound.c.o"

# External object files for target module_shapes
module_shapes_EXTERNAL_OBJECTS =

module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/sphere.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/shape.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/testshape.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/plane.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/cube.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/cylinder.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/cone.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/group.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/triangle.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/csg.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/bound.c.o
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/build.make
module_shapes/libmodule_shapes.a: module_shapes/CMakeFiles/module_shapes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zrowitsch/src/raytracer-c/build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking C static library libmodule_shapes.a"
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && $(CMAKE_COMMAND) -P CMakeFiles/module_shapes.dir/cmake_clean_target.cmake
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/module_shapes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
module_shapes/CMakeFiles/module_shapes.dir/build: module_shapes/libmodule_shapes.a

.PHONY : module_shapes/CMakeFiles/module_shapes.dir/build

module_shapes/CMakeFiles/module_shapes.dir/clean:
	cd /home/zrowitsch/src/raytracer-c/build-debug/module_shapes && $(CMAKE_COMMAND) -P CMakeFiles/module_shapes.dir/cmake_clean.cmake
.PHONY : module_shapes/CMakeFiles/module_shapes.dir/clean

module_shapes/CMakeFiles/module_shapes.dir/depend:
	cd /home/zrowitsch/src/raytracer-c/build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zrowitsch/src/raytracer-c /home/zrowitsch/src/raytracer-c/module_shapes /home/zrowitsch/src/raytracer-c/build-debug /home/zrowitsch/src/raytracer-c/build-debug/module_shapes /home/zrowitsch/src/raytracer-c/build-debug/module_shapes/CMakeFiles/module_shapes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : module_shapes/CMakeFiles/module_shapes.dir/depend
