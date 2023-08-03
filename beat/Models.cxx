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
	  int rc = 0;
	  if (model == "z-sin") rc = RunZSin(gewin, now, bini);
	  else if (model == "scale-sin") rc = RunScaleSin(gewin, now, bini);
	  else if (model == "gravity") rc = RunGravity(gewin, now, bini);
	  prevtime = now;
	  return rc; 
  }

  // модифицирует список объектов согласно модели "z-sin"
  int Models::RunZSin(GeObWindow* gewin, unsigned long now, BeatIni* bini)
  {
	  // цикл за 10 секунд = 10000 ms
	  int cycleMSec = 10000;
	  double cur = ((now % cycleMSec) * MY_2PI) / cycleMSec;
	  double x, y, z;

	  int sz = gewin->GetSize();
	  for (int i = 0; i < sz; i++)
	  {
		  GeOb* geob = gewin->GetObj(i);
		  if (geob == NULL) continue;

		  geob->GetShift(x, y, z);
		  z = sin(cur) * 3;
		  geob->Translate(x, y, z);

		  /*std::wstringstream ss;
		  unsigned long dif = now - prevtime;
		  ss << now << ", dif=" << dif << ", cur=" << cur << ", z=" << z;
		  Utils::ErrorLog(ss.str());*/
	  }
	  return 1;
  }

  // модифицирует список объектов согласно модели "scale-sin"
  int Models::RunScaleSin(GeObWindow* gewin, unsigned long now, BeatIni* bini)
  {
	  // цикл за 10 секунд = 10000 ms
	  int cycleMSec = 10000;
	  double cur = ((now % cycleMSec) * MY_2PI) / cycleMSec;
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
	  double dt = ((now - prevtime) / 1000.0); // шаг времени в сек.
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
			  if (zNew <= 0)
			  { // отскок
				  zNew = fabs(zNew);
				  speedZ = fabs(speedZ);
			  }
			  speedZ -= g * dt; // новая скорость
			  sprintf_s(buf, "%.6f", speedZ);
			  par->pcurr = buf; // запомнить
			  geob->Translate(x, y, zNew);
			  /*if (ge_id == 0)
			  {
				  std::wstringstream ss;
				  unsigned long dif = now - prevtime;
				  ss << now << ", dif=" << dif << ", ge_id=" << ge_id << ",x=" << x << ",y=" << y << ",z=" << z << ",zn=" << zNew << ",speedZ" << speedZ << ",dt" << dt;
				  Utils::ErrorLog(ss.str());
			  }*/
		  }
	  }
	  return 1;

  }


} // namespace Grasp