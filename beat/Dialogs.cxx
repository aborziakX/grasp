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

}