#
# CMakeLists.txt для построения fltk_demo с помощью CMake системы построения
#
# Copyright 2023 by Andrey Borzyak and others.
#######################################################################
message ("start!")

#######################################################################
# set CMake minimum version (must be before `project()`
#######################################################################

# Minimum CMake version required by FLTK 1.4 (06/2020, work in progress)
cmake_minimum_required (VERSION 3.2.3 FATAL_ERROR)

project(fltk_demo)

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
  set(FLTK_DIR "~/develop/fltk-1.4/build/" CACHE STRING 
    "FLTK binary folder with FLTKConfig.cmake")
endif (UNIX)

find_package(FLTK REQUIRED NO_MODULE)

#debug
message(STATUS "BUILD_FLTK                  = ${BUILD_FLTK}")
message(STATUS "FLTK_DIR                    = ${FLTK_DIR}")
message(STATUS "FLTK_FOUND                  = ${FLTK_FOUND}")
message(STATUS "FLTK_COMMIT                 = '${FLTK_COMMIT}'")
message(STATUS "FLTK_INCLUDE_DIRS           = ${FLTK_INCLUDE_DIRS}")

add_executable(fltk_demo WIN32 MACOSX_BUNDLE "main.cxx")
if (APPLE)
  target_link_libraries(fltk_demo "-framework cocoa")
endif (APPLE)

target_include_directories(fltk_demo PUBLIC ${FLTK_INCLUDE_DIRS})

target_link_libraries(fltk_demo "fltk_gl;fltk;${OPENGL_LIBRARIES}")

message ("done ${OPENGL_LIBRARIES}")

