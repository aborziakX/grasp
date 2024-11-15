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
    Fl_Window* win = new Fl_Window(400, 350, u8"Пример с TAB");
    {
        // создать tab widget
        Fl_Tabs* tabs = new Fl_Tabs(10, 10, 400 - 20, 350 - 20);
        {
            // добавить "Кнопки" TAB
            // Мы делаем это, добавляя дочернюю группу к виджету вкладки.
            // Метка дочерней группы определяет метку вкладки.
            //
            Fl_Group* aaa = new Fl_Group(10, 35, 400 - 20, 350 - 45, u8"Кнопки");
            {
                // Поместите несколько разных кнопок в группу, которая будет отображаться
                // когда вкладка выбрана.
                Fl_Button* b1 = new Fl_Button(50, 60, 90, 25, "Button A1"); b1->color(88 + 1);
                Fl_Button* b2 = new Fl_Button(50, 90, 90, 25, "Button A2"); b2->color(88 + 4);
                Fl_Button* b3 = new Fl_Button(50, 120, 90, 25, "Button A3"); b3->color(88 - 3);
            }
            aaa->end();

            // добавить 2-й TAB
            Fl_Group* bbb = new Fl_Group(10, 35, 400 - 10, 350 - 35, u8"FLTK палитра");
            {
                // Поместите несколько боксов в группу, которая будет отображаться
                // когда вкладка выбрана.
                int x = 40, y = 30;
                for (int i = 0; i < 200; i++)
                {
                    if (i % 16 == 0)
                    { // по 16 в ряд
                        x = 40;
                        y += 20;
                    }
                    Fl_Box* b1 = new Fl_Box(x, y, 20, 20);
                    b1->box(FL_FLAT_BOX); // стиль-плоский бокс
                    b1->color(56 + i);
                    x += 20;
                }
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
