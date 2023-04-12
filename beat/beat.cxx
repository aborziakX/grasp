//
// Beat - препроцессор для моделирования. Генерирует ini-файл.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//

#ifndef _WINDOWS
#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>

#include <stdlib.h>

#include "GeObWindow.h"
#include "Cube.h"

using namespace Grasp;

Fl_Window *form; // главное окно/форма
Fl_Glut_Window* glut_win_main = NULL; // окно с OpenGl
Fl_Slider *slRotX, *slRotY, *slRotZ; // слайдеры
Fl_Button *btnApply; // кнопка
// ввод текста
Fl_Input *inXTop, *inYTop, *inZTop, * inXCam, * inYCam, * inZCam, * inXLook, * inYLook, * inZLook;
Fl_Multiline_Output* text2; // окно вывода текста
Fl_Pack* pack;
Fl_Scroll* scroll;
GeObWindow* geob_win = NULL; // список геометрических объектов
Cube *cub1, *cub2;

#define MENUBAR_H 25 // высота меню
#define MARGIN 20    // фиксированный отступ вокруг виджетов
#define MARGIN2 (MARGIN * 2)
#define MARGIN3 (MARGIN * 3)

// Помощь
void show_info_cb(Fl_Widget *, void *) 
{
  // fl_close = "Закрыть";
  fl_message(u8"Для работы со сценой использует стрелки Влево, Вправо, Вверх, Вниз, Плюс, Минус.\n"
             "Multiple widgets can be added to Fl_geob_windows.\n"
             "They will be rendered as overlays over the scene.");
}

int done = 0; // set to 1 in exit button callback

// exit button callback
void exit_cb(Fl_Widget *, void *) 
{
  done = 1;
}

// apply button callback
void apply_cb(Fl_Widget *, void *) 
{
  if (done == 1) return;
  try {
    float xTop = (float)atof(inXTop->value());
    float yTop = (float)atof(inYTop->value());
    float zTop = (float)atof(inZTop->value());

    float xCam = (float)atof(inXCam->value());
    float yCam = (float)atof(inYCam->value());
    float zCam = (float)atof(inZCam->value());

    float xLook = (float)atof(inXLook->value());
    float yLook = (float)atof(inYLook->value());
    float zLook = (float)atof(inZLook->value());
    
    geob_win->SetXCam(xCam);
    geob_win->SetYCam(yCam);
    geob_win->SetZCam(zCam);

    geob_win->SetXTop(xTop);
    geob_win->SetYTop(yTop);
    geob_win->SetZTop(zTop);

    geob_win->SetXLook(xLook);
    geob_win->SetYLook(yLook);
    geob_win->SetZLook(zLook);


    double distance, azimut, elevation;
    geob_win->CalcPolar(distance, azimut, elevation);
    distance += 0.0;
  } catch (...) {
  }
}

void UpdatePosInfo()
{
    if (done == 1) return;
    double distance, azimut, elevation;
    geob_win->GetPolar(distance, azimut, elevation);
    char buf[33];
    sprintf_s(buf, "%.3f", geob_win->GetXCam());
    inXCam->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYCam());
    inYCam->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZCam());
    inZCam->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetXTop());
    inXTop->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYTop());
    inYTop->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZTop());
    inZTop->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetXLook());
    inXLook->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYLook());
    inYLook->value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZLook());
    inZLook->value(buf);

    geob_win->GetPolar(distance, azimut, elevation);
    sprintf_s(buf, "d=%.3f a=%.3f e=%.3f ", distance, azimut, elevation);
    text2->value(buf);
}

//Ключи статуса камеры. Переменные инициализируются нулевыми значениями
//когда клавиши не нажаты
int xOrigin = -1;
int yOrigin = -1;

// Функция для обработки нажатия мыши
void mouseButton(int button, int state, int x, int y) 
{
    if (done == 1) return;
    // только при начале движения, если нажата левая кнопка
    if (button == GLUT_LEFT_BUTTON) 
    {
        // когда кнопка отпущена
        if (state == GLUT_UP) {
            xOrigin = -1;
            yOrigin = -1;
            UpdatePosInfo();
        }
        else {// state = GLUT_DOWN
            xOrigin = x;
            yOrigin = y;
        }
    }
}

// Функцию для обработки движения мыши​
void mouseMove(int x, int y) 
{
    if (done == 1) return;
    // если левая кнопка опущена
    if (xOrigin >= 0)
    {
        double distance, azimut, elevation;
        geob_win->GetPolar(distance, azimut, elevation);
        // обновить направления камеры
        double fraction = 0.001;
        azimut -= (x - xOrigin) * fraction;
        elevation -= (y - yOrigin) * fraction;

        geob_win->SetPolar(distance, azimut, elevation);
    }
}

// Функция рисования через помощник 
void render() 
{
    if (done == 1) return;
    geob_win->Draw();
    // если OpenGL graphics driver установлен, дать ему шанс
    // рисовать виджеты
    //glut_win_main->display();
}

// обработчик для изменения размеров 
void changeSize(int w, int h)
{
    if (done == 1) return;
    // предупредим деление на ноль
    // если окно сильно перетянуто будет
    if (h == 0)
        h = 1;
    float ratio = 1.0f * w / h;

    // используем матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // сброс матрицы
    glLoadIdentity();

    // определяем окно просмотра
    glViewport(0, 0, w, h);

    // установить корректную перспективу.
    gluPerspective(45, ratio, 1, 1000);

    // вернуться к модели
    glMatrixMode(GL_MODELVIEW);
}

//Функция обработки нажатия клавиш перемещения
//Более естественно, когда по стрелке вправо сцена вращается вправо (против часовой), поэтому азимуи уменьшаем (камера смещаетсч влево)
void processSpecialKeys(int key, int xx, int yy) 
{
    if (done == 1) return;
    if (key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT && key != GLUT_KEY_UP && key != GLUT_KEY_DOWN)
        return;

    double distance, azimut, elevation;
    geob_win->GetPolar(distance, azimut, elevation);

    double fraction = 0.1;
    switch (key) {
    case GLUT_KEY_LEFT:
        azimut += fraction;
        break;
    case GLUT_KEY_RIGHT:
        azimut -= fraction;
        break;
    case GLUT_KEY_UP:
        elevation -= fraction;
        break;
    case GLUT_KEY_DOWN:
        elevation += fraction;
        break;
    }

    geob_win->SetPolar(distance, azimut, elevation);

    UpdatePosInfo();

    render();
}

//Функция обработки нажатия обычных клавиш
void processNormalKeys(unsigned char key, int xx, int yy) 
{
    double distance, azimut, elevation;
    if (key == 27) //Esc
        exit(0);
    else if (key == 61) //+
    {
        geob_win->GetPolar(distance, azimut, elevation);
        distance *= 0.9;
        geob_win->SetPolar(distance, azimut, elevation);
        UpdatePosInfo();
        render();
    }
    else if (key == 45) //-
    {
        geob_win->GetPolar(distance, azimut, elevation);
        distance *= 1.1;
        geob_win->SetPolar(distance, azimut, elevation);
        UpdatePosInfo();
        render();
    }
}

int gl_w = 350; //ширина GL окна
int gl_h = 350; //высота GL окна

// создать главное окно и виджеты
void MakeForm(const char *name) 
{
  int ct_grp_w = 450; // ширина области виджетов
  int rt_grp_w = 10; // для будущего

  int form_w = gl_w + ct_grp_w + rt_grp_w + 4 * MARGIN; // ширина главного окна
  int form_h = gl_h + MENUBAR_H + 2 * MARGIN;           // высота главного окна
  // XYWH'меню
  int me_bar_x = 0, 
      me_bar_y = 0, 
      me_bar_w = form_w, 
      me_bar_h = MENUBAR_H; 
  // XYWH'центральная группа
  int ct_grp_x = gl_w + MARGIN2,
      ct_grp_y = MENUBAR_H + MARGIN/2,
      ct_grp_h = form_h - MENUBAR_H - MARGIN; 
  // XYWH'правая группа
  int rt_grp_x = ct_grp_x + ct_grp_w, 
      rt_grp_y = ct_grp_y, 
      rt_grp_h = ct_grp_h;
  char buf[33];

  // главное окно
  form = new Fl_Window(form_w, form_h, name);
  form->begin();

  // меню
  Fl_Menu_Bar *menubar = new Fl_Menu_Bar(me_bar_x, me_bar_y, me_bar_w, me_bar_h);
  menubar->add(u8"File/Печать", FL_COMMAND + 'p', show_info_cb);
  menubar->add(u8"File/Выход", FL_COMMAND + 'q', exit_cb);
  menubar->add("Edit/Print window2", FL_COMMAND + 'p', show_info_cb);
  menubar->add("Edit/Quit2", FL_COMMAND + 'q', exit_cb);
  menubar->add("Help", FL_COMMAND + 'h', show_info_cb);

  // центральная группа
  Fl_Group *ct_grp = new Fl_Group(ct_grp_x, ct_grp_y, ct_grp_w, ct_grp_h);
  ct_grp->begin();

  //list box объектов
  scroll = new Fl_Scroll(ct_grp_x + MARGIN, MENUBAR_H + MARGIN, 140, 200);
  pack = new Fl_Pack(ct_grp_x + MARGIN, MENUBAR_H + MARGIN, 140, 200);
  pack->box(FL_DOWN_FRAME);
  int nbuttons = 24;
  // create buttons: position (xx, xx) will be "fixed" by Fl_Pack/Fl_Flex
  int xx = 35;
  for (int i = 0; i < nbuttons; i++) {
      char ltxt[8];
      snprintf(ltxt, 8, "b%d", i + 1);
      Fl_Button* b = new Fl_Button(xx, xx, 25, 25);
      b->copy_label(ltxt);
      xx += 10;
  }
  pack->end();
  scroll->end();

  // поля ввода
  int wiIn = 50, heIn = 20, xIn = 700, yIn = MENUBAR_H + MARGIN;
  inXCam = new Fl_Input(xIn, yIn, wiIn, heIn, "xCam:");
  inXCam->tooltip("XCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetXCam());
  inXCam->value(buf);

  inYCam = new Fl_Input(xIn, yIn + (MARGIN + heIn), wiIn, heIn, "yCam:");
  inYCam->tooltip("YCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetYCam());
  inYCam->value(buf);

  inZCam = new Fl_Input(xIn, yIn + 2*(MARGIN + heIn), wiIn, heIn, "zCam:");
  inZCam->tooltip("ZCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetZCam());
  inZCam->value(buf);

  inXTop = new Fl_Input(xIn, yIn + 3*(MARGIN + heIn), wiIn, heIn, "xTop:");
  inXTop->tooltip("XTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetXTop());
  inXTop->value(buf);

  inYTop = new Fl_Input(xIn, yIn + 4*(MARGIN + heIn), wiIn, heIn, "yTop:");
  inYTop->tooltip("ZTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetYTop());
  inYTop->value(buf);

  inZTop = new Fl_Input(xIn, yIn + 5*(MARGIN + heIn), wiIn, heIn, "zTop:");
  inZTop->tooltip("ZTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetZTop());
  inZTop->value(buf);

  inXLook = new Fl_Input(xIn, yIn + 6 * (MARGIN + heIn), wiIn, heIn, "xLook:");
  inXLook->tooltip("XLook input field");
  sprintf_s(buf, "%.3f", geob_win->GetXLook());
  inXLook->value(buf);

  inYLook = new Fl_Input(xIn, yIn + 7 * (MARGIN + heIn), wiIn, heIn, "yLook:");
  inYLook->tooltip("YLook input field");
  sprintf_s(buf, "%.3f", geob_win->GetYLook());
  inYLook->value(buf);

  inZLook = new Fl_Input(xIn, yIn + 8 * (MARGIN + heIn), wiIn, heIn, "zLook:");
  inZLook->tooltip("ZLook input field");
  sprintf_s(buf, "%.3f", geob_win->GetZLook());
  inZLook->value(buf);


  text2 = new Fl_Multiline_Output(ct_grp_x + MARGIN, 250, 200, 100, u8"расстояние, азимут, возвышение");
  text2->value("");
  text2->align(FL_ALIGN_BOTTOM);
  text2->tooltip("Fl_Multiline_Output widget.");

  btnApply = new Fl_Button(ct_grp_x + MARGIN, form_h - MARGIN - 25, 100, 25, u8"Применить");
  btnApply->callback(apply_cb);

  ct_grp->end();
  ct_grp->box(FL_BORDER_BOX);
  //ct_grp->resizable(slRotX); // only sliders resize vertically, not buttons
  
  // right resizer
  Fl_Box *rt_resizer = new Fl_Box(rt_grp_x - 5, rt_grp_y, 10, rt_grp_h);
  rt_resizer->box(FL_NO_BOX);

  form->end();
  form->resizable(rt_resizer);
  form->size_range(form->w(), form->h()); // minimum window size
}

// создаем главное Fl_Glut_Window* glut_win_main
void MakeGlWindow(bool bDouble)
{
    form->begin();

    unsigned int mode = GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE;
    if (bDouble) mode |= GLUT_DOUBLE;
    glutInitDisplayMode(mode);
    glutInitWindowPosition(MARGIN, MENUBAR_H + MARGIN);
    glutInitWindowSize(gl_w, gl_h);
    glutCreateWindow("Glut Window");
    glut_win_main = glut_window;

    /*glut_win_main->begin();
    Fl_Widget *w = new Fl_Button(10, 10, 120, 30, "FLTK over GL");
    w->color(FL_FREE_COLOR);
    w->box(FL_BORDER_BOX);
    w->callback(show_info_cb);
    glut_win_main->end();

    geob_win->win_glut = glut_win_main;*/

    form->end();
    form->resizable(glut_win_main);

    // устанавливаем для него обработчики
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutDisplayFunc(render);
}

int main(int argc, char **argv) 
{
  Fl::use_high_res_GL(1);
  Fl::set_color(FL_FREE_COLOR, 255, 255, 0, 75);
  bool bDouble = false;
  for (int k = 1; k < argc; k++)
  {
      if (strcmp(argv[k], "-double") == 0) bDouble = true;
  }
  glutInit(&argc, const_cast<char**>(argv));

  // создаем Хранилище геометрических объектов
  geob_win = new GeObWindow();

  // создаем главное окно и виджеты
  MakeForm(argv[0]);

  // создаем главное Fl_Glut_Window* glut_win_main
  MakeGlWindow(bDouble);

  form->show();

  // создать Геометрические объекты
  cub1 = new Cube();
  cub1->Translate(1.0f, 0.0f, 0.0f);
  geob_win->Add(cub1);
  Facet3 *fac = cub1->GetFacet(0);
  if (fac != NULL) {
    fac->SetColor(255, 0, 0);
  }

  cub2 = new Cube();
  geob_win->Add(cub2);
  Facet3 *fac2 = cub2->GetFacet(0);
  if (fac2 != NULL)
    fac2->SetColor(0, 0, 255);
  cub2->Transform();
  cub2->Translate(-1.5, 0, 0);

  //аналог glutMainLoop(); или Fl::run
  for (;;) 
  {
    if (form->visible()) 
    { //проверить статус
      if (!Fl::check())
        break; // выход немедленно
    } 
    else 
    { // ждать событий
      if (!Fl::wait())
        break;
    }

    //geob_win->SetRotateX((float)slRotX->value());
    //geob_win->SetRotateY((float)slRotY->value());
    //geob_win->SetRotateZ((float)slRotZ->value());
    //geob_win->Draw();

    render();

    if (done)
      break; // 'exit button' была нажата

    Fl::wait(0.01); // заснуть в секундах
  }

  // удалить главное окно и виджеты
  delete form;
  // удалить Хранилище
  delete geob_win;
  return 0;
}
//==



