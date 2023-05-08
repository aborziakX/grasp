Windows

Запустить Visual Studio, из меню Средства (Tools), Командная строка, Командная строка разработчика
выполнить последовательно
cd c:\
mkdir fltk-1.4
git clone https://github.com/fltk/fltk fltk-1.4
cd fltk-1.4
mkdir build
cd build
cmake ..

При успехе в build будет создано решение fltk.sln
Открыть его, собрать решение. Будут созданы библиотеки и тесты (около 60) в папке test. Можно запустить demo.exe.

cd c:\
mkdir grasp
git clone https://github.com/aborziakX/grasp.git grasp
cd grasp
cd beat
mkdir build
cd build
cmake ..

При успехе в build будет создано решение beat.sln
Открыть его, собрать решение. Будет создан beat.exe. Это прото-прототип. Мышкой можно вращать сцену с кубиками или использовать стрелки.

===============

Linux
команды аналогичны, 
CMakeLists.txt удалить
Linux-C-MakeLists.txt переиеновать в CMakeLists.txt
после cmake запускать для построения make .

=============== beat
В папке beat имеется файл beat.bpj. Его можно использовать для команды меню "Проект/Открыть".
