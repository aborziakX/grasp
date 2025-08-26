# CMakeLists.txt для построения test.exe с помощью CMake системы построения
message ("start!")
project(test.exe)
add_executable(test.exe "test.cpp")
target_link_libraries(test.exe "stdc++")
message ("done")

