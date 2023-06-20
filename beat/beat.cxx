//
// Beat - препроцессор для моделирования. Генерирует ini-файл.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//
// В этом файле создается интерфейс пользователя и расположены обработчики событий

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
#include <FL/Fl_Native_File_Chooser.H>

#include <stdlib.h>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif

#include "GeObWindow.h"
#include "Cube.h"
#include "Cyl.h"
#include "Lines.h"
#include "Cone.h"
#include "Sphere.h"
#include "Gadget.h"
#include "Poly.h"
#include "Dialogs.h"
#include "beat_ini.h"
#include "Utils.h"

#define RUSSIAN
#include "messages.h"

using namespace Grasp;

Fl_Window *form; // главное окно/форма
Fl_Glut_Window* glut_win_main = NULL; // окно с OpenGl
//Fl_Slider *slRotX, *slRotY, *slRotZ; // слайдеры
Fl_Button *btnApply; // кнопка
// ввод текста
//Fl_Input *inXTop, *inYTop, *inZTop, * inXCam, * inYCam, * inZCam, * inXLook, * inYLook, * inZLook;
Fl_Multiline_Output* text2; // окно вывода текста
Fl_Pack* pack;
Fl_Scroll* scroll;
GeObWindow* geob_win = NULL; // список геометрических объектов
GeOb* geobCurrent = NULL; // текущий объект
Fl_Widget* lbBtnCurrent = NULL; // текущая кнопка в LB
BeatIni* beatIni = NULL; // загрузчик ini

// диалоги
Pass_Window* pass_window = NULL;
AddCubeDialog* add_cube_dlg = NULL;
CameraXyzDialog* camera_xyz_dlg = NULL;
CameraSphDialog* camera_sph_dlg = NULL;
PhysPropDialog* phys_dlg = NULL;
BeatDialog* bt_dlg = NULL;

// высота меню
#define MENUBAR_H 25
// фиксированный отступ вокруг виджетов
#define MARGIN 20
#define MARGIN2 (MARGIN * 2)
#define MARGIN3 (MARGIN * 3)
#define MARGIN4 (MARGIN * 4)

char cCurrentPath[FILENAME_MAX]; // для текущей директории

void FillListBox();
void bt_dlg_done_cb(Fl_Widget* bt, void* ud);
void file_save_as_cb(Fl_Widget*, void*);
void add_cube_done_cb(Fl_Widget*, void*);

// чтобы диалог запускаемый по нажатию кнопки стал popup, его надо проинициировать из меню
void PrepareBeatDialog()
{
    if (beatIni != NULL && bt_dlg == NULL)
    {
        Utils::ErrorLog(L"popup");
        TDialog* dlg = beatIni->lstDlg.size() > 0 ? beatIni->lstDlg[0] : NULL;
        bt_dlg = new BeatDialog(bt_dlg_done_cb);
        bt_dlg->Init(dlg);
        //bt_dlg->show();
    }
}

// Помощь
void help_cb(Fl_Widget *, void *) 
{
  // fl_close = "Закрыть";
  fl_message(u8"Сначала надо создать проект или открыть имеющийся.\n"
    "Затем множественные геометрические объекты могут добавлену в сцену.\n"
    "Для работы со сценой используйте стрелки Влево, Вправо, Вверх, Вниз, Плюс, Минус.\n"
    "В итоге можно сгенерировать ini-файл.\n"
  );
  PrepareBeatDialog();
}

bool IsReady()
{
    if (beatIni == NULL)
    {
        fl_message(u8"Нет проекта");
        return false;
    }
    return true;
}

bool IsSelected()
{
    if (geobCurrent == NULL)
    {
        fl_message(u8"Объект не выбран!");
        return false;
    }
    return true;
}

// выбор файла
string file_open_dialog(const char * sTitle, const char * sFilter, bool bNew = false)
{
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return "";
    }
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    //fl_message(cCurrentPath);
    //return "";

    string sIniFile;
    // создать "нативный быбор файла" диалог
    Fl_Native_File_Chooser native;
    native.title(sTitle);
    int t = bNew ? Fl_Native_File_Chooser::BROWSE_SAVE_FILE : // need this if file doesn't exist yet
        Fl_Native_File_Chooser::BROWSE_FILE;
    native.type(t);
    native.filter(sFilter);
    native.directory(cCurrentPath);
    //native.preset_file(G_filename->value());
    // Show native chooser
    switch (native.show()) {
    case -1: //G_tty->printf("ERROR: %s\n", native.errmsg()); 
        fl_beep(); break;      // ERROR
    case  1: //G_tty->printf("*** CANCEL\n"); 
        break;           // CANCEL
    default:                                                            // PICKED FILE
        if (native.filename()) {
            sIniFile = native.filename();
            //G_tty->printf("filename='%s'\n", native.filename());
        }
        else {
            fl_message("NULL");
            //G_tty->printf("filename='(null)'\n");
        }
        break;
    }
    return sIniFile;
}

// Открыть Проект
void file_open_cb(Fl_Widget*, void*)
{
    string sIniFile = file_open_dialog(MES_SELECT_FILE, MES_BPJ_FILTER);
    if (sIniFile.empty()) return;

    //fl_message(sIniFile.c_str());
    if (beatIni == NULL)
    {
        beatIni = new BeatIni(geob_win);
        Utils::ErrorLog(L"Я started2!");
        //const wstring sMessageJpn[] = { L"Я started!" };
        //beatIni->WriteUnicodeUTF8toFile("error.log", sMessageJpn, 1, true);
    }
    int n = beatIni->LoadIni(sIniFile.c_str());
    cout << "\nn=" << n;
    PrepareBeatDialog();
    FillListBox();

    //text2->value(beatIni->lstVal[1]->c_str()); //test
    
    //beatIni->ReadUtf8UnicodeFile(sIniFile.c_str());  //test  
    //wstring data = beatIni->readFile(sIniFile.c_str());
    //wcout << "\ndata-" << data; //Linux ok
    //std::string ss = BeatIni::wstring_to_utf8(data);
    //text2->value(ss.c_str());
}

// Сохранить Проект
void file_save_cb(Fl_Widget*, void*)
{
    if (!IsReady()) return;
    //fl_message(u8"file_save_cb");
    if (!beatIni->filename.empty())
        beatIni->Save(beatIni->filename.c_str());
    else file_save_as_cb(0, 0);
}

// Создать Проект
void file_create_cb(Fl_Widget*, void*)
{
    string sIniFile = file_open_dialog(MES_SELECT_TEMPL, MES_BTT_FILTER);
    if (sIniFile.empty()) return;

    if (beatIni == NULL)
    {
        beatIni = new BeatIni(geob_win);
        Utils::ErrorLog(L"Я started2!");
    }
    int n = beatIni->LoadIni(sIniFile.c_str());
    cout << "\nn=" << n;
    PrepareBeatDialog();
    FillListBox();
    beatIni->filename = ""; // empty
}

// Сохранить как Проект
void file_save_as_cb(Fl_Widget*, void*)
{
    if (!IsReady()) return;
    string sIniFile = file_open_dialog(MES_SELECT_FILE, MES_BPJ_FILTER, true);
    if (sIniFile.empty()) return;

    //fl_message(sIniFile.c_str());
    beatIni->Save(sIniFile.c_str());
}

// Генерировать ini
void file_gener_cb(Fl_Widget*, void*)
{
    if (!IsReady()) return;
    string sIniFile = file_open_dialog(MES_SELECT_FILE, MES_INI_FILTER, true);
    if (sIniFile.empty()) return;

    //fl_message(file_gener_cb.c_str());
    beatIni->GenerateIni(sIniFile.c_str());
}

// Геометрия/Изменить
void edit_cb(Fl_Widget*, void*)
{
    if (!IsReady()) return;
    //fl_message(u8"edit_cb");
    if (!IsSelected()) return;

    geom_type_enum geom_type = geobCurrent->GetGeomType();
    if (add_cube_dlg == NULL)
        add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
    add_cube_dlg->Init(geobCurrent, geom_type);
    add_cube_dlg->show();
}

// Геометрия/Свойства 
void features_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    { //Ok
        int id = geobCurrent->GetIndex();
        TMolecule* mol = beatIni->FindMolecule(id);
        TParam* par = mol->FeatureByName("color");
        string old = "";
        if (par != NULL) old = par->pcurr;

        bool bSuc = phys_dlg->GetPos();
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }
        if (par != NULL && par->pcurr != old)
        {
            unsigned char _red, _green, _blue;
            int clrInd = atoi(par->pcurr.c_str());
            Facet3::GetColorByIndex(clrInd, _red, _green, _blue);
            geobCurrent->SetColor(_red, _green, _blue);
        }
    }
    phys_dlg->hide();
}
void features_cb(Fl_Widget*, void*)
{
    if (!IsReady()) return;
    if (!IsSelected()) return;
    int id = geobCurrent->GetIndex();
    TMolecule * mol = beatIni->FindMolecule(id);
    if (mol == NULL)
    {
        fl_message(u8"физ.объект не найден!");
        return;
    }
    //fl_message(mol->objname.c_str());
    
    if (phys_dlg == NULL)
        phys_dlg = new PhysPropDialog(features_done_cb);
    phys_dlg->Init(mol);
    phys_dlg->show();
}

// нажатие кнопки геом.объекта в listbox
void listbox_cb(Fl_Widget* bt, void* ud)
{
    //char buf[333] = { 0 };
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

// диалог завершен
void bt_dlg_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    { //Ok
        bool bSuc = bt_dlg->GetPos();
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }
    }
    bt_dlg->hide();
}

// нажатие кнопки диалога в listbox
void listbox_dial_cb(Fl_Widget* bt, void* ud)
{
    if (ud != NULL)
    {
        TDialog* dlg = ((TDialog*)ud);
        //fl_message(dlg->dname.c_str());

        if (bt_dlg == NULL)
            bt_dlg = new BeatDialog(bt_dlg_done_cb);
        //bt_dlg->set_modal();
        bt_dlg->Init(dlg);
        bt_dlg->show();
    }
}

// удалить объект
void delete_cb(Fl_Widget*, void* ud)
{
    if (!IsReady()) return;
    if (!IsSelected()) return;

    char buf[333] = { 0 }, buf2[333] = { 0 };
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

// demo
void pass_window_done_cb(Fl_Widget* bt, void* ud)
{
    pass_window->hide();
}

const int m_0 = 0; // куб
const int m_1 = 1; // цил.
const int m_2 = 2; // лин.
const int m_3 = 3; // Конус
const int m_4 = 4; // Сфера
const int m_5 = 5; // Точка учета
const int m_6 = 6; // Полимер

// Добавить завершено
void add_cube_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    GeOb* cub3 = NULL;
    unsigned char _red, _green, _blue;
    bool bNew = false;
    if (m == 1)
    { //Ok
        double x, y, z, xSc, ySc, zSc;
        int nSide;
        bool bSuc = add_cube_dlg->GetPos(x, y, z, xSc, ySc, zSc, nSide, _red, _green, _blue);
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }

        if (add_cube_dlg->geob != NULL)
        {
            int ns = add_cube_dlg->geob->GetNside();
            if (ns != nSide)
            {
                geob_win->ClearFacets();
                add_cube_dlg->geob->SetNside(nSide);
                geob_win->UpdateFacets();
            }
        }

        if (add_cube_dlg->geob == NULL)
        {
            if (nSide < 3) nSide = 3;
            if (add_cube_dlg->geom_type == geom_type_enum::GO_BOX)
                cub3 = new Cube();
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_CYLINDER)
                cub3 = new Cyl(nSide);
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_LINES)
            {
                Lines * ln = new Lines();
                Vec3 v1, v2;
                v1.Copy(x, y, z);
                v2.Copy(xSc, ySc, zSc);
                ln->AddLine(v1, v2);
                cub3 = ln;
            }
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_TETRA)
                cub3 = new Cone(nSide);
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_SPHERE)
                cub3 = new Sphere(nSide);
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_GADGET)
            { // Точка учета
                cub3 = new Gadget();
                cub3->Scale(0.2, 0.2, 0.2);
                cub3->Translate(x, y, z);
            }
            else if (add_cube_dlg->geom_type == geom_type_enum::GO_POLY)
                cub3 = new Poly("C:\\grasp\\beat\\vrml\\scene.wrl");

            geob_win->Add(cub3);
            beatIni->AddGeOb(cub3);
            bNew = true;
        }
        else
        {
            cub3 = add_cube_dlg->geob;
            if (add_cube_dlg->geom_type == geom_type_enum::GO_LINES)
            {
                Facet3* f = cub3->GetFacet(0);
                Vec3* v1 = f->GetPoint(0);
                v1->SetX(x);
                v1->SetY(y);
                v1->SetZ(z);

                v1 = f->GetPoint(1);
                v1->SetX(xSc);
                v1->SetY(ySc);
                v1->SetZ(zSc);
            }
            if (add_cube_dlg->geom_type == geom_type_enum::GO_GADGET)
            { // Точка учета
                cub3->Translate(x, y, z);
            }
        }

        if (add_cube_dlg->geom_type != geom_type_enum::GO_LINES && 
            add_cube_dlg->geom_type != geom_type_enum::GO_GADGET)
        {
            cub3->Scale(xSc, ySc, zSc);
            cub3->Translate(x, y, z);
        }
        if(bNew) FillListBox();

        cub3->SetColor(_red, _green, _blue);
    }
    add_cube_dlg->hide();
}

// Добавить
void add_cube_cb(Fl_Widget* bt, void* ud)
{
    if (!IsReady()) return;
    int m = *(int*)ud;
    if (m == m_0)
    {   // Куб
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_BOX);
        add_cube_dlg->show();
    }
    else if (m == m_1)
    {   // цилиндр
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_CYLINDER);
        add_cube_dlg->show();
    }
    else if (m == m_2)
    { // линия
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_LINES);
        add_cube_dlg->show();
    }
    else if (m == m_3)
    {   // Конус
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_TETRA);
        add_cube_dlg->show();
    }
    else if (m == m_4)
    {   // Сфера
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_SPHERE);
        add_cube_dlg->show();
    }
    else if (m == m_5)
    {   // Точка учета
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_GADGET);
        add_cube_dlg->show();
    }
    else if (m == m_6)
    { // Полимер
        if (add_cube_dlg == NULL)
            add_cube_dlg = new AddCubeDialog(add_cube_done_cb);
        add_cube_dlg->Init(NULL, geom_type_enum::GO_POLY);
        add_cube_dlg->show();
    }
}

// Ребра-Сплошной
void wire_cb(Fl_Widget* bt, void* ud)
{
    if (!IsReady()) return;
    if (!IsSelected()) return;

    bool bw = geobCurrent->bWire;
    geobCurrent->bWire = !bw;
}

// камера XYZ завершена
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
// открыть камера XYZ диалог
void camera_xyz_cb(Fl_Widget*, void*)
{
    if (camera_xyz_dlg == NULL)
        camera_xyz_dlg = new CameraXyzDialog(camera_xyz_done_cb);
    camera_xyz_dlg->Init(geob_win);
    camera_xyz_dlg->show();
}

// камера сферичические завершена
void camera_sph_done_cb(Fl_Widget* bt, void* ud)
{
    int m = *(int*)ud;
    if (m == 1)
    {   //Ok 
        double distance, azimut, elevation, xLook, yLook, zLook;
        bool bSuc = camera_sph_dlg->GetPos(distance, azimut, elevation, xLook, yLook, zLook);
        if (!bSuc)
        {
            fl_message(u8"Ошибка в данных");
            return;
        }
        geob_win->SetXLook(xLook);
        geob_win->SetYLook(yLook);
        geob_win->SetZLook(zLook);

        geob_win->SetPolar(distance, azimut, elevation);
    }

    camera_sph_dlg->hide();
}
// открыть камера сферичические диалог
void camera_sph_cb(Fl_Widget*, void*)
{
    if (camera_sph_dlg == NULL)
        camera_sph_dlg = new CameraSphDialog(camera_sph_done_cb);
    camera_sph_dlg->Init(geob_win);
    camera_sph_dlg->show();
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
    char buf[33] = { 0 };
    sprintf_s(buf, "d=%.3f a=%.3f e=%.3f ", distance, azimut, elevation);
    text2->value(buf);
}

// Координаты мыши. Переменные инициализируются значениями -1
// когда клавиши не нажаты
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

// Функция обработки нажатия клавиш перемещения
// Более естественно, когда по стрелке вправо сцена вращается вправо (против часовой), поэтому азимуи уменьшаем (камера смещаетсч влево)
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

// Функция обработки нажатия обычных клавиш
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

int ct_grp_w = 240; // ширина области виджетов
int gl_w = 400; // ширина GL окна
int gl_h = 400; // высота GL окна
//int rt_grp_w = 5; // область для будущего

// создать главное окно и виджеты
void MakeForm(const char *name) 
{
  int form_w = gl_w + ct_grp_w + MARGIN3; // ширина главного окна
  int form_h = gl_h + MENUBAR_H + MARGIN2;           // высота главного окна
  // XYWH'меню
  int me_bar_x = 0, 
      me_bar_y = 0, 
      me_bar_w = form_w, 
      me_bar_h = MENUBAR_H; 
  // XYWH'центральная группа
  int ct_grp_x = MARGIN,
      ct_grp_y = MENUBAR_H + MARGIN/2,
      ct_grp_h = form_h - MENUBAR_H - MARGIN; 
  /*// XYWH'правая группа
  int rt_grp_x = ct_grp_x + ct_grp_w + gl_w + MARGIN2,
      rt_grp_y = ct_grp_y, 
      rt_grp_h = ct_grp_h;*/
  //char buf[33] = { 0 };

  // главное окно
  form = new Fl_Window(form_w, form_h, name);
  form->begin();

  // меню
  Fl_Menu_Bar *menubar = new Fl_Menu_Bar(me_bar_x, me_bar_y, me_bar_w, me_bar_h);
  menubar->add(u8"Проект/Создать", FL_COMMAND + 'c', file_create_cb);
  menubar->add(u8"Проект/Открыть", FL_COMMAND + 'o', file_open_cb);
  menubar->add(u8"Проект/Сохранить", FL_COMMAND + 's', file_save_cb);
  menubar->add(u8"Проект/Сохранить как", FL_COMMAND + 'a', file_save_as_cb);
  menubar->add(u8"Проект/Генерировать ini", FL_COMMAND + 'g', file_gener_cb);
  menubar->add(u8"Проект/Выход", FL_COMMAND + 'q', exit_cb);

  menubar->add(u8"Камера/Камера XYZ", FL_COMMAND + 'x', camera_xyz_cb);
  menubar->add(u8"Камера/Камера сферично", FL_COMMAND + 'r', camera_sph_cb);

  menubar->add(u8"Геометрия/Добавить/Куб", FL_COMMAND + 'u', add_cube_cb, (void*)&m_0);
  menubar->add(u8"Геометрия/Добавить/Цилиндр", FL_COMMAND + 'y', add_cube_cb, (void*)&m_1);
  menubar->add(u8"Геометрия/Добавить/Конус", FL_COMMAND + 'n', add_cube_cb, (void*)&m_3);
  menubar->add(u8"Геометрия/Добавить/Сфера", FL_COMMAND + 'p', add_cube_cb, (void*)&m_4);
  menubar->add(u8"Геометрия/Добавить/Линии", FL_COMMAND + 'l', add_cube_cb, (void*)&m_2);
  menubar->add(u8"Геометрия/Добавить/Точку учета", FL_COMMAND + 't', add_cube_cb, (void*)&m_5);
  menubar->add(u8"Геометрия/Добавить/Полимер", 0, add_cube_cb, (void*)&m_6);
  menubar->add(u8"Геометрия/Изменить", FL_COMMAND + 'e', edit_cb);
  menubar->add(u8"Геометрия/Удалить", FL_COMMAND + 'd', delete_cb);
  menubar->add(u8"Геометрия/Свойства", FL_COMMAND + 'h', features_cb);
  menubar->add(u8"Геометрия/Ребра-Сплошной", FL_COMMAND + 'w', wire_cb);

  menubar->add(u8"Помощь", FL_COMMAND + 'h', help_cb);

  // центральная группа
  Fl_Group *ct_grp = new Fl_Group(ct_grp_x, ct_grp_y, ct_grp_w, ct_grp_h);
  ct_grp->begin();

  //list box объектов
  scroll = new Fl_Scroll(ct_grp_x + MARGIN, MENUBAR_H + MARGIN2, 200, 300, u8"объекты");
  scroll->begin();
  pack = new Fl_Pack(ct_grp_x + MARGIN, MENUBAR_H + MARGIN2, 200, 300);
  pack->box(FL_DOWN_FRAME);
  scroll->end();

  text2 = new Fl_Multiline_Output(ct_grp_x + MARGIN, ct_grp_h - MARGIN3, 200, 40, u8"Инфо окно");
  text2->value("");
  text2->align(FL_ALIGN_BOTTOM);
  //text2->tooltip("Инфо окно");

  ct_grp->end();
  ct_grp->box(FL_BORDER_BOX);
  
  // right resizer
  //Fl_Box *rt_resizer = new Fl_Box(rt_grp_x - 5, rt_grp_y, 5, rt_grp_h);
  //rt_resizer->box(FL_NO_BOX);

  form->end();
  //form->resizable(rt_resizer);
  form->size_range(form->w(), form->h()); // minimum window size
}

// создаем главное Fl_Glut_Window* glut_win_main
void MakeGlWindow(bool bDouble)
{
    form->begin();

    unsigned int mode = GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE;
    if (bDouble) mode |= GLUT_DOUBLE;
    glutInitDisplayMode(mode);
    glutInitWindowPosition(ct_grp_w + MARGIN2, MENUBAR_H + MARGIN);
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
    char ltxt[330];
    // добавить геометрию
    for (int i = 0; i < nbuttons; i++) {       
        GeOb * ob = geob_win->GetObj(i);
        if (ob == NULL) continue;
        ob->GetName(ltxt);
        Fl_Button* b = new Fl_Button(xx, xx, 200, 25);
        b->copy_label(ltxt);
        b->callback(listbox_cb);
        b->user_data((void*)ob);
        b->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
        xx += 10;
    }

    // добавить диалоги
    int nb2 = beatIni == NULL ? 0 : (int)beatIni->lstDlg.size();
    for (int i = 0; i < nb2; i++) {
        TDialog * ob = beatIni->lstDlg[i];
        if (ob == NULL) continue;
        Fl_Button* b = new Fl_Button(xx, xx, 200, 25);
        b->copy_label(ob->dname.c_str());
        b->callback(listbox_dial_cb);
        b->user_data((void*)ob);
        b->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
        xx += 10;
    }

    pack->end();
    pack->redraw();
    scroll->redraw();
    geobCurrent = NULL;
    lbBtnCurrent = NULL;
}

int main(int argc, char **argv) 
{
#ifdef _WINDOWS
  SetConsoleOutputCP(CP_UTF8);
#endif
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
  MakeForm(u8"Beat - препроцессор");

  // создаем главное Fl_Glut_Window* glut_win_main
  MakeGlWindow(bDouble);

  form->show();

  /*// создать Геометрические объекты
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
  geob_win->Add(ln1);*/

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

  // удалить загрузчик ini
  if (beatIni != NULL)
  {
    delete beatIni;
    beatIni = NULL;
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
  if (camera_sph_dlg != NULL)
  {
      delete camera_sph_dlg;
      camera_sph_dlg = NULL;
  }
  if (phys_dlg != NULL)
  {
      delete phys_dlg;
      phys_dlg = NULL;
  }
  if (bt_dlg != NULL)
  {
      delete bt_dlg;
      bt_dlg = NULL;
  }
  return 0; 
}
//==



