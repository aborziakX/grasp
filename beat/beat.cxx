//
// Hello, Beat! program to study FLTK.
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

#include <stdlib.h>

#include "GeObWindow.h"
#include "Cube.h"

using namespace Grasp;

Fl_Window *form;
Fl_Glut_Window* glut_win_main = NULL;
Fl_Slider *slRotX, *slRotY, *slRotZ;
Fl_Button *btnApply;
Fl_Input *inXTop, *inYTop, *inZTop, * inXCam, * inYCam, * inZCam;
Fl_Multiline_Output* text2;
GeObWindow* geob_win = NULL;
Cube *cub1, *cub2;

#define MENUBAR_H 25 // menubar height
#define MARGIN 20    // fixed margin around widgets
#define MARGIN2 (MARGIN * 2)
#define MARGIN3 (MARGIN * 3)

void show_info_cb(Fl_Widget *, void *) {
  // fl_close = "Закрыть";
  fl_message(u8"Закрыть This is an example of using FLTK widgets inside OpenGL windows.\n"
             "Multiple widgets can be added to Fl_geob_windows. They will be\n"
             "rendered as overlays over the scene.");
}

int done = 0; // set to 1 in exit button callback
int Level = 1;
int Rebuild = 0;

// exit button callback
void exit_cb(Fl_Widget *, void *) {
  done = 1;
}

// apply button callback
void apply_cb(Fl_Widget *, void *) {
  try {
    float xTop = (float)atof(inXTop->value());
    float yTop = (float)atof(inYTop->value());
    float zTop = (float)atof(inZTop->value());

    float xCam = (float)atof(inXCam->value());
    float yCam = (float)atof(inYCam->value());
    float zCam = (float)atof(inZCam->value());

    geob_win->SetXCam(xCam);
    geob_win->SetYCam(yCam);
    geob_win->SetZCam(zCam);

    geob_win->SetXTop(xTop);
    geob_win->SetYTop(yTop);
    geob_win->SetZTop(zTop);

    double distance, azimut, elevation;
    geob_win->CalcPolar(distance, azimut, elevation);
    distance += 0.0;
  } catch (...) {
  }
}

void UpdatePosInfo()
{
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

    geob_win->GetPolar(distance, azimut, elevation);
    sprintf_s(buf, "d=%.3f a=%.3f e=%.3f ", distance, azimut, elevation);
    text2->value(buf);
}

//Ключи статуса камеры. Переменные инициализируются нулевыми значениями
//когда клавиши не нажаты
int xOrigin = -1;
int yOrigin = -1;

void mouseButton(int button, int state, int x, int y) 
{
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

// Функцию для обработки движения мыши​ :
void mouseMove(int x, int y) 
{
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
    geob_win->Draw();
    // если OpenGL graphics driver установлен, дать ему шанс
    // рисовать виджеты
    //glut_win_main->display();
}

// обработчик для изменения размеров 
void changeSize(int w, int h)
{
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
void processSpecialKeys(int key, int xx, int yy) {
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
    if (key == 27)
        exit(0);
}
int gl_w = 350; //GL 3D
int gl_h = 350;

void makeform7(const char *name) 
{
  // Widget's XYWH's
  int ct_grp_w = 350; //controls
  int rt_grp_w = 50; //for future

  int form_w = gl_w + ct_grp_w + rt_grp_w + 4 * MARGIN; // main window width
  int form_h = gl_h + MENUBAR_H + 2 * MARGIN;                  // main window height
  int me_bar_x = 0, 
      me_bar_y = 0, 
      me_bar_w = form_w, 
      me_bar_h = MENUBAR_H; // menubar
  int lt_grp_x = 0, 
      lt_grp_y = MENUBAR_H + MARGIN, 
      lt_grp_w = gl_w + ct_grp_w + MARGIN3,
      lt_grp_h = form_h - MENUBAR_H - MARGIN2; // left group
  int lt_cub_x = lt_grp_x + MARGIN, 
      lt_cub_y = lt_grp_y, 
      lt_cub_w = gl_w,
      lt_cub_h = lt_grp_h; // left cube box (GL)
  int ct_grp_x = lt_grp_x + gl_w + MARGIN2,
      ct_grp_y = lt_grp_y, 
      ct_grp_h = lt_grp_h; // center group
  int rt_grp_x = lt_grp_x + lt_grp_w, 
      rt_grp_y = lt_grp_y, 
      rt_grp_h = lt_grp_h; // right group

  // main window
  form = new Fl_Window(form_w, form_h, name);
  form->begin();
  // menu bar
  Fl_Menu_Bar *menubar = new Fl_Menu_Bar(me_bar_x, me_bar_y, me_bar_w, me_bar_h);
  menubar->add(u8"File/Печать", FL_COMMAND + 'p', show_info_cb);
  menubar->add(u8"File/Выход", FL_COMMAND + 'q', exit_cb);
  menubar->add("Edit/Print window2", FL_COMMAND + 'p', show_info_cb);
  menubar->add("Edit/Quit2", FL_COMMAND + 'q', exit_cb);

  // center group inside left group
  Fl_Group *ct_grp = new Fl_Group(ct_grp_x, ct_grp_y, ct_grp_w, ct_grp_h);
  ct_grp->begin();
  //input fields

  btnApply = new Fl_Button(ct_grp_x, form_h - MARGIN - 25, 100, 25, "Apply");
  btnApply->callback(apply_cb);
  ct_grp->end();
  //ct_grp->resizable(slRotX); // only sliders resize vertically, not buttons
  
  // right resizer
  Fl_Box *rt_resizer = new Fl_Box(rt_grp_x - 5, rt_grp_y, 10, rt_grp_h);
  rt_resizer->box(FL_NO_BOX);

  form->end();
  form->resizable(rt_resizer);
  form->size_range(form->w(), form->h()); // minimum window size
}

void setlevel(int value)
{
    Level = value;
    Rebuild = 1;
    glutPostRedisplay();
}

void MenuInit(void)
{
    int submenu3, submenu2, submenu1;

    submenu1 = glutCreateMenu(setlevel);
    glutAddMenuEntry((char*)"0", 0);  glutAddMenuEntry((char*)"1", 1);
    glutAddMenuEntry((char*)"2", 2);  glutAddMenuEntry((char*)"3", 3);
    glutAddMenuEntry((char*)"4", 4);  glutAddMenuEntry((char*)"5", 5);
    glutAddMenuEntry((char*)"6", 6);  glutAddMenuEntry((char*)"7", 7);
    glutAddMenuEntry((char*)"8", 8);

    /*submenu2 = glutCreateMenu(choosefract);
    glutAddMenuEntry((char*)"Moutain", MOUNTAIN);
    glutAddMenuEntry((char*)"Tree", TREE);
    glutAddMenuEntry((char*)"Island", ISLAND);

    submenu3 = glutCreateMenu(agvSwitchMoveMode);
    glutAddMenuEntry((char*)"Flying", FLYING);
    glutAddMenuEntry((char*)"Polar", POLAR);

    glutCreateMenu(handlemenu);
    glutAddSubMenu((char*)"Level", submenu1);
    glutAddSubMenu((char*)"Fractal", submenu2);
    glutAddSubMenu((char*)"Movement", submenu3);
    glutAddMenuEntry((char*)"New Fractal", MENU_RAND);
    glutAddMenuEntry((char*)"Toggle Axes", MENU_AXES);
    glutAddMenuEntry((char*)"Quit", MENU_QUIT);*/
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// FLTK-style callbacks to Glut menu callback translators:
void setlevel(Fl_Widget*, void* value) { setlevel(fl_int(value)); }

int main7(int argc, char **argv) 
{
  Fl::use_high_res_GL(1);
  Fl::set_color(FL_FREE_COLOR, 255, 255, 0, 75);
  bool bDouble = false;
  for (int k = 1; k < argc; k++)
  {
      if (strcmp(argv[k], "-double") == 0) bDouble = true;
  }

  glutInit(&argc, const_cast<char**>(argv));

  // helper
  geob_win = new GeObWindow();
  char buf[33];

  // main window
  makeform7(argv[0]);

  form->begin();

  inXCam = new Fl_Input(700, MENUBAR_H + MARGIN, 50, 20, "xCam:");
  inXCam->tooltip("XCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetXCam());
  inXCam->value(buf);

  inYCam = new Fl_Input(700, MENUBAR_H + 3*MARGIN + inXCam->h(), 50, 20, "yCam:");
  inYCam->tooltip("YCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetYCam());
  inYCam->value(buf);

  inZCam = new Fl_Input(700, MENUBAR_H + 5 * MARGIN + inXCam->h() + inYCam->h(), 50, 20, "zCam:");
  inZCam->tooltip("ZCam input field");
  sprintf_s(buf, "%.3f", geob_win->GetZCam());
  inZCam->value(buf);

  inXTop = new Fl_Input(700, MENUBAR_H + 7 * MARGIN + inXCam->h() + 2*inYCam->h(), 50, 20, "xTop:");
  inXTop->tooltip("XTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetXTop());
  inXTop->value(buf);

  inYTop = new Fl_Input(700, MENUBAR_H + 9 * MARGIN + inXCam->h() + 3*inYCam->h(), 50, 20, "yTop:");
  inYTop->tooltip("ZTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetYTop());
  inYTop->value(buf);

  inZTop = new Fl_Input(700, MENUBAR_H + 11 * MARGIN + inXCam->h() + 4*inYCam->h(), 50, 20, "zTop:");
  inZTop->tooltip("ZTop input field");
  sprintf_s(buf, "%.3f", geob_win->GetZTop());
  inZTop->value(buf);

  text2 = new Fl_Multiline_Output(450, 150, 200, 100, u8"расстояние, азимут, возвышение");
  text2->value("");
  text2->align(FL_ALIGN_BOTTOM);
  text2->tooltip("Fl_Multiline_Output widget.");

  // создаем главное Fl_Glut_Window* glut_win_main
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
  form->show();

  // устанавливаем для него обработчики
  glutReshapeFunc(changeSize);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutDisplayFunc(render);

  // Геометрические объекты
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


  //glutMenuStateFunc(menuuse);
  //MenuInit();

  //glutMainLoop();
  for (;;) {
    if (form->visible()) {
      if (!Fl::check())
        break; // returns immediately
    } else {
      if (!Fl::wait())
        break; // waits until something happens
    }

    //geob_win->SetRotateX((float)slRotX->value());
    //geob_win->SetRotateY((float)slRotY->value());
    //geob_win->SetRotateZ((float)slRotZ->value());
    //geob_win->Draw();
    render();

    if (done)
      break; // exit button was clicked

    Fl::wait(0.01); //in seconds
  }
  //delete main window and all its children
  delete form;
  delete geob_win;
  return 0;
}
//==

int main(int argc, char** argv)
{
    int rc = 0;
    Fl::scheme("gtk+");
    rc = main7(argc, argv);
    return rc;
}


