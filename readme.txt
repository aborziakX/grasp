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

cd ~
mkdir develop
mkdir fltk-1.4
git clone https://github.com/fltk/fltk fltk-1.4
cd fltk-1.4
mkdir build
cd build
cmake ..

после cmake запускать для построения make .

Далее установка grasp

cd ~/develop
mkdir grasp
git clone https://github.com/aborziakX/grasp.git grasp
cd beat
mkdir build
cd build

CMakeLists.txt редактировать
if (UNIX)
  set(FLTK_DIR "~/develop/fltk-1.4/build/" CACHE STRING 
    "FLTK binary folder with FLTKConfig.cmake")
  #set(BOOST_INCLUDE_DIRS "/home/andrey/boost/boost_1_82_0" )
endif (UNIX)

cmake ..

после cmake запускать для построения make .




=============== beat
В папке beat имеется файл beat.btt. Его можно использовать для команды меню "Проект/Создать".
В папке beat имеется файл beat.bpj. Его можно использовать для команды меню "Проект/Открыть".

В секции 
[Controls]
могут быть настройки генерации ini-файла, например: 
geom_header=[Geometry]
geom_record={geom_type};{x};{y};{z};{dx};{dy};{dz};{side};{clr};{speedX};{speedY};{speedZ};{mass};{temperature};{material}
geom_features=features=geom_type,x,y,z,dx,dy,dz,side,clr,

Для связи - Борзяк Андрей - aborziak@yandex.ru
