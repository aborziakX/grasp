#include "Cube.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>


namespace Grasp {
Cube::~Cube() {
  lasttime = 0.0;
}

/* Draw a colored cube */
void Cube::Draw() {
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 0, 255);
  v3f(v0);
  v3f(v1);
  v3f(v2);
  v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 255, 255);
  v3f(v4);
  v3f(v5);
  v3f(v6);
  v3f(v7);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 0, 255);
  v3f(v0);
  v3f(v1);
  v3f(v5);
  v3f(v4);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 255, 0);
  v3f(v2);
  v3f(v3);
  v3f(v7);
  v3f(v6);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 255, 0);
  v3f(v0);
  v3f(v4);
  v3f(v7);
  v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 0, 0);
  v3f(v1);
  v3f(v2);
  v3f(v6);
  v3f(v5);
  glEnd();
}


} // namespace Grasp