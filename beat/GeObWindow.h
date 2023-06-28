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

  void Reset(); ///< очистка
  
  /**
  рисовать список Геометрических объектов
  */  
  void Draw();

  /**
  получить dRotateZ
  */
  double GetRotateZ() { return dRotateZ; }
  /**
  установить dRotateZ
  */
  void SetRotateZ(double _dRotateZ) { dRotateZ = _dRotateZ; }

  /**
  получить dRotateX
  */
  double GetRotateX() { return dRotateX; }
  /**
  установить dRotateX
  */
  void SetRotateX(double _dRotateX) { dRotateX = _dRotateX; }

  /**
  получить dRotateY
  */
  double GetRotateY() { return dRotateY; }
  /**
  установить dRotateY
  */
  void SetRotateY(double _dRotateY) { dRotateY = _dRotateY; }

  /**
  получить dShiftX
  */
  double GetShiftX() { return dShiftX; }
  /**
  установить dShiftX
  */
  void SetShiftX(double _dShiftX) { dShiftX = _dShiftX; }

  /**
  получить dShiftY
  */
  double GetShiftY() { return dShiftY; }
  /**
  установить dShiftY
  */
  void SetShiftY(double _dShiftY) { dShiftY = _dShiftY; }

  /**
  получить dShiftZ
  */
  double GetShiftZ() { return dShiftZ; }
  /**
  установить dShiftZ
  */
  void SetShiftZ(double _dShiftZ) { dShiftZ = _dShiftZ; }

  /**
  получить xCam
  */
  double GetXCam() { return vecCam.GetX(); }
  /**
  установить xCam
  */
  void SetXCam(double f)
  {
      vecCam.SetX(f);
  };

  /**
  получить yCam
  */
  double GetYCam() { return vecCam.GetY(); }
  /**
  установить yCam
  */
  void SetYCam(double f)
  {
      vecCam.SetY(f);
  };

  /**
  получить zCam
  */
  double GetZCam() { return vecCam.GetZ(); }
  /**
  установить zCam
  */
  void SetZCam(double f)
  {
      vecCam.SetZ(f);
  };

  /**
  получить xLook
  */
  double GetXLook() { return vecLook.GetX(); }
  /**
  установить xLook
  */
  void SetXLook(double f)
  {
      vecLook.SetX(f);
  };

  /**
  получить yLook
  */
  double GetYLook() { return vecLook.GetY(); }
  /**
  установить yLook
  */
  void SetYLook(double f)
  {
      vecLook.SetY(f);
  };

  /**
  получить zLook
  */
  double GetZLook() { return vecLook.GetZ(); }
  /**
  установить zLook
  */
  void SetZLook(double f)
  {
      vecLook.SetZ(f);
  };

  /**
  получить xTop
  */
  double GetXTop() { return vecTop.GetX(); }
  /**
  установить xTop
  */
  void SetXTop(double f)
  {
      vecTop.SetX(f);
  };

  /**
  получить yTop
  */
  double GetYTop() { return vecTop.GetY(); }
  /**
  установить yTop
  */
  void SetYTop(double f)
  {
      vecTop.SetY(f);
  };

  /**
  получить zTop
  */
  double GetZTop() { return vecTop.GetZ(); }
  /**
  установить zTop
  */
  void SetZTop(double f)
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

  //from http://www.mesa3d.org
  void gluLookAt2(GLdouble eyex, GLdouble eyey, GLdouble eyez,
      GLdouble centerx, GLdouble centery, GLdouble centerz,
      GLdouble upx, GLdouble upy, GLdouble upz);

  /**
  добавить GeOb в список
  */
  void Add(GeOb* cube);

  /**
  вернуть число элементов в vecGeOb
  */
  int GetSize() { return (int)vecGeOb.size(); }

  /**
  вернуть GeOb по индексу
  */
  GeOb* GetObj(int index) {
      int sz = (int)vecGeOb.size();
      if( index >= sz || index < 0 ) return NULL;
      return vecGeOb[index];
  }

  /**
  удалить GeOb из списка
  */
  void Delete(GeOb* cube);

  /**
  создать GeOb и добавить в список (по умолчанию)
  */
  GeOb * CreateObj(geom_type_enum typ, double x_0, double y_0, double z_0, double dx, double dy, double dz,
      int nSide, int clr, string& fin, bool bAdd = true);

  /**
  найти GeOb по id
  */
  GeOb* FindObjById(int id);

  /**
  обновить список граней vecFacet
  */
  void UpdateFacets();

  // очистить список граней vecFacet
  void ClearFacets();

  int wire = 0;
  Fl_Glut_Window* win_glut = NULL;

protected:
  std::vector<GeOb *> vecGeOb; /** список Геометрических объектов */

  std::vector<Facet3*> vecFacet; /** список граней */

  Vec3 vecCam; /** позиция камеры */
  Vec3 vecLook; /** камера смотрит сюда */
  Vec3 vecTop; /** вертикальный вектор камеры */

private:
  double dRotateZ = 0, dRotateX = 0, dRotateY = 0;
  double dShiftZ = 0, dShiftX = 0, dShiftY = 0;
  //??45
  double distance = 9.0, azimut = (MY_PI * 45.0 / 180.0), elevation = (MY_PI * 30.0 / 180.0); ///< камера в полярных координатах (радианы)

  bool bResort; /** true = пересортировать vecFacet */
  void ResortFacet(Vec3& vecCam, Vec3& vecCamDir);
  bool bCanDraw = true;
};

} // namespace Grasp
#endif