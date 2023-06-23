#include "GeOb.h"
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
void Facet3::Draw(Vec3& vecCamDir, bool wire, bool norm, bool bDouble)
{
    unsigned char _red, _green, _blue;
    Facet3* f = this;

    if (f->owner != NULL && f->owner->bSelect)
        f->owner->GetColorBright(_red, _green, _blue, 1.3, 144.0); // ярче
    else f->GetColor(_red, _green, _blue);

    f->CalcCenter();
    f->CalcNormal();
    //cout << fixed << setprecision(18) << f->vNorm.GetX() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetY() << endl;
    //cout << fixed << setprecision(18) << f->vNorm.GetZ() << endl;

    double sca = (vecCamDir.ScalarProduct(f->vNorm)); //между -1 и 1
    int sz = f->GetSize();
    if (sz == 2) sca = -1;
    if (sca > 0 && !bDouble)
    { // невидимая грань
        _red = 0;
        _green = 0;
        _blue = 0;
    }
    sca = abs(sca);
    //cout << fixed << setprecision(18) << sca << endl;
    //cout << endl;

    int mode = GL_POLYGON;
    if (wire) mode = GL_LINE_LOOP;
    else if (sz == 3) mode = GL_TRIANGLES;
    else if (sz == 2) mode = GL_LINE;

    glBegin(mode);
    glColor3ub((char)(sca * _red), (char)(sca * _green), (char)(sca * _blue));

    for (int j = 0; j < f->GetSize(); j++) {
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

// получить каналы цвета по индексу
void Facet3::GetColorByIndex(int ind, unsigned char& _red, unsigned char& _green, unsigned char& _blue) {
    if (ind == 0) { //red
        _red = 255;
        _green = 0;
        _blue = 0;
    }
    else if (ind == 1) { //orange
        _red = 255;
        _green = 190;
        _blue = 0;
    }
    else if (ind == 2) { //yellow
        _red = 255;
        _green = 255;
        _blue = 0;
    }
    else if (ind == 3) { //green
        _red = 0;
        _green = 255;
        _blue = 0;
    }
    else if (ind == 4) { //light blue
        _red = 0;
        _green = 0;
        _blue = 255;
    }
    else if (ind == 5) { //blue
        _red = 0;
        _green = 0;
        _blue = 127;
    }
    else if (ind == 6) { //violet
        _red = 255;
        _green = 0;
        _blue = 255;
    }
    else if (ind == 7) { //white
        _red = 255;
        _green = 255;
        _blue = 255;
    }
    else if (ind == 8) { //light gray
        _red = 190;
        _green = 190;
        _blue = 190;
    }
    else if (ind == 9) { //gray
        _red = 127;
        _green = 127;
        _blue = 127;
    }
    else { //black
        _red = 0;
        _green = 0;
        _blue = 0;
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
    // сдвиги с учетом предыдущего
    double dx_2 = dx - dShiftX;
    double dy_2 = dy - dShiftY;
    double dz_2 = dz - dShiftZ;
    
    for (int i = 0; i < vecFacet.size(); i++)
    { //цикл по граням
        Facet3 *f = vecFacet[i];

        for (int j = 0; j < f->GetSize(); j++)
        { //цикл по точкам грани
            Vec3 *p = f->GetPoint(j);
            if (p != NULL) {
                p->Add(dx_2, dy_2, dz_2);
            }
        }
        f->SetCalcDone(false);
    }
    // абсолютный сдвиг
    dShiftX = dx;
    dShiftY = dy;
    dShiftZ = dz;
}

// масштабировать точки
void GeOb::Scale(double dx, double dy, double dz)
{
    if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) return;
    // масштаб с учетом предыдущего
    double dx_2 = dx / dScaleX;
    double dy_2 = dy / dScaleY;
    double dz_2 = dz / dScaleZ;

    for (int i = 0; i < vecFacet.size(); i++)
    {   //цикл по граням
        Facet3* f = vecFacet[i];

        for (int j = 0; j < f->GetSize(); j++) {
            //цикл по точкам грани
            Vec3* p = f->GetPoint(j);
            if (p != NULL) {
                p->Scale3(dx_2, dy_2, dz_2);
            }
        }
        f->SetCalcDone(false);
    }
    // абсолютный масштаб
    dScaleX = dx;
    dScaleY = dy;
    dScaleZ = dz;
}

} // namespace Grasp