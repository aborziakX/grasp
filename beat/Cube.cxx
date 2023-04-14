#include "Cube.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {

Cube::~Cube() {
  lasttime = 0.0;
  Clear();
}

// название
void Cube::GetName(char buf[33])
{
    int sz = 33;// sizeof(buf);
    snprintf(buf, sz, "%s id %d", u8"Куб,", GetIndex());
}
//создание граней
void Cube::Init() {
    GeOb::Init();

    if (vecFacet.size() == 0) {
        for (int j = 0; j < 6; j++) {
            Facet3 *f0 = new Facet3();
            for (int i = 0; i < 4; i++) {
                f0->Add(new Vec3(v3));
            }
            f0->SetOwner(this);
            vecFacet.push_back(f0);
        }
    }

    //задняя
    Facet3 *f0 = vecFacet[0];
    f0->GetPoint(0)->Copy(v0);
    f0->GetPoint(1)->Copy(v3);
    f0->GetPoint(2)->Copy(v2);
    f0->GetPoint(3)->Copy(v1);

    //передняя к Z
    Facet3 *f1 = vecFacet[1];
    f1->GetPoint(0)->Copy(v4);
    f1->GetPoint(1)->Copy(v5);
    f1->GetPoint(2)->Copy(v6);
    f1->GetPoint(3)->Copy(v7);

    //нижняя
    Facet3 *f2 = vecFacet[2];
    f2->GetPoint(0)->Copy(v0);
    f2->GetPoint(1)->Copy(v1);
    f2->GetPoint(2)->Copy(v5);
    f2->GetPoint(3)->Copy(v4);

    //верхняя
    Facet3 *f3 = vecFacet[3];
    f3->GetPoint(0)->Copy(v2);
    f3->GetPoint(1)->Copy(v3);
    f3->GetPoint(2)->Copy(v7);
    f3->GetPoint(3)->Copy(v6);

    //левая
    Facet3 *f4 = vecFacet[4];
    f4->GetPoint(0)->Copy(v0);
    f4->GetPoint(1)->Copy(v4);
    f4->GetPoint(2)->Copy(v7);
    f4->GetPoint(3)->Copy(v3);

    //правая
    Facet3 *f5 = vecFacet[5];
    f5->GetPoint(0)->Copy(v1);
    f5->GetPoint(1)->Copy(v2);
    f5->GetPoint(2)->Copy(v6);
    f5->GetPoint(3)->Copy(v5);
}

} // namespace Grasp