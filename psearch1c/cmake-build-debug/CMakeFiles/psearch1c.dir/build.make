# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /snap/clion/44/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/44/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/psearch1c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/psearch1c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/psearch1c.dir/flags.make

CMakeFiles/psearch1c.dir/main.c.o: CMakeFiles/psearch1c.dir/flags.make
CMakeFiles/psearch1c.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/psearch1c.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/psearch1c.dir/main.c.o   -c "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/main.c"

CMakeFiles/psearch1c.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/psearch1c.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/main.c" > CMakeFiles/psearch1c.dir/main.c.i

CMakeFiles/psearch1c.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/psearch1c.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/main.c" -o CMakeFiles/psearch1c.dir/main.c.s

# Object files for target psearch1c
psearch1c_OBJECTS = \
"CMakeFiles/psearch1c.dir/main.c.o"

# External object files for target psearch1c
psearch1c_EXTERNAL_OBJECTS =

psearch1c: CMakeFiles/psearch1c.dir/main.c.o
psearch1c: CMakeFiles/psearch1c.dir/build.make
psearch1c: CMakeFiles/psearch1c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable psearch1c"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/psearch1c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/psearch1c.dir/build: psearch1c

.PHONY : CMakeFiles/psearch1c.dir/build

CMakeFiles/psearch1c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/psearch1c.dir/cmake_clean.cmake
.PHONY : CMakeFiles/psearch1c.dir/clean

CMakeFiles/psearch1c.dir/depend:
	cd "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c" "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c" "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug" "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug" "/home/mmuazekici/CLionProjects/OS_Course_Project _1/psearch1c/cmake-build-debug/CMakeFiles/psearch1c.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/psearch1c.dir/depend
