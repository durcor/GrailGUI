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
CMAKE_SOURCE_DIR = /mnt/c/ubuntu_work/grail/examples/chatserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/ubuntu_work/grail/examples/chatserver/build

# Include any dependencies generated for this target.
include CMakeFiles/chatserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chatserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chatserver.dir/flags.make

CMakeFiles/chatserver.dir/chatserver.cpp.o: CMakeFiles/chatserver.dir/flags.make
CMakeFiles/chatserver.dir/chatserver.cpp.o: ../chatserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/ubuntu_work/grail/examples/chatserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chatserver.dir/chatserver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatserver.dir/chatserver.cpp.o -c /mnt/c/ubuntu_work/grail/examples/chatserver/chatserver.cpp

CMakeFiles/chatserver.dir/chatserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatserver.dir/chatserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/ubuntu_work/grail/examples/chatserver/chatserver.cpp > CMakeFiles/chatserver.dir/chatserver.cpp.i

CMakeFiles/chatserver.dir/chatserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatserver.dir/chatserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/ubuntu_work/grail/examples/chatserver/chatserver.cpp -o CMakeFiles/chatserver.dir/chatserver.cpp.s

# Object files for target chatserver
chatserver_OBJECTS = \
"CMakeFiles/chatserver.dir/chatserver.cpp.o"

# External object files for target chatserver
chatserver_EXTERNAL_OBJECTS =

chatserver: CMakeFiles/chatserver.dir/chatserver.cpp.o
chatserver: CMakeFiles/chatserver.dir/build.make
chatserver: /usr/lib/x86_64-linux-gnu/libssl.so
chatserver: /usr/lib/x86_64-linux-gnu/libcrypto.so
chatserver: CMakeFiles/chatserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/ubuntu_work/grail/examples/chatserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable chatserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chatserver.dir/build: chatserver

.PHONY : CMakeFiles/chatserver.dir/build

CMakeFiles/chatserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chatserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chatserver.dir/clean

CMakeFiles/chatserver.dir/depend:
	cd /mnt/c/ubuntu_work/grail/examples/chatserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/ubuntu_work/grail/examples/chatserver /mnt/c/ubuntu_work/grail/examples/chatserver /mnt/c/ubuntu_work/grail/examples/chatserver/build /mnt/c/ubuntu_work/grail/examples/chatserver/build /mnt/c/ubuntu_work/grail/examples/chatserver/build/CMakeFiles/chatserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chatserver.dir/depend

