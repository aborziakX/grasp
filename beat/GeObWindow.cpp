#include "GeObWindow.h"

namespace Grasp {
GeObWindow::~GeObWindow() 
{
  for (int i = 0; i < vecGeOb.size(); i++) {
    if (vecGeOb[i] == NULL)
      continue;
    delete vecGeOb[i];
    vecGeOb[i] = NULL;
  }
}

int GeObWindow::handle(int e) {
  switch (e) {
    case FL_ENTER:
      cursor(FL_CURSOR_CROSS);
      break;
    case FL_LEAVE:
      cursor(FL_CURSOR_DEFAULT);
      break;
  }
  return Fl_Gl_Window::handle(e);
}

void GeObWindow::draw() {
  if (!valid()) {
    glLoadIdentity();
    glViewport(0, 0, pixel_w(), pixel_h());
    glEnable(GL_DEPTH_TEST);
    glFrustum(-1, 1, -1, 1, 2, 10000);
    glTranslatef(0, 0, -10);
    glClearColor(0.4f, 0.4f, 0.4f, 0);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(float(dRotateZ * 360), 0, 0, 1);
  glRotatef(float(dRotateX * 360), 1, 0, 0);
  glRotatef(float(dRotateY * 360), 0, 1, 0);
  glTranslatef(float(dShiftX), float(dShiftY), float(dShiftZ));
  glScalef(float(size), float(size), float(size));
  // fraw all GeOb here
  //drawcube(wire);
  for (int i = 0; i < vecGeOb.size(); i++) {
    if (vecGeOb[i] == NULL)
      continue;
    vecGeOb[i]->Draw();
  }

  glPopMatrix();

  // draw title
  gl_color(FL_GRAY);
  glDisable(GL_DEPTH_TEST);
  gl_font(FL_HELVETICA_BOLD, 16);
  gl_draw(wire ? "Cube: wire" : "Cube: flat", -4.5f, -4.5f);
  glEnable(GL_DEPTH_TEST);

  // if an OpenGL graphics driver is installed, give it a chance
  // to draw additional graphics
  Fl_Gl_Window::draw();
}

} // namespace Grasp