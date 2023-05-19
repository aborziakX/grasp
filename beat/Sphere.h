#ifndef BEAT_Sphere_H
#define BEAT_Sphere_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Sphere
@brief   Геометрический объект Сфера (superclass - GeOb)
*/
class Sphere : public GeOb {
public:
  /**
  Конструктор Сферы с центром в (0, 0, 0.5) и радиусом 0.5
  */
  Sphere(int _steps = 8)
    : GeOb(){
    geom_type = geom_type_enum::GO_SPHERE;
    nSide = _steps;
    Init();
  };

  ~Sphere();

  /**
  создание граней
  */
  void Init() override;

  /**
  название
  */
  void GetName(char buf[33]) override;

protected:
private:
    double height = 1.0; ///< высота
    double radius = 0.5; ///< радиус
};


} // namespace Grasp
#endif