#ifndef BEAT_GEOB_H
#define BEAT_GEOB_H

#include <vector>
#include "Vec3.h"
#define v3f(x) glVertex3fv(x)

#ifndef NULL
#define NULL 0
#endif

namespace Grasp {

/**
@class   Facet3
@brief   грань из нескольких точек
*/
class Facet3 {
public:
  Facet3() {}

  ~Facet3() { Clear(); }

  /**
  добавить Vec3 в грань
  */
  void Add(Vec3 *vec) { vecPoint.push_back(vec); }

  /**
  очистить грань
  */
  void Clear() {
    for (int i = 0; i < vecPoint.size(); i++) {
      if (vecPoint[i] == NULL)
        continue;
      delete vecPoint[i];
      vecPoint[i] = NULL;
    }
    vecPoint.clear();
  }

  /**
  вернуть точку по индексу
  */
  Vec3 *GetPoint(int index) {
    if (index >= 0 && index < vecPoint.size())
      return vecPoint[index];
    return NULL;
  }

  /**
  get bOwnColor
  */
  bool GetOwnColor() { return bOwnColor; }
  /**
  set bOwnColor
  */
  void SetOwnColor(bool _bOwnColor) { bOwnColor = _bOwnColor; }

  /**
  get каналы цвета
  */
  void GetColor(unsigned char& _red, unsigned char& _green, unsigned char& _blue) {
    _red = red;
    _green = green;
    _blue = blue;
  }
  /**
  set каналы цвета
  */
  void SetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
    red = _red;
    green = _green;
    blue = _blue;
    bOwnColor = true;
  }

  Vec3 vCenter; ///< центр
  Vec3 vNorm; ///< нормаль

  /**
  вычислить центр
  */
  void CalcCenter() {
    vCenter.Copy(0, 0, 0);
    if (vecPoint.size() == 0)
      return;
    for (int i = 0; i < vecPoint.size(); i++) {
      Vec3 *v = vecPoint[i];
      if (v == NULL)
        continue;
      vCenter.Add(v->GetX(), v->GetY(), v->GetZ());
    }
    vCenter.Scale(1.0 / vecPoint.size());
  }

  /**
  вычислить нормаль
  */
  void CalcNormal() {
    vNorm.Copy(0, 0, 0);
    if (vecPoint.size() < 3)
      return;
    Vec3 *v1 = vecPoint[0];
    Vec3 *v2 = vecPoint[1];
    Vec3 *v3 = vecPoint[2];

    Vec3 tvec1(*v2);
    tvec1.Add(-v1->GetX(), -v1->GetY(),
              -v1->GetZ()); // C# new Vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

    Vec3 tvec2(*v3);
    tvec2.Add(-v1->GetX(), -v1->GetY(),
              -v1->GetZ()); // C# new Vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

    Vec3::Product(tvec1, tvec2, vNorm);
    vNorm.Normalize();
  }

private:
  std::vector<Vec3 *> vecPoint; ///< список точек
  bool bOwnColor = false; ///< признак, что у грани собственный цвет
  unsigned char red = 0, green = 255, blue = 0; ///< каналы цвета
};

/// <summary>
/// Геометрический объект (Geometrical object - GeOb) - список граней
/// </summary>
class GeOb {
public:
  /**
  Конструктор GeOb
  */
  GeOb() {
    lasttime = 0.0;
    Init();
  }

  ~GeOb();

  /**
  очистить грани
  */
  void Clear() {
    for (int i = 0; i < vecFacet.size(); i++) {
      if (vecFacet[i] == NULL)
        continue;
      delete vecFacet[i];
      vecFacet[i] = NULL;
    }
    vecFacet.clear();
  }

  /**
  создание граней
  */
  virtual void Init() {}

  /**
  рисуем GeOb в Fl_Gl_Window
  */
  void Draw(Vec3 & vecCam);

  /**
  get dRotateZ
  */
  double GetRotateZ() { return dRotateZ; }
  /**
  set dRotateZ
  */
  void SetRotateZ(double _dRotateZ) { dRotateZ = _dRotateZ; }

  /**
  get каналы цвета
  */
  void GetColor(unsigned char &_red, unsigned char &_green, unsigned char &_blue) {
    _red = red;
    _green = green;
    _blue = blue;
  }
  /**
  set каналы цвета
  */
  void SetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
    red = _red;
    green = _green;
    blue = _blue;
  }

  /**
  вернуть грань по индексу
  */
  Facet3 *GetFacet(int index) {
    if (index >= 0 && index < vecFacet.size())
      return vecFacet[index];
    return NULL;
  }

  /**
  сдвинуть точки
  */
  void Translate(double dx, double dy, double dz);

  /**
  Transform GeOb
  */
  void Transform() { Init(); }


  float lasttime = 0.0, fsize = 1.0;

protected:
  std::vector<Facet3 *> vecFacet; // список граней

private:
  double dRotateZ = 0, dRotateX = 0, dRotateY = 0;
  double dShiftZ = 0, dShiftX = 0, dShiftY = 0;

  unsigned char red = 0, green = 255, blue = 0; ///< каналы цвета

  int wire = 0;

  bool bNeedTransf = true;
};

} // namespace Grasp
#endif