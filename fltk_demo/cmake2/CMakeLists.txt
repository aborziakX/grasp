# CMakeLists.txt
# требуем версию не ниже 3.5
cmake_minimum_required(VERSION 3.5)
# создать проект HelloWorld из кода на языке С++
project(HelloWorld CXX)
# создать приложение hello из файла hello.cpp
add_executable(hello hello.cpp)
