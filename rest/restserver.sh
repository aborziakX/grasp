echo "Построить C++ restserver"
gcc -I ~/develop/boost_1_89_0/ -I ~/develop/nlohmann/include -lstdc++ -lpthread -o restserver -g restserver.cpp
echo "Финиш"
