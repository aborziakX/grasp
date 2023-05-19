#include "Sphere.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {

Sphere::~Sphere() {
  lasttime = 0.0;
  Clear();
}

// название
void Sphere::GetName(char buf[33])
{
    int sz = 33;// sizeof(buf)=8;
    snprintf(buf, sz, "%s id %d", u8"Сфера,", GetIndex());
}

//создание граней
void Sphere::Init() {
    GeOb::Init();

    double x0, y0, z0, x1, y1, z1;
    Vec3 v0, v1, v2, v3;
    double angle_step = (MY_2PI / nSide);
    double angle_h_step = (MY_2PI / nSide) / 2;
    double angle, diam = 2.0 * radius;
    int divide = nSide;

    double angle_h = 0, rad_1, rad_2;
    for (int j = 0; j < divide / 2; j++)
    {
        rad_1 = cos(angle_h) * radius;
        z0 = sin(angle_h) * radius + radius;
        angle_h += angle_h_step;
        rad_2 = cos(angle_h) * radius;
        z1 = sin(angle_h) * radius + radius;
        angle = 0;

        for (int i = 0; i < divide; i++)
        {
            // вершины
            x0 = cos(angle) * rad_1;
            y0 = sin(angle) * rad_1;
            angle += angle_step;
            x1 = cos(angle) * rad_1;
            y1 = sin(angle) * rad_1;

            v0.Copy(x0, y0, z0);
            v1.Copy(x1, y1, z0);

            angle -= angle_step;
            x0 = cos(angle) * rad_2;
            y0 = sin(angle) * rad_2;
            angle += angle_step;
            x1 = cos(angle) * rad_2;
            y1 = sin(angle) * rad_2;

            v2.Copy(x1, y1, z1);
            v3.Copy(x0, y0, z1);

            Facet3 * fac0_a = new Facet3();
            if (j < divide / 2 - 1)
            {
                //new Facet3(v0, v1, v2, v3) :
                fac0_a->Add(new Vec3(v0));
                fac0_a->Add(new Vec3(v1));
                fac0_a->Add(new Vec3(v2));
                fac0_a->Add(new Vec3(v3));
            }
            else
            {
                //new Facet3(v0, v1, v3);
                fac0_a->Add(new Vec3(v0));
                fac0_a->Add(new Vec3(v1));
                fac0_a->Add(new Vec3(v3));
            }
            fac0_a->SetOwner(this);
            vecFacet.push_back(fac0_a);

            // симметрично вниз
            v0.SetZ(-v0.GetZ() + diam);
            v1.SetZ(-v1.GetZ() + diam);
            v2.SetZ(-v2.GetZ() + diam);
            v3.SetZ(-v3.GetZ() + diam);
            fac0_a = new Facet3();
            if (j < divide / 2 - 1)
            {
                //new Facet3(v0, v3, v2, v1)
                fac0_a->Add(new Vec3(v0));
                fac0_a->Add(new Vec3(v3));
                fac0_a->Add(new Vec3(v2));
                fac0_a->Add(new Vec3(v1));
            }
            else
            {
                //new Facet3(v0, v3, v1);
                fac0_a->Add(new Vec3(v0));
                fac0_a->Add(new Vec3(v3));
                fac0_a->Add(new Vec3(v1));
            }
            fac0_a->SetOwner(this);
            vecFacet.push_back(fac0_a);            
        }
    }
}

} // namespace Grasp