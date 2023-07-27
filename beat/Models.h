#ifndef MODELS_H
#define MODELS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstdarg>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <chrono>
#include <cstring>
#include <errno.h>

#include "GeObWindow.h"
#include "Utils.h"
#include "beat_ini.h"

using namespace std;

namespace Grasp {

/**
@class   Models
@brief   простые модели
*/
class Models {

public:
  Models();

  virtual ~Models();

  static unsigned long prevtime; /** количество миллисекунд, static для простоты */

  /** модифицирует список объектов согласно указанной модели model */
  static int Run(string & model, GeObWindow* gewin, unsigned long now, BeatIni * bini);

  /** модифицирует список объектов согласно модели "z-sin" */
  static int RunZSin(GeObWindow* gewin, unsigned long now, BeatIni* bini);

  /** модифицирует список объектов согласно модели "scale-sin" */
  static int RunScaleSin(GeObWindow* gewin, unsigned long now, BeatIni* bini);

  /** модифицирует список объектов согласно модели "gravity" */
  static int RunGravity(GeObWindow* gewin, unsigned long now, BeatIni* bini);

protected:

private:
};

} // namespace Grasp
#endif