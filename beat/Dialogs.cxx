#include "Dialogs.h"
#include <FL/fl_show_colormap.H>
#include <FL/Fl_Color_Chooser.H>

namespace Grasp {
    bool IsValid(const char * val)
    {
        double d = atof(val);
        if (d == 0 && !(strcmp(val,"0") == 0 || strcmp(val, "0.0") == 0))
            return false;
        return true;
    }

Pass_Window::Pass_Window(Fl_Callback* cb) : 
    Fl_Window(200, 100, 300, 300, "Password example")
{
    /*button_show_message.callback([](Fl_Widget* sender, void* data) {
        auto window = reinterpret_cast<Pass_Window*>(data);
        auto result = fl_password("User password:", window->box_result.label());
        if (result) window->box_result.copy_label(result);
        }, this);*/
    button_show_message.callback(cb);

    box_result.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
}
//==

//==Добавить куб etc.
// выбор цвета
#define fullcolor_cell (FL_FREE_COLOR)

void cb_color(Fl_Widget* bt, void* v) {
    AddCubeDialog* dlg = (AddCubeDialog*)v;
    dlg->c = fl_show_colormap(dlg->c);
    Fl_Box* b = dlg->Getbox();
    b->color(dlg->c);
    b->redraw();
}
void cb_color2(Fl_Widget* bt, void* v) {
    AddCubeDialog* dlg = (AddCubeDialog*)v;
    uchar r, g, b;
    Fl::get_color(dlg->c, r, g, b);
    if (!fl_color_chooser(u8"Новый цвет:", r, g, b, 2)) return;
    dlg->c = fullcolor_cell;
    dlg->red = r; 
    dlg->green = g;
    dlg->blue = b;
    Fl::set_color(fullcolor_cell, r, g, b);
    Fl_Box* bx = dlg->Getbox();
    bx->color(fullcolor_cell);
    bx->redraw();
}

AddCubeDialog::AddCubeDialog(Fl_Callback* cb) : 
    Fl_Window(200, 100, 300, 300, u8"Добавить куб")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);
    button_Clr.callback(cb_color2, this);
    box.box(FL_THIN_DOWN_BOX);
}

bool AddCubeDialog::GetPos(double& x, double& y, double& z, double& xSc, double& ySc, double& zSc, int &nSide,
    unsigned char& _red, unsigned char& _green, unsigned char& _blue)
{
    bool rc = true;
    try {
        x = atof(inX.value());
        y = atof(inY.value());
        z = atof(inZ.value());
        xSc = atof(inScX.value());
        ySc = atof(inScY.value());
        zSc = atof(inScZ.value());
        nSide = atoi(inSide.value());

        _red = red;
        _green = green;
        _blue = blue;
    }
    catch (...) {
        rc = false;
    }
    return rc;
}
// инициализация
void AddCubeDialog::Init(GeOb* _geob, geom_type_enum _geom_type)
{
    geob = _geob;
    /** 0 - sphere, 1 - box, 2 -cylinder, 3 -tetra, 4 - lines, 5 - gadget, 1000 - default */
    geom_type = _geom_type;

    char buf[33];
    double x, y, z;
    if (geob == NULL)
    { // новый объект
        inX.value("0.0");
        inY.value("0.0");
        inZ.value("0.0");
        inScX.value("1.0");
        inScY.value("1.0");
        inScZ.value("1.0");
        inSide.value(geom_type == geom_type_enum::GO_GADGET ? "3" : "8");

        if (geom_type == geom_type_enum::GO_BOX) this->copy_label(u8"Добавить куб");
        else if (geom_type == geom_type_enum::GO_CYLINDER) this->copy_label(u8"Добавить цилиндр");
        else if (geom_type == geom_type_enum::GO_LINES) this->copy_label(u8"Добавить линию");
        else if (geom_type == geom_type_enum::GO_TETRA) this->copy_label(u8"Добавить конус");
        else if (geom_type == geom_type_enum::GO_SPHERE) this->copy_label(u8"Добавить сферу");
        else if (geom_type == geom_type_enum::GO_GADGET) this->copy_label(u8"Добавить точку учета");
    }
    else
    {
        Facet3* f = NULL;
        geob->GetShift(x, y, z);
        if (geom_type == geom_type_enum::GO_LINES)
        {
            f = geob->GetFacet(0);
            Vec3 * v1 = f->GetPoint(0);
            x = v1->GetX();
            y = v1->GetY();
            z = v1->GetZ();
        }

        sprintf_s(buf, "%.3f", x);
        inX.value(buf);

        sprintf_s(buf, "%.3f", y);
        inY.value(buf);

        sprintf_s(buf, "%.3f", z);
        inZ.value(buf);

        geob->GetScale(x, y, z);
        if (geom_type == geom_type_enum::GO_LINES)
        {
            f = geob->GetFacet(0);
            Vec3* v1 = f->GetPoint(1);
            x = v1->GetX();
            y = v1->GetY();
            z = v1->GetZ();
        }

        sprintf_s(buf, "%.3f", x);
        inScX.value(buf);

        sprintf_s(buf, "%.3f", y);
        inScY.value(buf);

        sprintf_s(buf, "%.3f", z);
        inScZ.value(buf);

        sprintf_s(buf, "%d", geob->GetNside());
        inSide.value(buf);

        if (geom_type == geom_type_enum::GO_BOX) this->copy_label(u8"Изменить куб");
        else if (geom_type == geom_type_enum::GO_CYLINDER) this->copy_label(u8"Изменить цилиндр");
        else if (geom_type == geom_type_enum::GO_LINES) this->copy_label(u8"Изменить линию");
        else if (geom_type == geom_type_enum::GO_TETRA) this->copy_label(u8"Изменить конус");
        else if (geom_type == geom_type_enum::GO_SPHERE) this->copy_label(u8"Изменить сферу");
        else if (geom_type == geom_type_enum::GO_GADGET) this->copy_label(u8"Изменить точку учета");
    }

    if (geom_type == geom_type_enum::GO_BOX || geom_type == geom_type_enum::GO_LINES
        || geom_type == geom_type_enum::GO_GADGET)
    {
        inSide.clear_visible();
    }
    else inSide.set_visible();

    if (geom_type == geom_type_enum::GO_GADGET)
    {
        inScX.clear_visible();
        inScY.clear_visible();
        inScZ.clear_visible();
    }
    else
    {
        inScX.set_visible();
        inScY.set_visible();
        inScZ.set_visible();
    }

    
    if (geob != NULL)
    {
        geob->GetColor(red, green, blue);
    }
    /*
    Fl_Color => 0xrrggbbii
            | | | |
            | | | +--- \ref drawing_colors "index" between 0 and 255
            | | +----- blue color component (8 bit)
            | +------- green component (8 bit)
            +--------- red component (8 bit) */
    Fl::set_color(fullcolor_cell, red, green, blue);
    c = fullcolor_cell;
    //c = (_red << 24) | (_green << 16) | (_blue << 8);
    box.color(c);

}
//==

//==камера XYZ
CameraXyzDialog::CameraXyzDialog(Fl_Callback* cb) :
    Fl_Window(200, 100, 400, 300, u8"камера XYZ")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);
}

bool CameraXyzDialog::GetPos(double& x, double& y, double& z, 
    double& xLook, double& yLook, double& zLook,
    double& xTop, double& yTop, double& zTop)
{
    bool rc = true;
    try {
        x = atof(inX.value());
        y = atof(inY.value());
        z = atof(inZ.value());
        xLook = atof(inLookX.value());
        yLook = atof(inLookY.value());
        zLook = atof(inLookZ.value());
        xTop = atof(inTopX.value());
        yTop = atof(inTopY.value());
        zTop = atof(inTopZ.value());
    }
    catch (...) {
        rc = false;
    }
    return rc;
}

void CameraXyzDialog::Init(GeObWindow* geob_win)
{
    char buf[33];
    sprintf_s(buf, "%.3f", geob_win->GetXCam());
    inX.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYCam());
    inY.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZCam());
    inZ.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetXTop());
    inTopX.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYTop());
    inTopY.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZTop());
    inTopZ.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetXLook());
    inLookX.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYLook());
    inLookY.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZLook());
    inLookZ.value(buf);
}
//==

//==камера сферические координаты
CameraSphDialog::CameraSphDialog(Fl_Callback* cb) :
    Fl_Window(200, 100, 300, 300, u8"камера сферические")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);
}

bool CameraSphDialog::GetPos(double& di, double& az, double& el,
    double& xLook, double& yLook, double& zLook)
{
    bool rc = true;
    try {
        di = atof(inDi.value());
        az = atof(inAz.value());
        el = atof(inEl.value());
        xLook = atof(inLookX.value());
        yLook = atof(inLookY.value());
        zLook = atof(inLookZ.value());
    }
    catch (...) {
        rc = false;
    }
    return rc;
}

void CameraSphDialog::Init(GeObWindow* geob_win)
{
    char buf[33];
    double di, az, el;
    geob_win->GetPolar(di, az, el);
    sprintf_s(buf, "%.3f", di);
    inDi.value(buf);

    sprintf_s(buf, "%.3f", az);
    inAz.value(buf);

    sprintf_s(buf, "%.3f", el);
    inEl.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetXLook());
    inLookX.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetYLook());
    inLookY.value(buf);

    sprintf_s(buf, "%.3f", geob_win->GetZLook());
    inLookZ.value(buf);
}
//==

//==физические свойства
PhysPropDialog::PhysPropDialog(Fl_Callback* cb) :
    Fl_Window(200, 100, 300, 350, u8"физические свойства")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);
}

// присвоить значения
bool PhysPropDialog::GetPos()
{
    bool rc = true;
    Fl_Input* field;
    Fl_Choice* choice;
    std::vector<std::string*> vec;
    try {
        int i;
        // проверить корректность ввода
        for (i = 0; i < 10 && i < mol->lstFeature.size(); i++)
        {
            field = fieldByIndex(i);
            if (field->visible() == 0) continue;
            if (!IsValid(field->value()))
            {
                rc = false;
                break;
            }
        }

        // изменить значения
        for (i = 0; rc && i < 10 && i < mol->lstFeature.size(); i++)
        {
            field = fieldByIndex(i);
            choice = choiceByIndex(i);

            if (field->visible() != 0)
            {
                TParam* par = mol->lstFeature[i];
                par->pcurr = field->value();
            }
            if (choice->visible() != 0)
            {
                TParam* par = mol->lstFeature[i];
                string txt(choice->text());
                for (int k = 0; k < par->lstCombo.size(); k++)
                {
                    BeatIni::split2vector(*par->lstCombo[k], ':', vec);
                    if (vec.size() == 2)
                    {
                        if (*vec[1] == txt)
                        {
                            par->pcurr = *vec[0];
                            break;
                        }
                    }
                }               
            }
        }
    }
    catch (...) {
        rc = false;
    }
    BeatIni::split2vector(NULL, ':', vec); // очистить память
    return rc;
}

Fl_Input* PhysPropDialog::fieldByIndex(int i)
{
    Fl_Input* field = NULL;
    if (i == 0) field = &in1;
    else if (i == 1) field = &in2;
    else if (i == 2) field = &in3;
    else if (i == 3) field = &in4;
    else if (i == 4) field = &in5;
    else if (i == 5) field = &in6;
    else if (i == 6) field = &in7;
    else if (i == 7) field = &in8;
    else if (i == 8) field = &in9;
    else if (i == 9) field = &in10;
    return field;
}

Fl_Choice* PhysPropDialog::choiceByIndex(int i)
{
    Fl_Choice* field = NULL;
    if (i == 0) field = &ch1;
    else if (i == 1) field = &ch2;
    else if (i == 2) field = &ch3;
    else if (i == 3) field = &ch4;
    else if (i == 4) field = &ch5;
    else if (i == 5) field = &ch6;
    else if (i == 6) field = &ch7;
    else if (i == 7) field = &ch8;
    else if (i == 8) field = &ch9;
    else if (i == 9) field = &ch10;
    return field;
}

// инициализация
void PhysPropDialog::Init(TMolecule* _mol)
{
    mol = _mol;
    char buf[330];
    Fl_Input* field;
    Fl_Choice* choice;
    std::vector<std::string*> vec;
    int i;
    for (i = 0; i < 10; i++)
    {
        field = fieldByIndex(i);
        choice = choiceByIndex(i);

        if (i < mol->lstFeature.size()) 
        {
            TParam* par = mol->lstFeature[i];
            if (par->ptype != 3)
            {   // edit box
                sprintf_s(buf, "%s", par->pcurr.c_str());
                field->value(buf);
                field->copy_label(par->pcomment.c_str());

                field->set_visible();
                choice->clear_visible();
            }
            else
            {   // combo box
                choice->copy_label(par->pcomment.c_str());
                int m = 0;
                for (int k = 0; k < par->lstCombo.size(); k++)
                {
                    BeatIni::split2vector(*par->lstCombo[k], ':', vec);
                    if (vec.size() == 2)
                    {
                        choice->add(vec[1]->c_str());
                        if (*vec[0] == par->pcurr)
                            choice->value(m);
                        m++;
                    }
                }

                field->clear_visible();
                choice->set_visible();
            }
        }
        else
        {
            field->clear_visible();
            choice->clear_visible();
        }
    }
    BeatIni::split2vector(NULL, ':', vec); // очистить память
}
//==

//==выбор значений параметров
BeatDialog::BeatDialog(Fl_Callback* cb) :
    Fl_Window(100, 100, 400, 350, u8"выбор значений")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);
}

// присвоить значения
bool BeatDialog::GetPos()
{
    if (mol == NULL) return true;
    bool rc = true;
    Fl_Input* field;
    Fl_Choice* choice;
    std::vector<std::string*> vec;
    try {
        int i;
        // проверить корректность ввода
        for (i = 0; i < 10 && i < mol->lstParams.size(); i++)
        {
            field = fieldByIndex(i);
            if (field->visible() == 0) continue;
            if (!IsValid(field->value()))
            {
                rc = false;
                break;
            }
        }

        // изменить значения
        for (i = 0; rc && i < 10 && i < mol->lstParams.size(); i++)
        {
            field = fieldByIndex(i);
            choice = choiceByIndex(i);

            if (field->visible() != 0)
            {
                TParam* par = mol->lstParams[i];
                par->pcurr = field->value();
            }
            if (choice->visible() != 0)
            {
                TParam* par = mol->lstParams[i];
                string txt(choice->text());
                for (int k = 0; k < par->lstCombo.size(); k++)
                {
                    BeatIni::split2vector(*par->lstCombo[k], ':', vec);
                    if (vec.size() == 2)
                    {
                        if (*vec[1] == txt)
                        {
                            par->pcurr = *vec[0];
                            break;
                        }
                    }
                }
            }
        }
    }
    catch (...) {
        rc = false;
    }
    BeatIni::split2vector(NULL, ':', vec); // очистить память
    return rc;
}

Fl_Input* BeatDialog::fieldByIndex(int i)
{
    Fl_Input* field = NULL;
    if (i == 0) field = &in1;
    else if (i == 1) field = &in2;
    else if (i == 2) field = &in3;
    else if (i == 3) field = &in4;
    else if (i == 4) field = &in5;
    else if (i == 5) field = &in6;
    else if (i == 6) field = &in7;
    else if (i == 7) field = &in8;
    else if (i == 8) field = &in9;
    else if (i == 9) field = &in10;
    return field;
}

Fl_Choice* BeatDialog::choiceByIndex(int i)
{
    Fl_Choice* field = NULL;
    if (i == 0) field = &ch1;
    else if (i == 1) field = &ch2;
    else if (i == 2) field = &ch3;
    else if (i == 3) field = &ch4;
    else if (i == 4) field = &ch5;
    else if (i == 5) field = &ch6;
    else if (i == 6) field = &ch7;
    else if (i == 7) field = &ch8;
    else if (i == 8) field = &ch9;
    else if (i == 9) field = &ch10;
    return field;
}

// инициализация
void BeatDialog::Init(TDialog* _mol)
{
    mol = _mol;
    if (mol == NULL) return;
    this->copy_label(mol->dname.c_str());

    char buf[330];
    Fl_Input* field;
    Fl_Choice* choice;
    std::vector<std::string*> vec;
    int i;
    for (i = 0; i < 10; i++)
    {
        field = fieldByIndex(i);
        choice = choiceByIndex(i);

        if (i < mol->lstParams.size())
        {
            TParam* par = mol->lstParams[i];
            if (par->ptype != 3)
            {
                sprintf_s(buf, "%s", par->pcurr.c_str());
                field->value(buf);
                field->copy_label(par->pcomment.c_str());

                field->set_visible();
                choice->clear_visible();
            }
            else
            {
                choice->copy_label(par->pcomment.c_str());
                int m = 0;
                for (int k = 0; k < par->lstCombo.size(); k++)
                {
                    BeatIni::split2vector(*par->lstCombo[k], ':', vec);
                    if (vec.size() == 2)
                    {
                        choice->add(vec[1]->c_str());
                        if (*vec[0] == par->pcurr)
                            choice->value(m);
                        m++;
                    }
                }

                choice->set_visible();
                field->clear_visible();
            }
        }
        else
        {
            field->clear_visible();
            choice->clear_visible();
        }

    }
    BeatIni::split2vector(NULL, ':', vec); // очистить память
}
//==


}