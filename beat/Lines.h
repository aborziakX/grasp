#ifndef BEAT_LINES_H
#define BEAT_LINES_H

#include <vector>
#include "Vec3.h"
#include "GeOb.h"

namespace Grasp {

/**
@class   Lines
@brief   Геометрический объект - набор линий (superclass - GeOb)
*/
class Lines : public GeOb {
public:
  /**
  Конструктор
  */
    Lines()
    : GeOb(){
    geom_type = geom_type_enum::GO_LINES;
    bWire = true;
    Init();
  };

  ~Lines();

  /**
  создание граней
  */
  void Init() override;

  /**
  название
  */
  void GetName(char buf[33]) override;

  /**
  добавить линию
  */
  void AddLine(Vec3& v0, Vec3& v1);

protected:
private:


};


} // namespace Grasp
#endif