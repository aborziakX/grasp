#ifndef BEAT_INI_H
#define BEAT_INI_H

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {
/**
@class   beat_ini
@brief   разбор ini-файла
*/
class BeatIni {

public:
    BeatIni();

  ~BeatIni();

  /**
  get dRotateZ
  */
  float GetRotateZ() { return dRotateZ; }
  /**
  set dRotateZ
  */
  void SetRotateZ(float _dRotateZ) { dRotateZ = _dRotateZ; }


  int wire = 0;
  float lasttime = 0.0, size = 1.0, speed = 0.0;

protected:
private:
  float dRotateZ = 0, dRotateX = 0, dRotateY = 0; ///< bla-bla
  float dShiftZ = 0, dShiftX = 0, dShiftY = 0, dCamZ = 0;

};

} // namespace Grasp
#endif