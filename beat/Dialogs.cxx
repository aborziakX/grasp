#include "Dialogs.h"

namespace Grasp {

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

//==Добавить куб
AddCubeDialog::AddCubeDialog(Fl_Callback* cb) : 
    Fl_Window(200, 100, 300, 300, u8"Добавить куб")
{
    button_Ok.callback(cb, &m1);
    button_Cancel.callback(cb, &m2);

    //box_result.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
}
bool AddCubeDialog::GetPos(double& x, double& y, double& z, double& xSc, double& ySc, double& zSc)
{
    bool rc = true;
    try {
        x = atof(inX.value());
        y = atof(inY.value());
        z = atof(inZ.value());
        xSc = atof(inScX.value());
        ySc = atof(inScY.value());
        zSc = atof(inScZ.value());
    }
    catch (...) {
        rc = false;
    }
    return rc;
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

bool PhysPropDialog::GetPos(double * di)
{
    bool rc = true;
    try {
        Fl_Input* field;
        int i;
        for (i = 0; i < 10 && i < mol->lstFeature.size(); i++)
        {
            field = fieldByIndex(i);

            if (field->visible() == 0) continue;
            di[0] = atof(field->value());
        }

        for (i = 0; i < 10 && i < mol->lstFeature.size(); i++)
        {
            field = fieldByIndex(i);

            if (field->visible() == 0) continue;
            TParam* par = mol->lstFeature[i];
            par->pcurr = field->value();
        }

    }
    catch (...) {
        rc = false;
    }
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

void PhysPropDialog::Init(TMolecule* _mol)
{
    mol = _mol;
    char buf[330];
    Fl_Input* field;
    int i;
    for (i = 0; i < 10; i++)
    {
        field = fieldByIndex(i);

        if (i < mol->lstFeature.size()) {
            TParam* par = mol->lstFeature[i];
            sprintf_s(buf, "%s", par->pcurr.c_str());
            field->value(buf);
            field->label( par->pcomment.c_str() );
        }
        else field->clear_visible();
    }


}
//==

}