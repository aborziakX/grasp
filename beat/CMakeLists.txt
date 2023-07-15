#
# CMakeLists.txt used to build beat app by the CMake build system
#
# Copyright 20023 by Andrey Borzyak and others.
#######################################################################
message ("start!")

#######################################################################
# set CMake minimum version (must be before `project()`
#######################################################################

# Minimum CMake version required by FLTK 1.4 (06/2020, work in progress)
cmake_minimum_required (VERSION 3.2.3 FATAL_ERROR)

project(beat)

# The following line is required only if (a) you didn't install FLTK
# or if (b) find_package can't find your installation directory because
# you installed FLTK in a non-standard location.  It points to
# (a) the base folder of the build directory, or
# (b) <fltk-install-prefix>/share/fltk
# resp., where <fltk-install-prefix> is the installation prefix you
# used to install FLTK.
# (The file FLTKConfig.cmake and others must be found in that path.)

if (WIN32)
  set(FLTK_DIR "C:/fltk-1.4/build" CACHE STRING 
    "FLTK binary folder with FLTKConfig.cmake")
endif (WIN32)
if (UNIX)
  set(FLTK_DIR "/home/andrey/fltk-1.4/build/" CACHE STRING 
    "FLTK binary folder with FLTKConfig.cmake")
endif (UNIX)

find_package(FLTK REQUIRED NO_MODULE)
#debug
message(STATUS "BUILD_FLTK                  = ${BUILD_FLTK}")
message(STATUS "FLTK_DIR                    = ${FLTK_DIR}")
message(STATUS "FLTK_FOUND                  = ${FLTK_FOUND}")
message(STATUS "FLTK_COMMIT                 = '${FLTK_COMMIT}'")
message(STATUS "FLTK_INCLUDE_DIRS           = ${FLTK_INCLUDE_DIRS}")

add_executable(beat WIN32 MACOSX_BUNDLE "beat.cxx;GeObWindow.cxx;GeOb.cxx;Cube.cxx;Vec3.cxx;Cyl.cxx;Lines.cxx;Dialogs.cxx;beat_ini.cxx;Cone.cxx;Sphere.cxx;Gadget.cxx;Utils.cxx;Poly.cxx;Models.cxx")
if (APPLE)
  target_link_libraries(beat "-framework cocoa")
endif (APPLE)

target_include_directories(beat PUBLIC ${FLTK_INCLUDE_DIRS})

target_link_libraries(beat "fltk_gl;fltk;${OPENGL_LIBRARIES}")

message ("done ${OPENGL_LIBRARIES}")

