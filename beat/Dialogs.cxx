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

}