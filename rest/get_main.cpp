#include <iostream>
#include <unistd.h>
#include "get.h"
using namespace std;
int main(int argc, char** argv)
{
    // сформировать адрес ресурса: по умолчанию для теста или реальный из списка аргументов вызова
    string url = (argc <= 1 ? "www.pvobr.ru" : argv[1]);
    string s = Grasp::Rest::Get(url, "80", "/");
    // вывести содержимое ресурса в консоль
    cout << s << endl;
}
