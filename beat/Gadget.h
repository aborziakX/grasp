#ifndef BEAT_Gadget_H
#define BEAT_Gadget_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Gadget
@brief   Геометрический объект Точка учета (superclass - GeOb)
*/
class Gadget : public GeOb {
public:
  /**
  Конструктор Точка учета с центром в (0, 0, 0.5) и радиусом у основания 0.5
  */
  // на основе конуса
  Gadget(int _steps = 3)
    : GeOb(){
    geom_type = geom_type_enum::GO_GADGET;
    nSide = _steps;
    Init();
  };

  ~Gadget();

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