# CMakeLists.txt
message ("start!")
# требуем версию не ниже 3.2.3
cmake_minimum_required (VERSION 3.2.3 FATAL_ERROR)

# создать переменные
set(SOURCE main.cpp foo.cpp)
set(HEADER main.h
           foo.h)
set(FLTK_DIR "C:/fltk-1.4/build" CACHE STRING 
    "FLTK binary folder with FLTKConfig.cmake")

message ("SOURCE ${SOURCE}")
message ("HEADER ${HEADER}")
message ("FLTK_DIR ${FLTK_DIR}")

message ("finish!")
