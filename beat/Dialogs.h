#ifndef BEAT_DIALOGS_H
#define BEAT_DIALOGS_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Input.H>

#include <stdlib.h>
#include "GeObWindow.h"

namespace Grasp {
/**
@class   Pass_Window
@brief   Pass_Window со списком Геометрических объектов
*/
    class Pass_Window : public Fl_Window
    {
    public:
        Pass_Window(Fl_Callback* cb);

    private:
        Fl_Button button_show_message{ 10, 10, 100, 25, "Password..." };
        Fl_Box box_result{ 10, 50, 200, 25, "sysad47!74dasys" };
    };

/**
@class   AddCubeDialog
@brief   Добавить куб
*/
class AddCubeDialog : public Fl_Window
{
    public:
        AddCubeDialog(Fl_Callback * cb);
        bool GetPos(double& x, double& y, double& z, double& xSc, double& ySc, double& zSc);

    private:
        Fl_Button button_Ok{ 10, 220, 100, 25, u8"Сохранить" };
        Fl_Button button_Cancel{ 150, 220, 100, 25, u8"Отмена" };
        int m1 = 1; //Ok
        int m2 = 2; //Cancel
        Fl_Input inX{ 30, 10, 70, 20, "x:" };
        Fl_Input inY{ 30, 40, 70, 20, "y:" };
        Fl_Input inZ{ 30, 70, 70, 20, "z:" };
        Fl_Input inScX{ 150, 10, 70, 20, u8"масш.x:" };
        Fl_Input inScY{ 150, 40, 70, 20, u8"масш.y:" };
        Fl_Input inScZ{ 150, 70, 70, 20, u8"масш.z:" };
};

/**
@class   CameraXyzDialog
@brief   камера XYZ
*/
class CameraXyzDialog : public Fl_Window
{
public:
    CameraXyzDialog(Fl_Callback* cb);
    void Init(GeObWindow* go);
    bool GetPos(double& x, double& y, double& z,
        double& xLook, double& yLook, double& zLook,
        double& xTop, double& yTop, double& zTop);

private:
    Fl_Button button_Ok{ 10, 220, 100, 25, u8"Сохранить" };
    Fl_Button button_Cancel{ 150, 220, 100, 25, u8"Отмена" };
    int m1 = 1; //Ok
    int m2 = 2; //Cancel
    Fl_Input inX{ 50, 10, 70, 20, "xCam:" };
    Fl_Input inY{ 50, 40, 70, 20, "yCam:" };
    Fl_Input inZ{ 50, 70, 70, 20, "zCam:" };

    Fl_Input inLookX{ 170, 10, 70, 20, u8"xLook:" };
    Fl_Input inLookY{ 170, 40, 70, 20, u8"yLook:" };
    Fl_Input inLookZ{ 170, 70, 70, 20, u8"zLook:" };

    Fl_Input inTopX{ 290, 10, 70, 20, u8"xTop:" };
    Fl_Input inTopY{ 290, 40, 70, 20, u8"yTop:" };
    Fl_Input inTopZ{ 290, 70, 70, 20, u8"zTop:" };
};

} // namespace Grasp
#endif