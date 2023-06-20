#ifndef BEAT_Poly_H
#define BEAT_Poly_H

#include <vector>
#include <string>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Poly
@brief   Геометрический объект Полимер (superclass - GeOb)
*/
class Poly : public GeOb {
public:
  /**
  Конструктор Конуса с центром в (0, 0, 0.5) и радиусом у основания 0.5
  */
  Poly(const char * _fname)
    : GeOb(){
    geom_type = geom_type_enum::GO_POLY;
    fname = _fname;
    Init();
  };

  ~Poly();

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
    std::string fname; ///< радиус

    //void Load();
};


} // namespace Grasp
#endif