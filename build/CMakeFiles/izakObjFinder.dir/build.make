# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.0.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.0.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build

# Include any dependencies generated for this target.
include CMakeFiles/izakObjFinder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/izakObjFinder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/izakObjFinder.dir/flags.make

CMakeFiles/izakObjFinder.dir/src/main.cpp.o: CMakeFiles/izakObjFinder.dir/flags.make
CMakeFiles/izakObjFinder.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/izakObjFinder.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/izakObjFinder.dir/src/main.cpp.o -c /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main.cpp

CMakeFiles/izakObjFinder.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/izakObjFinder.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main.cpp > CMakeFiles/izakObjFinder.dir/src/main.cpp.i

CMakeFiles/izakObjFinder.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/izakObjFinder.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main.cpp -o CMakeFiles/izakObjFinder.dir/src/main.cpp.s

CMakeFiles/izakObjFinder.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/izakObjFinder.dir/src/main.cpp.o.requires

CMakeFiles/izakObjFinder.dir/src/main.cpp.o.provides: CMakeFiles/izakObjFinder.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/izakObjFinder.dir/build.make CMakeFiles/izakObjFinder.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/izakObjFinder.dir/src/main.cpp.o.provides

CMakeFiles/izakObjFinder.dir/src/main.cpp.o.provides.build: CMakeFiles/izakObjFinder.dir/src/main.cpp.o

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o: CMakeFiles/izakObjFinder.dir/flags.make
CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o: ../src/main_skeleton.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o -c /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main_skeleton.cpp

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main_skeleton.cpp > CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.i

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/src/main_skeleton.cpp -o CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.s

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.requires:
.PHONY : CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.requires

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.provides: CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.requires
	$(MAKE) -f CMakeFiles/izakObjFinder.dir/build.make CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.provides.build
.PHONY : CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.provides

CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.provides.build: CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o

# Object files for target izakObjFinder
izakObjFinder_OBJECTS = \
"CMakeFiles/izakObjFinder.dir/src/main.cpp.o" \
"CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o"

# External object files for target izakObjFinder
izakObjFinder_EXTERNAL_OBJECTS =

izakObjFinder: CMakeFiles/izakObjFinder.dir/src/main.cpp.o
izakObjFinder: CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o
izakObjFinder: CMakeFiles/izakObjFinder.dir/build.make
izakObjFinder: /usr/local/lib/libopencv_videostab.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_video.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ts.a
izakObjFinder: /usr/local/lib/libopencv_superres.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_stitching.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_photo.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ocl.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_objdetect.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_nonfree.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ml.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_legacy.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_imgproc.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_highgui.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_gpu.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_flann.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_features2d.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_core.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_contrib.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_calib3d.2.4.9.dylib
izakObjFinder: libmftracker.a
izakObjFinder: libpredator.a
izakObjFinder: libmftracker.a
izakObjFinder: /usr/local/lib/libopencv_videostab.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ts.a
izakObjFinder: /usr/local/lib/libopencv_superres.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_stitching.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_contrib.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_nonfree.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ocl.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_gpu.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_photo.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_objdetect.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_legacy.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_video.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_ml.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_calib3d.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_features2d.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_highgui.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_imgproc.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_flann.2.4.9.dylib
izakObjFinder: /usr/local/lib/libopencv_core.2.4.9.dylib
izakObjFinder: CMakeFiles/izakObjFinder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable izakObjFinder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/izakObjFinder.dir/link.txt --verbose=$(VERBOSE)
	/usr/local/Cellar/cmake/3.0.1/bin/cmake -DSRC="/Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/Resources/tldConfig.txt" -DDST="/Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build/" -P /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/copy.cmake

# Rule to build all files generated by this target.
CMakeFiles/izakObjFinder.dir/build: izakObjFinder
.PHONY : CMakeFiles/izakObjFinder.dir/build

CMakeFiles/izakObjFinder.dir/requires: CMakeFiles/izakObjFinder.dir/src/main.cpp.o.requires
CMakeFiles/izakObjFinder.dir/requires: CMakeFiles/izakObjFinder.dir/src/main_skeleton.cpp.o.requires
.PHONY : CMakeFiles/izakObjFinder.dir/requires

CMakeFiles/izakObjFinder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/izakObjFinder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/izakObjFinder.dir/clean

CMakeFiles/izakObjFinder.dir/depend:
	cd /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build /Users/ermanoarruda/Documents/cmake-ws/izakObjFinder/build/CMakeFiles/izakObjFinder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/izakObjFinder.dir/depend

