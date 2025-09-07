echo "Построить C++ httpsvr"
gcc -I ~/develop/boost_1_89_0/ -lstdc++ -lpthread -o httpsvr -g httpsvr.cpp
echo "Финиш"
