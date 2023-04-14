#ifndef BEAT_CYL_H
#define BEAT_CYL_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Cyl
@brief   Геометрический объект Цилиндр (superclass - GeOb)
*/
class Cyl : public GeOb {
public:
  /**
  Конструктор Цилиндра с центром в (0, 0, 0.5) и радиусом 1
  */
  Cyl(int _steps = 8)
    : GeOb(){
    divide = _steps;
    Init();
  };

  ~Cyl();

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
    int divide; ///< число шагов
};


} // namespace Grasp
#endif