#ifndef BEAT_CUBE_H
#define BEAT_CUBE_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Cube
@brief   Геометрический объект Куб (superclass - GeOb)
*/
class Cube : public GeOb {
public:
  /**
  Конструктор Куба
  */
  Cube()
    : GeOb(){
    Init();
  };

  ~Cube();

  /**
  создание граней
  */
  void Init() override;

protected:
private:
  /* точки куба */
  double v0[3] = {-0.5, -0.5, -0.5 };
  double v1[3] = {0.5, -0.5, -0.5 };
  double v2[3] = {0.5, 0.5, -0.5 };
  double v3[3] = {-0.5, 0.5, -0.5 };
  double v4[3] = {-0.5, -0.5, 0.5 };
  double v5[3] = {0.5, -0.5, 0.5 };
  double v6[3] = {0.5, 0.5, 0.5 };
  double v7[3] = {-0.5, 0.5, 0.5 };


};


} // namespace Grasp
#endif