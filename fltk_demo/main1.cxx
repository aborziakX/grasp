//
// Hello, flex_demo! Небольшие программы для изучения FLTK.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

//===на базе hello.cxx
int main1(int argc, char** argv) {
    // создать главное окно
    Fl_Window* window = new Fl_Window(340, 180);
    // создать внутри окошко
    Fl_Box* box = new Fl_Box(20, 40, 300, 100, "Hello, flex_demo!");
    // стиль окошка чуть выпуклый
    box->box(FL_UP_BOX);
    // задать шрифт Жирный, Курсив
    box->labelfont(FL_BOLD + FL_ITALIC);
    // размер окошка
    box->labelsize(36);
    // стиль текста с тенью
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    // показать окно
    window->show(argc, argv);
    // выполнять до закрытия окна
    return Fl::run();
}
// главная функция
int main(int argc, char** argv)
{
    int rc = 0;
    // задать стиль рисования контролов/виджетов
    Fl::scheme("gtk+");
    // запуск первого демо
    rc = main1(argc, argv);
    return rc;
}
