#include "GeOb.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <iostream>
#include <iomanip>
using namespace std;

namespace Grasp {

GeOb::~GeOb() {
  lasttime = 0.0;
  Clear();
}

// рисуем GeOb
void GeOb::Draw(Vec3 & vecCam) {
  //cout << fixed << setprecision(18) << vecCam.GetX() << endl;
  //cout << fixed << setprecision(18) << vecCam.GetY() << endl;
  //cout << fixed << setprecision(18) << vecCam.GetZ() << endl;

  unsigned char _red, _green, _blue;
  for (int i = 0; i < vecFacet.size(); i++) {
    //цикл по граням
    Facet3 *f = vecFacet[i];
    f->CalcNormal();
    //cout << fixed << setprecision(18) << f->vNorm.GetX() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetY() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetZ() << endl;
    double sca = ( vecCam.ScalarProduct(f->vNorm) ); //между -1 и 1
    if (sca > 0) continue; // невидимая грань
    sca = abs(sca);
    //cout << fixed << setprecision(18) << sca << endl;
    //cout << endl;

    glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
    if (!f->GetOwnColor())
      glColor3ub((char)(sca * red), (char)(sca * green), (char)(sca * blue));
    else {
      f->GetColor(_red, _green, _blue);
      glColor3ub((char)(sca * _red), (char)(sca * _green), (char)(sca * _blue));
    }

    for (int j = 0; j < 4; j++) {
      //цикл по точкам грани
      Vec3 *p = f->GetPoint(j);
      if (p != NULL) {        
        glVertex3f((float)p->GetX(), (float)p->GetY(), (float)p->GetZ());
      }
    }
    glEnd();
  }
}

// сдвинуть точки
void GeOb::Translate(double dx, double dy, double dz) {
  for (int i = 0; i < vecFacet.size(); i++) {
    //цикл по граням
    Facet3 *f = vecFacet[i];

    for (int j = 0; j < 4; j++) {
      //цикл по точкам грани
      Vec3 *p = f->GetPoint(j);
      if (p != NULL) {
        p->Add(dx, dy, dz);
      }
    }
  }
}

} // namespace Grasp