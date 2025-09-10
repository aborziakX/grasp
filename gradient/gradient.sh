echo "Построить C++ gradient.exe"
gcc -o gradient.exe -g POptimizer.cpp PFunc.cpp -lstdc++ -lm -lpthread
echo "Финиш"
