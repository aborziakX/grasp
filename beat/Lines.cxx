#include "Lines.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

namespace Grasp {

Lines::~Lines() {
  lasttime = 0.0;
  Clear();
}

// название
void Lines::GetName(char buf[33])
{
    int sz = 33;// sizeof(buf);
    snprintf(buf, sz, "%s id %d", u8"Линии,", GetIndex());
}

//создание граней
void Lines::Init() {
    GeOb::Init();
}

// добавить линию
void Lines::AddLine(Vec3 & v0, Vec3& v1)
{
    //грань из 2-х точек
    Facet3* f0 = new Facet3();
    f0->Add(new Vec3(v0));
    f0->Add(new Vec3(v1));
    f0->SetOwner(this);
    vecFacet.push_back(f0);
}

} // namespace Grasp