#include "Models.h"

namespace Grasp {
  Models::Models()
  {
  }

  Models::~Models()
  {
  }

  // модифицирует список объектов согласно указанной модели model
  int Models::Run(string& model, GeObWindow* gewin, long now)
  {
	  if (model == "z-sin") return RunZSin(gewin, now);
	  else if (model == "scale-sin") return RunScaleSin(gewin, now);
	  return 0;
  }

  // модифицирует список объектов согласно модели "z-sin"
  int Models::RunZSin(GeObWindow* gewin, long now)
  {
	  double cur = 0.001 * (now % 2000 - 1000) * MY_PI;
	  double x, y, z;

	  int sz = gewin->GetSize();
	  for (int i = 0; i < sz; i++)
	  {
		  GeOb* geob = gewin->GetObj(i);
		  if (geob == NULL) continue;

		  geob->GetShift(x, y, z);
		  geob->Translate(x, y, sin(cur));
	  }
	  return 1;
  }

  // модифицирует список объектов согласно модели "scale-sin"
  int Models::RunScaleSin(GeObWindow* gewin, long now)
  {
	  double cur = 0.001 * (now % 2000 - 1000) * MY_PI;
	  double x, y, z;

	  int sz = gewin->GetSize();
	  for (int i = 0; i < sz; i++)
	  {
		  GeOb* geob = gewin->GetObj(i);
		  if (geob == NULL) continue;

		  //geob->GetScale(x, y, z);
		  z = 1.0 + 0.5 * sin(cur);
		  geob->Scale(z, z, z);
	  }
	  return 1;
  }

} // namespace Grasp