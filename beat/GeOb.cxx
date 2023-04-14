﻿#include "GeOb.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <iostream>
#include <iomanip>
using namespace std;

namespace Grasp {
int Facet3::counter = 0;

/**
вычислить центр
*/
void Facet3::CalcCenter() 
{
    vCenter.Copy(0, 0, 0);
    if (vecPoint.size() == 0)
        return;
    for (int i = 0; i < vecPoint.size(); i++) {
        Vec3* v = vecPoint[i];
        if (v == NULL)
            continue;
        vCenter.Add(v->GetX(), v->GetY(), v->GetZ());
    }
    vCenter.Scale(1.0 / vecPoint.size());
}

/**
вычислить нормаль
*/
void Facet3::CalcNormal() 
{
    vNorm.Copy(0, 0, 0);
    if (vecPoint.size() < 3)
        return;
    Vec3* v1 = vecPoint[0];
    Vec3* v2 = vecPoint[1];
    Vec3* v3 = vecPoint[2];

    Vec3 tvec1(*v2);
    tvec1.Add(-v1->GetX(), -v1->GetY(),
        -v1->GetZ()); // C# new Vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

    Vec3 tvec2(*v3);
    tvec2.Add(-v1->GetX(), -v1->GetY(),
        -v1->GetZ()); // C# new Vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

    Vec3::Product(tvec1, tvec2, vNorm);
    vNorm.Normalize();
}

// рисуем Facet3
void Facet3::Draw(Vec3& vecCamDir, bool wire, bool norm)
{
    unsigned char _red, _green, _blue;
    Facet3* f = this;
    f->GetColor(_red, _green, _blue);
    f->CalcCenter();
    f->CalcNormal();
    //cout << fixed << setprecision(18) << f->vNorm.GetX() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetY() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetZ() << endl;
    double sca = (vecCamDir.ScalarProduct(f->vNorm)); //между -1 и 1
    //sca = -1; //test
    if (sca > 0)
    { // невидимая грань
        _red = 0;
        _green = 0; 
        _blue = 0;
    }
    sca = abs(sca);
    //cout << fixed << setprecision(18) << sca << endl;
    //cout << endl;

    glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
    glColor3ub((char)(sca * _red), (char)(sca * _green), (char)(sca * _blue));

    for (int j = 0; j < 4; j++) {
        //цикл по точкам грани
        Vec3* p = f->GetPoint(j);
        if (p != NULL) {
            glVertex3f((float)p->GetX(), (float)p->GetY(), (float)p->GetZ());
        }
    }
    glEnd();

    if (norm)
    {   //нормаль
        glBegin(GL_LINE_LOOP);
        glColor3ub((char)255, (char)255, (char)255);
        glVertex3f((float)f->vCenter.GetX(), (float)f->vCenter.GetY(), (float)f->vCenter.GetZ());
        glVertex3f((float)(f->vNorm.GetX() + f->vCenter.GetX()),
            (float)(f->vNorm.GetY() + f->vCenter.GetY()),
            (float)(f->vNorm.GetZ() + f->vCenter.GetZ()));
        glEnd();
    }
}
//== end Facet3

int GeOb::counter = 0;

GeOb::~GeOb() {
  lasttime = 0.0;
  Clear();
}

// рисуем GeOb
void GeOb::Draw(Vec3 & vecCamDir) {
  for (int i = 0; i < vecFacet.size(); i++) {
    //цикл по граням
    Facet3 *f = vecFacet[i];
    bool wire = false, norm = false;
    GeOb * owner = f->GetOwner();
    if (owner != NULL)
    {
        wire = owner->bWire;
        norm = owner->bNormal;
    }
    f->Draw(vecCamDir, wire, norm);
  }
}

// сдвинуть точки
void GeOb::Translate(double dx, double dy, double dz) 
{
  for (int i = 0; i < vecFacet.size(); i++) 
  { //цикл по граням
    Facet3 *f = vecFacet[i];

    for (int j = 0; j < 4; j++) {
      //цикл по точкам грани
      Vec3 *p = f->GetPoint(j);
      if (p != NULL) {
        p->Add(dx, dy, dz);
      }
    }
    f->SetCalcDone(false);
  }
  dShiftX += dx;
  dShiftY += dy;
  dShiftZ += dz;
}

// масштабировать точки
void GeOb::Scale(double dx, double dy, double dz)
{
    for (int i = 0; i < vecFacet.size(); i++)
    { //цикл по граням
        Facet3* f = vecFacet[i];

        for (int j = 0; j < 4; j++) {
            //цикл по точкам грани
            Vec3* p = f->GetPoint(j);
            if (p != NULL) {
                p->Scale3(dx, dy, dz);
            }
        }
        f->SetCalcDone(false);
    }
}

} // namespace Grasp