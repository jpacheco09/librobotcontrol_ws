# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jpacheco/librobotcontrol_ws/libtest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jpacheco/librobotcontrol_ws/libtest/build

# Include any dependencies generated for this target.
include CMakeFiles/eqep_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/eqep_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/eqep_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/eqep_test.dir/flags.make

CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o: CMakeFiles/eqep_test.dir/flags.make
CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o: ../src/eqep_test.cpp
CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o: CMakeFiles/eqep_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jpacheco/librobotcontrol_ws/libtest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o -MF CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o.d -o CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o -c /home/jpacheco/librobotcontrol_ws/libtest/src/eqep_test.cpp

CMakeFiles/eqep_test.dir/src/eqep_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eqep_test.dir/src/eqep_test.cpp.i"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jpacheco/librobotcontrol_ws/libtest/src/eqep_test.cpp > CMakeFiles/eqep_test.dir/src/eqep_test.cpp.i

CMakeFiles/eqep_test.dir/src/eqep_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eqep_test.dir/src/eqep_test.cpp.s"
	/usr/bin/x86_64-linux-gnu-g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jpacheco/librobotcontrol_ws/libtest/src/eqep_test.cpp -o CMakeFiles/eqep_test.dir/src/eqep_test.cpp.s

# Object files for target eqep_test
eqep_test_OBJECTS = \
"CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o"

# External object files for target eqep_test
eqep_test_EXTERNAL_OBJECTS =

../bin/eqep_test: CMakeFiles/eqep_test.dir/src/eqep_test.cpp.o
../bin/eqep_test: CMakeFiles/eqep_test.dir/build.make
../bin/eqep_test: CMakeFiles/eqep_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jpacheco/librobotcontrol_ws/libtest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/eqep_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/eqep_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/eqep_test.dir/build: ../bin/eqep_test
.PHONY : CMakeFiles/eqep_test.dir/build

CMakeFiles/eqep_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/eqep_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/eqep_test.dir/clean

CMakeFiles/eqep_test.dir/depend:
	cd /home/jpacheco/librobotcontrol_ws/libtest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jpacheco/librobotcontrol_ws/libtest /home/jpacheco/librobotcontrol_ws/libtest /home/jpacheco/librobotcontrol_ws/libtest/build /home/jpacheco/librobotcontrol_ws/libtest/build /home/jpacheco/librobotcontrol_ws/libtest/build/CMakeFiles/eqep_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/eqep_test.dir/depend

