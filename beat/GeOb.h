﻿#ifndef BEAT_GEOB_H
#define BEAT_GEOB_H

#include <string>
#include <vector>
#include "Vec3.h"
#define v3f(x) glVertex3fv(x)

#ifndef NULL
#define NULL 0
#endif

namespace Grasp {

    class GeOb;

/**
@class   Facet3
@brief   грань из нескольких точек
*/
class Facet3 {
public:
  Facet3() {
      fc_id = counter++;
      temp = 0.0;
  }

  ~Facet3() { Clear(); }

  /**
  вернуть индекс
  */
  int GetIndex() {
      return fc_id;
  }

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
  вернуть число точек
  */
  int GetSize() {
      return (int)vecPoint.size();
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
  получить каналы цвета
  */
  void GetColor(unsigned char& _red, unsigned char& _green, unsigned char& _blue) {
    _red = red;
    _green = green;
    _blue = blue;
  }
  /**
  установить каналы цвета
  */
  void SetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
    red = _red;
    green = _green;
    blue = _blue;
  }

  /**
  вычислить центр и нормаль
  */
  void CalcCenterNormal() {
      if (bCalcDone) return;
      CalcCenter();
      CalcNormal();
      bCalcDone = true;
  }

  /**
  рисуем
  */
  void Draw(Vec3& vecCamDir, bool wire = true, bool norm = true);

  /**
  получить 'нормаль пересчитана'
  */
  bool GetCalcDone() {
      return bCalcDone;
  }

  /**
  установить 'нормаль пересчитана'
  */
  void SetCalcDone(bool b) {
      bCalcDone = b;
  }

  /**
  получить центр
  */
  void GetCenter(Vec3& _vCenter) { _vCenter.Copy(vCenter); }

  /**
  получить нормаль
  */
  void GetNormal(Vec3& _vNorm) { _vNorm.Copy(vNorm); }

  /**
  получить собственника
  */
  GeOb* GetOwner() {
      return owner;
  }

  /**
  установить собственника
  */
  void SetOwner(GeOb* b) {
      owner = b;
  }

  double temp; ///< временная переменная

private:
  std::vector<Vec3 *> vecPoint; ///< список точек
  unsigned char red = 0, green = 255, blue = 0; ///< каналы цвета
  GeOb* owner = NULL; ///< собственник
  Vec3 vCenter; ///< центр
  Vec3 vNorm; ///< нормаль
  bool bCalcDone = false; ///< true=нормаль пересчитана
  int fc_id;  ///< индекс
  static int counter; ///< счетчик

  /**
  вычислить центр
  */
  void CalcCenter();

  /**
  вычислить нормаль
  */
  void CalcNormal();
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
    ge_id = counter++;
    lasttime = 0.0;
    Init();
  }

  ~GeOb();

  /**
  вернуть индекс
  */
  int GetIndex() {
      return ge_id;
  }

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
  virtual void Init() {
      dShiftX = 0; dShiftY = 0; dShiftZ = 0;
      Clear();
  }

  /**
  название
  */
  virtual void GetName(char buf[33]) {}

  /**
  рисуем GeOb в Fl_Gl_Window
  */
  void Draw(Vec3 & vecCamDir);

  /**
  получить сдвиги
  */
  void GetShift(double& _dShiftX, double& _dShiftY, double& _dShiftZ) {
      _dShiftX = dShiftX;
      _dShiftY = dShiftY;
      _dShiftZ = dShiftZ;
  }

  /**
  получить масштабирование
  */
  void GetScale(double& _dScaleX, double& _dScaleY, double& _dScaleZ) {
      _dScaleX = dScaleX;
      _dScaleY = dScaleY;
      _dScaleZ = dScaleZ;
  }

  /**
  получить каналы цвета
  */
  void GetColor(unsigned char &_red, unsigned char &_green, unsigned char &_blue) {
    _red = red;
    _green = green;
    _blue = blue;
  }

  /**
  установить каналы цвета
  */
  void SetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
    red = _red;
    green = _green;
    blue = _blue;

    for (int i = 0; i < vecFacet.size(); i++) {
        if (vecFacet[i] == NULL)
            continue;
        vecFacet[i]->SetColor(red, green, blue);
    }
  }

  /**
  вернуть число граней
  */
  int GetSize() {
      return (int)vecFacet.size();
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
  масштабировать точки
  */
  void Scale(double dx, double dy, double dz);

  /**
  Transform GeOb
  */
  void Transform() { Init(); }

  /**
  получить каналы цвета ярче
  */
  void GetColorBright(unsigned char& _red, unsigned char& _green, unsigned char& _blue, double d, double add) {
      if (d < 0.0) d = 1.0;
      double r = ((double)red * d + add);
      double g = ((double)green * d + add);
      double b = ((double)blue * d + add);

      _red = (r > 255) ? 255 : (unsigned char)r;
      _green = (g > 255) ? 255 : (unsigned char)g;
      _blue = (b > 255) ? 255 : (unsigned char)b;
  }

  float lasttime = 0.0, diameter = 1.0;
  bool bWire = false; ///< true=только ребра
  bool bNormal = false; ///< true=рисовать нормали
  bool bSelect = false; ///< true=рисовать куб вокруг

protected:
  std::vector<Facet3 *> vecFacet; ///< список граней
  std::string name; ///< название

private:
  double dScaleX = 1.0, dScaleY = 1.0, dScaleZ = 1.0; ///< масштабирование
  double dShiftX = 0, dShiftY = 0, dShiftZ = 0; ///< сдвиг

  unsigned char red = 0, green = 255, blue = 0; ///< каналы цвета

  bool bNeedTransf = true;

  int ge_id;  ///< индекс
  static int counter; ///< счетчик
};

} // namespace Grasp
#endif