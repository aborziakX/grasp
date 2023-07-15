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

  /** модифицирует список объектов согласно указанной модели model */
  static int Run(string & model, GeObWindow* gewin, long now);

  /** модифицирует список объектов согласно модели "z-sin" */
  static int RunZSin(GeObWindow* gewin, long now);

  /** модифицирует список объектов согласно модели "scale-sin" */
  static int RunScaleSin(GeObWindow* gewin, long now);

protected:

private:
};

} // namespace Grasp
#endif