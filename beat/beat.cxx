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
#include "Cyl.h"
#include "Lines.h"
#include "Dialogs.h"

using namespace Grasp;

Fl_Window *form; // главное окно/форма
Fl_Glut_Window* glut_win_main = NULL; // окно с OpenGl
Fl_Slider *slRotX, *slRotY, *slRotZ; // слайдеры
Fl_Button *btnApply; // кнопка
// ввод текста
//Fl_Input *inXTop, *inYTop, *inZTop, * inXCam, * inYCam, * inZCam, * inXLook, * inYLook, * inZLook;
Fl_Multiline_Output* text2; // окно вывода текста
Fl_Pack* pack;
Fl_Scroll* scroll;
GeObWindow* geob_win = NULL; // список геометрических объектов
GeOb* geobCurrent = NULL; // текущий объект
Fl_Widget* lbBtnCurrent = NULL; // текущая кнопка в LB

// диалоги
Pass_Window* pass_window = NULL;
AddCubeDialog* add_cube_dlg = NULL;
CameraXyzDialog* camera_xyz_dlg = NULL;


#define MENUBAR_H 25 // высота меню
#define MARGIN 20    // фиксированный отступ вокруг виджетов
#define MARGIN2 (MARGIN * 2)
#define MARGIN3 (MARGIN * 3)

void FillListBox();

// Помощь
void help_cb(Fl_Widget *, void *) 
{
  // fl_close = "Закрыть";
  fl_message(u8"Для работы со сценой использует стрелки Влево, Вправо, Вверх, Вниз, Плюс, Минус.\n"
             "Multiple widgets can be added to Fl_geob_windows.\n"
             "They will be rendered as overlays over the scene.");
}

void file_open_cb(Fl_Widget*, void*)
{
}
void file_save_cb(Fl_Widget*, void*)
{
}


// нажатие кнопки в listbox
void listbox_cb(Fl_Widget* bt, void* ud)
{
    char buf[333];
    if (ud != NULL)
    {
        //((GeOb*)ud)->GetName(buf);
        if (lbBtnCurrent == bt) 
            return;

        if(geobCurrent != NULL)
            geobCurrent->bSelect = false;
        geobCurrent = (GeOb*)ud;
        geobCurrent->bSelect = true;

        if (lbBtnCurrent != NULL)
        {
            //lbBtnCurrent->activate();
            lbBtnCurrent->color(FL_BACKGROUND_COLOR);
            lbBtnCurrent->redraw();
        }
        //bt->deactivate();
        bt->color(FL_SELECTION_COLOR);
        lbBtnCurrent = bt;
    }
    //else snprintf(buf, 333, "%s", u8"Закрыть.");
    //fl_message(buf);
}

// удалить объект
void delete_cb(Fl_Widget*, void* ud)
{
    if (geobCurrent == NULL)
    {
        fl_message(u8"Объект не выбран!");
        return;
    }
    char buf[333], buf2[333];
    geobCurrent->GetName(buf2);
    snprintf(buf, 333, u8"[%s] будет удален. Продолжить?", buf2);
    int rep = fl_choice(buf, u8"Нет", u8" Да ", 0L);
    if (rep == 1)
    {
        geob_win->Delete(geobCurrent);
        geobCurrent = NULL;
        lbBtnCurrent = NULL;
        FillListBox();
    }
}

void pass_window_done_cb(Fl_Widget* bt, void* ud)
{
    pass_window->hide();
}

void add_cube_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    { //Ok
        double x, y, z, xSc, ySc, zSc;
        bool bSuc = add_cube_dlg->GetPos(x, y, z, xSc, ySc, zSc);
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }
        Cube* cub3 = new Cube();
        cub3->Scale(xSc, ySc, zSc);
        cub3->Translate(x, y, z);
        geob_win->Add(cub3);
        FillListBox();
    }
    add_cube_dlg->hide();
}

void add_cube_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 0)
    {
        //fl_message(u8"Куб");
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->show();
    }
    if (m == 1)
    {       
        if (pass_window == NULL)
            pass_window = new Pass_Window(pass_window_done_cb);
        pass_window->show();
    }
    if (m == 2)
    {
        fl_message(u8"Лин");
    }
}

//камера XYZ завершена
void camera_xyz_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    {   //Ok
        double xCam, yCam, zCam, xLook, yLook, zLook, xTop, yTop, zTop;
        bool bSuc = camera_xyz_dlg->GetPos(xCam, yCam, zCam, xLook, yLook, zLook, xTop, yTop, zTop);
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }
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
    }

    camera_xyz_dlg->hide();
}
//открыть камера XYZ диалог
void camera_xyz_cb(Fl_Widget*, void*)
{
    if (camera_xyz_dlg == NULL)
        camera_xyz_dlg = new CameraXyzDialog(camera_xyz_done_cb);
    camera_xyz_dlg->Init(geob_win);
    camera_xyz_dlg->show();
}

void camera_sph_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    {   //Ok
    }
}
void camera_sph_cb(Fl_Widget*, void*)
{
}

int done = 0; // set to 1 in exit button callback

// exit button callback
void exit_cb(Fl_Widget *, void *) 
{
  done = 1;
}

void UpdatePosInfo()
{
    if (done == 1) return;
    double distance, azimut, elevation;
    geob_win->GetPolar(distance, azimut, elevation);
    char buf[33];
    sprintf_s(buf, "d=%.3f a=%.3f e=%.3f ", distance, azimut, elevation);
    text2->value(buf);
}

//Координаты мыши. Переменные инициализируются значениями -1
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

int m1 = 0; //куб
int m2 = 1; //цил.
int m3 = 2; //лин.

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
  menubar->add(u8"Файл/Открыть", FL_COMMAND + 'o', file_open_cb);
  menubar->add(u8"Файл/Сохранить", FL_COMMAND + 's', file_save_cb);
  menubar->add(u8"Файл/Выход", FL_COMMAND + 'q', exit_cb);

  menubar->add(u8"Проект/Камера XYZ", FL_COMMAND + 'x', camera_xyz_cb);
  menubar->add(u8"Проект/Камера сферично", FL_COMMAND + 'r', camera_sph_cb);

  menubar->add(u8"Геометрия/Добавить/Куб", FL_COMMAND + 'u', add_cube_cb, (void*)&m1);
  menubar->add(u8"Геометрия/Добавить/Цилиндр", FL_COMMAND + 'y', add_cube_cb, (void*)&m2);
  menubar->add(u8"Геометрия/Добавить/Линии", FL_COMMAND + 'l', add_cube_cb, (void*)&m3);
  menubar->add(u8"Геометрия/Удалить", FL_COMMAND + 'd', delete_cb);

  menubar->add(u8"Помощь", FL_COMMAND + 'h', help_cb);

  // центральная группа
  Fl_Group *ct_grp = new Fl_Group(ct_grp_x, ct_grp_y, ct_grp_w, ct_grp_h);
  ct_grp->begin();

  //list box объектов
  scroll = new Fl_Scroll(ct_grp_x + MARGIN, MENUBAR_H + MARGIN2, 140, 200, u8"объекты");
  scroll->begin();
  pack = new Fl_Pack(ct_grp_x + MARGIN, MENUBAR_H + MARGIN2, 140, 200);
  pack->box(FL_DOWN_FRAME);
  scroll->end();

  text2 = new Fl_Multiline_Output(ct_grp_x + MARGIN, 300, 200, 50, u8"расстояние, азимут, возвышение");
  text2->value("");
  text2->align(FL_ALIGN_BOTTOM);
  text2->tooltip("Fl_Multiline_Output widget.");

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

    geob_win->win_glut = glut_win_main;

    /*glut_win_main->begin(); //не работает
    Fl_Widget *w = new Fl_Button(10, 10, 120, 30, "FLTK over GL");
    w->color(FL_FREE_COLOR);
    w->box(FL_BORDER_BOX);
    w->callback(show_info_cb);
    glut_win_main->end();*/
    
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

// заполнить listbox с объектами
void FillListBox()
{
    for (int i = pack->children() - 1; i >= 0; i--) {
        pack->delete_child(i);
    }
    pack->begin();
    int nbuttons = geob_win->GetSize();
    // create buttons: position (xx, xx) will be "fixed" by Fl_Pack/Fl_Flex
    int xx = 35;
    char ltxt[33];
    for (int i = 0; i < nbuttons; i++) {       
        GeOb * ob = geob_win->GetObj(i);
        if (ob == NULL) continue;
        ob->GetName(ltxt);
        Fl_Button* b = new Fl_Button(xx, xx, 25, 25);
        b->copy_label(ltxt);
        b->callback(listbox_cb);
        b->user_data((void*)ob);
        b->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
        xx += 10;
    }
    pack->end();
    pack->redraw();
    scroll->redraw();
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
  Cube * cub1 = new Cube();
  cub1->Translate(1.0f, 0.0f, 0.0f);
  cub1->bNormal = true;
  geob_win->Add(cub1);
  Facet3 *fac = cub1->GetFacet(0); //задняя
  if (fac != NULL) {
    fac->SetColor(255, 0, 0);
  }
  fac = cub1->GetFacet(1); //передняя
  if (fac != NULL) {
      fac->SetColor(0, 255, 255);
  }

  Cube * cub2 = new Cube();
  cub2->Translate(-1.5, 0, 0);
  cub2->bWire = true;
  geob_win->Add(cub2);
  Facet3 *fac2 = cub2->GetFacet(0); //задняя
  if (fac2 != NULL)
    fac2->SetColor(0, 0, 255);

  Cube * cub3 = new Cube();
  cub3->Scale(1.0, 0.5, 2.0);
  cub3->Translate(0, -1.5, 0);
  geob_win->Add(cub3);

  Cyl* cyl1 = new Cyl(20);
  cyl1->Scale(1.0, 0.5, 2.0);
  cyl1->Translate(0, 1.5, 0);
  geob_win->Add(cyl1);

  Lines* ln1 = new Lines();
  Vec3 v1, v2;
  v1.Copy(2, 1, 1);
  v2.Copy(2, 5, 1);
  ln1->AddLine(v1, v2);
  v2.Copy(2, 5, 1);
  v2.Copy(5, 5, 1);
  ln1->AddLine(v1, v2);
  ln1->SetColor(255, 192, 0);
  geob_win->Add(ln1);

  // заполнить listbox с объектами
  FillListBox();

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

    Fl::wait(0.01); // заснуть в секундах = 10 ms
  }

  // удалить главное окно и виджеты
  delete form;
  // удалить Хранилище
  delete geob_win;

  // удалить диалоги
  if (add_cube_dlg != NULL)
  {
      delete add_cube_dlg;
      add_cube_dlg = NULL;
  }
  if (pass_window != NULL)
  {
      delete pass_window;
      pass_window = NULL;
  }
  if (camera_xyz_dlg != NULL)
  {
      delete camera_xyz_dlg;
      camera_xyz_dlg = NULL;
  }
  return 0; 
}
//==



