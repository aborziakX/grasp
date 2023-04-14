#include "Cyl.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {

Cyl::~Cyl() {
  lasttime = 0.0;
  Clear();
}

// название
void Cyl::GetName(char buf[33])
{
    int sz = 33;// sizeof(buf)=8;
    snprintf(buf, sz, "%s id %d", u8"Цилиндр,", GetIndex());
}

//создание граней
void Cyl::Init() {
    GeOb::Init();

    double x0, y0, z0 = 0.0, x1, y1, z1 = 2.0 * radius, size = height;
    Vec3 v0, v1, v2, v3;
    double angle_step = (MY_2PI / divide);
    double angle;
    double dH = size;

    //боковая поверхность
    double z = z0;
    for (int j = 0; j < 1; j++)
    {
        angle = 0;
        for (int i = 0; i < divide; i++)
        {
            //вершины
            x0 = cos(angle) * radius;
            y0 = sin(angle) * radius;
            angle += angle_step;
            x1 = cos(angle) * radius;
            y1 = sin(angle) * radius;

            v0.Copy(x0, y0, z);
            v1.Copy(x1, y1, z);
            v2.Copy(x1, y1, z + dH);
            v3.Copy(x0, y0, z + dH);

            //грани
            Facet3* f0 = new Facet3();
            f0->Add(new Vec3(v0));
            f0->Add(new Vec3(v1));
            f0->Add(new Vec3(v2));
            f0->Add(new Vec3(v3));
            f0->SetOwner(this);
            vecFacet.push_back(f0);
        }
        z += dH;
    }

    //верхний торец
    angle = 0;
    for (int i = 0; i < divide; i++)
    {
        //вершины
        x0 = cos(angle) * radius;
        y0 = sin(angle) * radius;
        angle += angle_step;
        x1 = cos(angle) * radius;
        y1 = sin(angle) * radius;

        v0.Copy(x0, y0, z1);
        v1.Copy(x1, y1, z1);
        v2.Copy(0, 0, z1);

        //грани
        Facet3* f0 = new Facet3();
        f0->Add(new Vec3(v0));
        f0->Add(new Vec3(v1));
        f0->Add(new Vec3(v2));
        f0->SetOwner(this);
        vecFacet.push_back(f0);
    }

    //нижний торец
    angle = 0;
    for (int i = 0; i < divide; i++)
    {
        //вершины
        x0 = cos(angle) * radius;
        y0 = sin(angle) * radius;
        angle += angle_step;
        x1 = cos(angle) * radius;
        y1 = sin(angle) * radius;

        v0.Copy(x0, y0, z0);
        v1.Copy(x1, y1, z0);
        v2.Copy(0, 0, z0);

        //грани
        Facet3 * f0 = new Facet3();
        f0->Add(new Vec3(v2));
        f0->Add(new Vec3(v1));
        f0->Add(new Vec3(v0));
        f0->SetOwner(this);
        vecFacet.push_back(f0);
    }
}

} // namespace Grasp