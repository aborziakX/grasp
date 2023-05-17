#ifndef BEAT_DIALOGS_H
#define BEAT_DIALOGS_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>

#include <stdlib.h>
#include "GeObWindow.h"
#include "beat_ini.h"
#ifndef _WINDOWS
#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif

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
        void Init(GeOb* _mol, geom_type_enum _geom_type); /** инициализация */
        bool GetPos(double& x, double& y, double& z, double& xSc, double& ySc, double& zSc, int& nSide);
        geom_type_enum geom_type = geom_type_enum::GO_BOX;
        GeOb* geob = NULL;

    private:
        Fl_Button button_Ok{ 10, 220, 100, 25, u8"Применить" };
        Fl_Button button_Cancel{ 150, 220, 100, 25, u8"Отмена" };
        int m1 = 1; //Ok
        int m2 = 2; //Cancel
        Fl_Input inX{ 30, 10, 70, 20, "x:" };
        Fl_Input inY{ 30, 40, 70, 20, "y:" };
        Fl_Input inZ{ 30, 70, 70, 20, "z:" };
        Fl_Input inScX{ 150, 10, 70, 20, u8"масш.x:" };
        Fl_Input inScY{ 150, 40, 70, 20, u8"масш.y:" };
        Fl_Input inScZ{ 150, 70, 70, 20, u8"масш.z:" };
        Fl_Input inSide{ 170, 100, 70, 20, u8"число боковых граней:" };
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
    Fl_Button button_Ok{ 10, 220, 100, 25, u8"Применить" };
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

/**
@class   CameraSphDialog
@brief   камера сферические координаты
*/
class CameraSphDialog : public Fl_Window
{
public:
    CameraSphDialog(Fl_Callback* cb);
    void Init(GeObWindow* go);
    bool GetPos(double& di, double& az, double& el,
        double& xLook, double& yLook, double& zLook);

private:
    Fl_Button button_Ok{ 10, 220, 100, 25, u8"Применить" };
    Fl_Button button_Cancel{ 150, 220, 100, 25, u8"Отмена" };
    int m1 = 1; //Ok
    int m2 = 2; //Cancel
    Fl_Input inDi{ 100, 10, 70, 20, u8"дистанция:" };
    Fl_Input inAz{ 100, 40, 70, 20, u8"азимут:" };
    Fl_Input inEl{ 100, 70, 70, 20, u8"возвышение:" };

    Fl_Input inLookX{ 100, 100, 70, 20, u8"xLook:" };
    Fl_Input inLookY{ 100, 130, 70, 20, u8"yLook:" };
    Fl_Input inLookZ{ 100, 160, 70, 20, u8"zLook:" };
};

/**
@class   PhysPropDialog
@brief   физические свойства
*/
class PhysPropDialog : public Fl_Window
{
public:
    PhysPropDialog(Fl_Callback* cb);
    void Init(TMolecule* _mol); /// инициализация
    bool GetPos(); /// присвоить значения

private:
    Fl_Button button_Ok{ 10, 320, 100, 25, u8"Применить" };
    Fl_Button button_Cancel{ 150, 320, 100, 25, u8"Отмена" };
    int m1 = 1; //Ok
    int m2 = 2; //Cancel
    Fl_Input in1{ 170, 10, 70, 20, u8"1:" };
    Fl_Input in2{ 170, 40, 70, 20, u8"2:" };
    Fl_Input in3{ 170, 70, 70, 20, u8"3:" };
    Fl_Input in4{ 170, 100, 70, 20, u8"4:" };
    Fl_Input in5{ 170, 130, 70, 20, u8"5:" };
    Fl_Input in6{ 170, 160, 70, 20, u8"6:" };
    Fl_Input in7{ 170, 190, 70, 20, u8"7:" };
    Fl_Input in8{ 170, 220, 70, 20, u8"8:" };
    Fl_Input in9{ 170, 250, 70, 20, u8"9:" };
    Fl_Input in10{ 170, 280, 70, 20, u8"10:" };

    Fl_Choice ch1{ 170, 10, 70, 20, u8"1:" };
    Fl_Choice ch2{ 170, 40, 70, 20, u8"2:" };
    Fl_Choice ch3{ 170, 70, 70, 20, u8"3:" };
    Fl_Choice ch4{ 170, 100, 70, 20, u8"4:" };
    Fl_Choice ch5{ 170, 130, 70, 20, u8"5:" };
    Fl_Choice ch6{ 170, 160, 70, 20, u8"6:" };
    Fl_Choice ch7{ 170, 190, 70, 20, u8"7:" };
    Fl_Choice ch8{ 170, 220, 70, 20, u8"8:" };
    Fl_Choice ch9{ 170, 250, 70, 20, u8"9:" };
    Fl_Choice ch10{ 170, 280, 70, 20, u8"10:" };

    TMolecule* mol = NULL;
    Fl_Input* fieldByIndex(int ind);
    Fl_Choice* choiceByIndex(int ind);
};

/**
@class   BeatDialog
@brief   выбор значений параметров
*/
class BeatDialog : public Fl_Window
{
public:
    BeatDialog(Fl_Callback* cb);
    void Init(TDialog* _mol); /// инициализация
    bool GetPos(); /// присвоить значения

private:
    Fl_Button button_Ok{ 10, 320, 100, 25, u8"Применить" };
    Fl_Button button_Cancel{ 150, 320, 100, 25, u8"Отмена" };
    int m1 = 1; //Ok
    int m2 = 2; //Cancel
    Fl_Input in1{ 270, 10, 70, 20, u8"1:" };
    Fl_Input in2{ 270, 40, 70, 20, u8"2:" };
    Fl_Input in3{ 270, 70, 70, 20, u8"3:" };
    Fl_Input in4{ 270, 100, 70, 20, u8"4:" };
    Fl_Input in5{ 270, 130, 70, 20, u8"5:" };
    Fl_Input in6{ 270, 160, 70, 20, u8"6:" };
    Fl_Input in7{ 270, 190, 70, 20, u8"7:" };
    Fl_Input in8{ 270, 220, 70, 20, u8"8:" };
    Fl_Input in9{ 270, 250, 70, 20, u8"9:" };
    Fl_Input in10{ 270, 280, 70, 20, u8"10:" };

    Fl_Choice ch1{ 270, 10, 70, 20, u8"1:" };
    Fl_Choice ch2{ 270, 40, 70, 20, u8"2:" };
    Fl_Choice ch3{ 270, 70, 70, 20, u8"3:" };
    Fl_Choice ch4{ 270, 100, 70, 20, u8"4:" };
    Fl_Choice ch5{ 270, 130, 70, 20, u8"5:" };
    Fl_Choice ch6{ 270, 160, 70, 20, u8"6:" };
    Fl_Choice ch7{ 270, 190, 70, 20, u8"7:" };
    Fl_Choice ch8{ 270, 220, 70, 20, u8"8:" };
    Fl_Choice ch9{ 270, 250, 70, 20, u8"9:" };
    Fl_Choice ch10{ 270, 280, 70, 20, u8"10:" };

    TDialog* mol = NULL;
    Fl_Input* fieldByIndex(int ind);
    Fl_Choice* choiceByIndex(int ind);
};

} // namespace Grasp
#endif