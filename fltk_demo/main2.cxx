//
// Hello, flex_demo! Небольшие программы для изучения FLTK.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

//===progress-simle.cxx
#ifdef WIN32
// WINDOWS
#include <windows.h>
#define usleep(v) Sleep(v/1000)
#else /*WIN32*/
// UNIX
#include <unistd.h>                            // usleep
#endif /*WIN32*/
#include <FL/Fl_Progress.H>
#include <FL/Fl_Button.H>

// обработчик кнопки
void butt_cb(Fl_Widget* butt, void* data)
{
    // деактивировать, дабы предотвратить повторное нажатие кнопки
    butt->deactivate();                   
    Fl::check();                               // дайте fltk немного процессорного времени, чтобы стала серая кнопка
    // Сделать индикатор выполнения
    Fl_Window* w = (Fl_Window*)data;           // 2-й параметр это главное окно
    w->begin();                                // добавить к нему индикатор выполнения
    Fl_Progress* progress = new Fl_Progress(10, 50, 200, 30);
    progress->minimum(0);                      // установить диапазон прогресса от 0,0 до 1,0
    progress->maximum(1);
    progress->color(0x88888800);               // фоновый цвет
    progress->selection_color(0x4444ff00);     // цвет индикатора выполнения
    progress->labelcolor(FL_WHITE);            // цвет текста с процентами
    w->end();                                  // завершить добавление в окно
    // Цикл вычислений..
    for (int t = 1; t <= 500; t++)
    {
        progress->value((float)(t / 500.0));              // обновить индикатор выполнения со значением от 0,0 до 1,0
        char percent[10];
        sprintf(percent, "%d%%", int((t / 500.0) * 100.0));
        progress->label(percent);              // обновить метку индикатора выполнения
        Fl::check();                           // дайте fltk немного процессора для обновления экрана
        usleep(1000);                          // заснуть
    }
    // Очистка
    w->remove(progress);                       // убрать индикатор выполнения из окна
    delete(progress);                          // освободить его память
    butt->activate();                          // повторная активация кнопки
    w->redraw();                               // сказать окну перерисовать теперь, когда индикатор прогресса удален
}

// демо
int main3(int argc, char** argv)
{
    // главное окно
    Fl_Window win(220, 90);
    // кнопка
    Fl_Button butt(10, 10, 100, 25, "Press");
    // обработчик кнопки, 2-й параметр это главное окно
    butt.callback(butt_cb, &win);
    // окно может менять размер
    win.resizable(win);
    // показать окно
    win.show();
    // ждать закрытия окна
    return(Fl::run());
}
// главная функция
int main(int argc, char** argv)
{
    int rc = 0;
    // задать стиль рисования контролов/виджетов
    Fl::scheme("gtk+");
    // запуск демо
    rc = main3(argc, argv);
    return rc;
}
