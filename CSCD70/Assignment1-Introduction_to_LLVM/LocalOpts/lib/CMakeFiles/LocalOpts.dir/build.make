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
CMAKE_SOURCE_DIR = /mnt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt

# Include any dependencies generated for this target.
include lib/CMakeFiles/LocalOpts.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/CMakeFiles/LocalOpts.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/LocalOpts.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/LocalOpts.dir/flags.make

lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o: lib/CMakeFiles/LocalOpts.dir/flags.make
lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o: lib/1-AlgebraicIdentity.cpp
lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o: lib/CMakeFiles/LocalOpts.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o -MF CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o.d -o CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o -c /mnt/lib/1-AlgebraicIdentity.cpp

lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.i"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/lib/1-AlgebraicIdentity.cpp > CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.i

lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.s"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/lib/1-AlgebraicIdentity.cpp -o CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.s

lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o: lib/CMakeFiles/LocalOpts.dir/flags.make
lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o: lib/2-StrengthReduction.cpp
lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o: lib/CMakeFiles/LocalOpts.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o -MF CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o.d -o CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o -c /mnt/lib/2-StrengthReduction.cpp

lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.i"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/lib/2-StrengthReduction.cpp > CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.i

lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.s"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/lib/2-StrengthReduction.cpp -o CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.s

lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o: lib/CMakeFiles/LocalOpts.dir/flags.make
lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o: lib/3-MultiInstOpt.cpp
lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o: lib/CMakeFiles/LocalOpts.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o -MF CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o.d -o CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o -c /mnt/lib/3-MultiInstOpt.cpp

lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.i"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/lib/3-MultiInstOpt.cpp > CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.i

lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.s"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/lib/3-MultiInstOpt.cpp -o CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.s

lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o: lib/CMakeFiles/LocalOpts.dir/flags.make
lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o: lib/LocalOpts.cpp
lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o: lib/CMakeFiles/LocalOpts.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o -MF CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o.d -o CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o -c /mnt/lib/LocalOpts.cpp

lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LocalOpts.dir/LocalOpts.cpp.i"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/lib/LocalOpts.cpp > CMakeFiles/LocalOpts.dir/LocalOpts.cpp.i

lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LocalOpts.dir/LocalOpts.cpp.s"
	cd /mnt/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/lib/LocalOpts.cpp -o CMakeFiles/LocalOpts.dir/LocalOpts.cpp.s

# Object files for target LocalOpts
LocalOpts_OBJECTS = \
"CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o" \
"CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o" \
"CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o" \
"CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o"

# External object files for target LocalOpts
LocalOpts_EXTERNAL_OBJECTS =

lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/1-AlgebraicIdentity.cpp.o
lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/2-StrengthReduction.cpp.o
lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/3-MultiInstOpt.cpp.o
lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/LocalOpts.cpp.o
lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/build.make
lib/libLocalOpts.so: lib/CMakeFiles/LocalOpts.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libLocalOpts.so"
	cd /mnt/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LocalOpts.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/LocalOpts.dir/build: lib/libLocalOpts.so
.PHONY : lib/CMakeFiles/LocalOpts.dir/build

lib/CMakeFiles/LocalOpts.dir/clean:
	cd /mnt/lib && $(CMAKE_COMMAND) -P CMakeFiles/LocalOpts.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/LocalOpts.dir/clean

lib/CMakeFiles/LocalOpts.dir/depend:
	cd /mnt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt /mnt/lib /mnt /mnt/lib /mnt/lib/CMakeFiles/LocalOpts.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : lib/CMakeFiles/LocalOpts.dir/depend

