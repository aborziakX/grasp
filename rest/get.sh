echo "Построить C++ get.exe"
gcc -I ~/develop/boost_1_89_0/ -lstdc++ -lpthread -o get.exe -g get.cpp get_main.cpp
echo "Финиш"
