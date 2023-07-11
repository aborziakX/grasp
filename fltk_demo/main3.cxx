//
// Hello, flex_demo! Небольшие программы для изучения FLTK.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
// 

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

//==tabs-simple.cxx
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
//
// Пример с TAB
//      _____  _____
//   __/ Aaa \/ Bbb \______________________
//  |    _______                           |
//  |   |_______|                          |
//  |    _______                           |
//  |   |_______|                          |
//  |    _______                           |
//  |   |_______|                          |
//  |______________________________________|
//
int main4(int argc, char* argv[]) {
    Fl::scheme("gtk+");
    Fl_Window* win = new Fl_Window(500, 200, u8"Пример с TAB");
    {
        // создать tab widget
        Fl_Tabs* tabs = new Fl_Tabs(10, 10, 500 - 20, 200 - 20);
        {
            // добавить "Aaa" TAB
            // Мы делаем это, добавляя дочернюю группу к виджету вкладки.
            // Метка дочерней группы определяет метку вкладки.
            //
            Fl_Group* aaa = new Fl_Group(10, 35, 500 - 20, 200 - 45, "Aaa");
            {
                // Поместите несколько разных кнопок в группу, которая будет отображаться
                // когда вкладка выбрана.
                Fl_Button* b1 = new Fl_Button(50, 60, 90, 25, "Button A1"); b1->color(88 + 1);
                Fl_Button* b2 = new Fl_Button(50, 90, 90, 25, "Button A2"); b2->color(88 + 2);
                Fl_Button* b3 = new Fl_Button(50, 120, 90, 25, "Button A3"); b3->color(88 + 3);
            }
            aaa->end();

            // добавить "Bbb" TAB
            Fl_Group* bbb = new Fl_Group(10, 35, 500 - 10, 200 - 35, "Bbb");
            {
                // Поместите несколько разных кнопок в группу, которая будет отображаться
                // когда вкладка выбрана.
                Fl_Button* b1 = new Fl_Button(50, 60, 90, 25, "Button B1"); b1->color(56);
                Fl_Button* b2 = new Fl_Button(150, 60, 90, 25, "Button B2"); b2->color(88);
                Fl_Button* b3 = new Fl_Button(250, 60, 90, 25, "Button B3"); b3->color(63);
                Fl_Button* b4 = new Fl_Button(50, 90, 90, 25, "Button B4"); b4->color(95);
                Fl_Button* b5 = new Fl_Button(150, 90, 90, 25, "Button B5"); b5->color(216);
                Fl_Button* b6 = new Fl_Button(250, 90, 90, 25, "Button B6"); b6->color(248);
                Fl_Button* b7 = new Fl_Button(50, 120, 90, 25, "Button B7"); b7->color(223);
                Fl_Button* b8 = new Fl_Button(150, 120, 90, 25, "Button B8"); b8->color(255);
                Fl_Button* b9 = new Fl_Button(250, 120, 90, 25, "Button B9"); b9->color(72);
            }
            bbb->end();
        }
        tabs->end();
    }
    win->end();
    win->show(argc, argv);
    return(Fl::run());
}

int main(int argc, char** argv)
{
    int rc = 0;
    // задать стиль рисования контролов/виджетов
    Fl::scheme("gtk+");
    // запуск демо
    rc = main4(argc, argv);
    return rc;
}
