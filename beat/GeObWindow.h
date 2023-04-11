#ifndef BEAT_GEOBWINDOW_H
#define BEAT_GEOBWINDOW_H

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.h>
#include <string>
#include <vector>
#include "Vec3.h"
#include "GeOb.h"

using namespace std;

namespace Grasp {
/*
оси 
     ^ Y
     |
     |
  O  -----> X
   /
  /
 / 
v Z
*/

#define v3f(x) glVertex3fv(x)

/**
@class   GeObWindow
@brief   со списком Геометрических объектов (Geometrical object - GeOb)
*/
class GeObWindow 
{
//  int handle(int e) FL_OVERRIDE;

public:
  /**
  Конструктор GeObWindow 
  */
  GeObWindow();

  ~GeObWindow(); 
  
  /**
  рисовать список Геометрических объектов
  */  
  void Draw();

  /**
  получить dRotateZ
  */
  float GetRotateZ() { return dRotateZ; }
  /**
  установить dRotateZ
  */
  void SetRotateZ(float _dRotateZ) { dRotateZ = _dRotateZ; }

  /**
  получить dRotateX
  */
  float GetRotateX() { return dRotateX; }
  /**
  установить dRotateX
  */
  void SetRotateX(float _dRotateX) { dRotateX = _dRotateX; }

  /**
  получить dRotateY
  */
  float GetRotateY() { return dRotateY; }
  /**
  установить dRotateY
  */
  void SetRotateY(float _dRotateY) { dRotateY = _dRotateY; }

  /**
  получить dShiftX
  */
  float GetShiftX() { return dShiftX; }
  /**
  установить dShiftX
  */
  void SetShiftX(float _dShiftX) { dShiftX = _dShiftX; }

  /**
  получить dShiftY
  */
  float GetShiftY() { return dShiftY; }
  /**
  установить dShiftY
  */
  void SetShiftY(float _dShiftY) { dShiftY = _dShiftY; }

  /**
  получить dShiftZ
  */
  float GetShiftZ() { return dShiftZ; }
  /**
  установить dShiftZ
  */
  void SetShiftZ(float _dShiftZ) { dShiftZ = _dShiftZ; }

  /**
  получить xCam
  */
  float GetXCam() { return (float)vecCam.GetX(); }
  /**
  установить xCam
  */
  void SetXCam(float f) 
  {
      vecCam.SetX(f);
  };

  /**
  получить yCam
  */
  float GetYCam() { return (float)vecCam.GetY(); }
  /**
  установить yCam
  */
  void SetYCam(float f)
  {
      vecCam.SetY(f);
  };

  /**
  получить zCam
  */
  float GetZCam() { return (float)vecCam.GetZ(); }
  /**
  установить zCam
  */
  void SetZCam(float f) 
  {
      vecCam.SetZ(f);
  };

  /**
  получить xLook
  */
  float GetXLook() { return (float)vecLook.GetX(); }
  /**
  установить xLook
  */
  void SetXLook(float f)
  {
      vecLook.SetX(f);
  };

  /**
  получить yLook
  */
  float GetYLook() { return (float)vecLook.GetY(); }
  /**
  установить yLook
  */
  void SetYLook(float f)
  {
      vecLook.SetY(f);
  };

  /**
  получить zLook
  */
  float GetZLook() { return (float)vecLook.GetZ(); }
  /**
  установить zLook
  */
  void SetZLook(float f)
  {
      vecLook.SetZ(f);
  };

  /**
  получить xTop
  */
  float GetXTop() { return (float)vecTop.GetX(); }
  /**
  установить xTop
  */
  void SetXTop(float f)
  {
      vecTop.SetX(f);
  };

  /**
  получить yTop
  */
  float GetYTop() { return (float)vecTop.GetY(); }
  /**
  установить yTop
  */
  void SetYTop(float f)
  {
      vecTop.SetY(f);
  };

  /**
  получить zTop
  */
  float GetZTop() { return (float)vecTop.GetZ(); }
  /**
  установить zTop
  */
  void SetZTop(float f)
  {
      vecTop.SetZ(f);
  };

  /**
  получить полярные координаты
  */
  void GetPolar(double& _distance, double& _azimut, double& _elevation);

  /**
  установить полярные координаты
  */
  void SetPolar(double _distance, double _azimut, double _elevation);

  /**
  вычислить полярные координаты по vecCam, vecLook, vecTop
  */
  void CalcPolar(double& _distance, double& _azimut, double& _elevation);


  void gluLookAt2(GLdouble eyex, GLdouble eyey, GLdouble eyez,
      GLdouble centerx, GLdouble centery, GLdouble centerz,
      GLdouble upx, GLdouble upy, GLdouble upz);

  /**
  add GeOb to window
  */
  void Add(GeOb * cube) { vecGeOb.push_back(cube); }


  int wire = 0;
  float lasttime = 0.0, size = 1.0;
  Fl_Glut_Window* win_glut = NULL;

protected:
  std::vector<GeOb *> vecGeOb; ///< список Геометрических объектов
  Vec3 vecCam; ///< позиция камеры
  Vec3 vecLook; ///< камера смотрит сюда
  Vec3 vecTop; ///< вертикальный вектор камеры

private:
  float dRotateZ = 0, dRotateX = 0, dRotateY = 0;
  float dShiftZ = 0, dShiftX = 0, dShiftY = 0;
  //??45
  double distance = 9.0, azimut = (MY_PI * 45.0 / 180.0), elevation = (MY_PI * 30.0 / 180.0); ///< камера в полярных координатах (радианы)

};

} // namespace Grasp
#endif