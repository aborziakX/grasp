#include "Models.h"

namespace Grasp {
  Models::Models()
  {
  }

  Models::~Models()
  {
  }

  unsigned long Models::prevtime = Utils::getTime(); // количество миллисекунд

  // модифицирует список объектов согласно указанной модели model
  int Models::Run(string& model, GeObWindow* gewin, unsigned long now, BeatIni* bini)
  {
	  if (model == "z-sin") return RunZSin(gewin, now, bini);
	  else if (model == "scale-sin") return RunScaleSin(gewin, now, bini);
	  else if (model == "gravity") return RunGravity(gewin, now, bini);
	  prevtime = now;
	  return 0; 
  }

  // модифицирует список объектов согласно модели "z-sin"
  int Models::RunZSin(GeObWindow* gewin, unsigned long now, BeatIni* bini)
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
  int Models::RunScaleSin(GeObWindow* gewin, unsigned long now, BeatIni* bini)
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

  // модифицирует список объектов согласно модели "gravity"
  int Models::RunGravity(GeObWindow* gewin, unsigned long now, BeatIni* bini)
  {
	  double g = 9.8; // ускорение свободного падения в м/сек**2
	  double dt = ((now - prevtime) / 50000.0); // шаг времени в сек.
	  double x, y, z, zNew = 0;
	  char buf[33];
	  int sz = gewin->GetSize();
	  for (int i = 0; i < sz; i++)
	  {
		  GeOb* geob = gewin->GetObj(i);
		  if (geob == NULL) continue;
		  int ge_id = geob->GetIndex();
		  TParam* par = bini != NULL ? bini->FindMoleculeFeature(ge_id, "speedZ") : NULL;
		  if (par != NULL && par->pcurr != "")
		  {
			  double speedZ = atof(par->pcurr.c_str()); // скорость
			  geob->GetShift(x, y, z);
			  zNew = z + speedZ * dt; // новая z-позиция
			  if (zNew < 0)
			  { // отскок
				  zNew = fabs(zNew * 0.1);
				  speedZ = fabs(speedZ);
			  }
			  speedZ -= g * dt; // новая скорость
			  sprintf_s(buf, "%.6f", speedZ);
			  par->pcurr = buf; // запомнить
			  geob->Translate(x, y, zNew);
			  if(ge_id == 0)
			  cout << ge_id << "," << x << "," << y << ",z=" << z << ",zn=" << zNew << "," << speedZ << "," << dt << endl;
		  }
	  }
	  return 1;

  }


} // namespace Grasp