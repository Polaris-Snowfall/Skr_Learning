# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/local/lib/python3.10/dist-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /usr/local/lib/python3.10/dist-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/FunctionInfo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/FunctionInfo/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/FunctionInfo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/CMakeFiles/FunctionInfo.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/FunctionInfo.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/FunctionInfo.dir/flags.make

lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o: lib/CMakeFiles/FunctionInfo.dir/flags.make
lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o: /mnt/FunctionInfo/lib/FunctionInfo.cpp
lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o: lib/CMakeFiles/FunctionInfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/FunctionInfo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o"
	cd /mnt/FunctionInfo/build/lib && $(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy-16;--extra-arg-before=-stdlib=libc++;--extra-arg-before=--driver-mode=g++" --source=/mnt/FunctionInfo/lib/FunctionInfo.cpp -- /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o -MF CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o.d -o CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o -c /mnt/FunctionInfo/lib/FunctionInfo.cpp

lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.i"
	cd /mnt/FunctionInfo/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/FunctionInfo/lib/FunctionInfo.cpp > CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.i

lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.s"
	cd /mnt/FunctionInfo/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/FunctionInfo/lib/FunctionInfo.cpp -o CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.s

# Object files for target FunctionInfo
FunctionInfo_OBJECTS = \
"CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o"

# External object files for target FunctionInfo
FunctionInfo_EXTERNAL_OBJECTS =

lib/libFunctionInfo.so: lib/CMakeFiles/FunctionInfo.dir/FunctionInfo.cpp.o
lib/libFunctionInfo.so: lib/CMakeFiles/FunctionInfo.dir/build.make
lib/libFunctionInfo.so: lib/CMakeFiles/FunctionInfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/FunctionInfo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libFunctionInfo.so"
	cd /mnt/FunctionInfo/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FunctionInfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/FunctionInfo.dir/build: lib/libFunctionInfo.so
.PHONY : lib/CMakeFiles/FunctionInfo.dir/build

lib/CMakeFiles/FunctionInfo.dir/clean:
	cd /mnt/FunctionInfo/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/FunctionInfo.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/FunctionInfo.dir/clean

lib/CMakeFiles/FunctionInfo.dir/depend:
	cd /mnt/FunctionInfo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/FunctionInfo /mnt/FunctionInfo/lib /mnt/FunctionInfo/build /mnt/FunctionInfo/build/lib /mnt/FunctionInfo/build/lib/CMakeFiles/FunctionInfo.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : lib/CMakeFiles/FunctionInfo.dir/depend

