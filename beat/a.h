#ifndef BEAT_CUBE_H
#define BEAT_CUBE_H

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {
/**
@class   GeObWindow
@brief   Fl_Gl_Window со списком Геометрических объектов (Geometrical object - GeOb)
*/
class GeObWindow : public Fl_Gl_Window {
  void draw() FL_OVERRIDE;
  int handle(int e) FL_OVERRIDE;

public:
  /**
  Creates a new Fl_Gl_Window widget using the given position,
  size, and label string
  */
  GeObWindow(int x, int y, int w, int h, const char *l = 0)
    : Fl_Gl_Window(x, y, w, h, l) {
    lasttime = 0.0;
    box(FL_DOWN_FRAME);
  }

  ~GeObWindow();


  /**
  Specify Fl_Gl_Window
  */
  void SetWindow(Fl_Gl_Window *_gl_win) { gl_win = _gl_win; }

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