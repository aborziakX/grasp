#ifndef BEAT_Cone_H
#define BEAT_Cone_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Cone
@brief   Геометрический объект Конус (superclass - GeOb)
*/
class Cone : public GeOb {
public:
  /**
  Конструктор Конуса с центром в (0, 0, 0.5) и радиусом у основания 0.5
  */
  Cone(int _steps = 8)
    : GeOb(){
    geom_type = geom_type_enum::GO_TETRA;
    nSide = _steps;
    Init();
  };

  ~Cone();

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