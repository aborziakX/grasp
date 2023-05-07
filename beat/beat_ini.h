#ifndef BEAT_INI_H
#define BEAT_INI_H

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

using namespace std;

namespace Grasp {

#define INI_MAX 1000
#define GAD_MAX 1000
#define MAXPOINTS 500

/**
@struct   TParam
@brief   параметр ini-файла
*/
struct TParam
{
	string pname;	//name for ini
	string pvalue;	//default value
	string pcurr;	//current value
	int ptype; //type: 0 - double, 1 - string, 2 - slider, 3 - combo, 4 - int
	string pcomment;	//комментарий=label text
	vector<string*> lstCombo;	//list of values for combo

	~TParam()
	{
		for (int m = 0; m < lstCombo.size(); m++)
		{
			delete lstCombo[m];
		}
	}
};

/**
@struct   TDialog
@brief   диалог для выбора значений параметров ini-файла
*/
struct TDialog
{
	string dname;	//dialog title
	vector<TParam*> lstParams;	//list of params

	~TDialog()
	{
		for (int k = 0; k < lstParams.size(); k++)
		{
			delete lstParams[k];
		}
	}
};

enum geom_type_enum { GO_SPHERE = 0, GO_BOX, GO_CYLINDER, GO_TETRAHEDRON, GO_LINES, GO_GADGET, GO_DEFAULT = 1000 };

/**
@struct   TMolecule
@brief   физичиские свойства геом.объекта
*/
struct TMolecule
{
	string objname, composit, fname;
	double* coord;	//points coordinates (for tetrahedron)
	double x_0, y_0, z_0, radius, dx, dy, dz;
	long geom_type;	/* 0 - sphere, 1 - box, 2 -cylinder, 3 -tetrahedron, 5 - lines, 6 - gadget, 1000 - default */
	long uid;	//unique id
	//bounding box
	double x_min, x_max, y_min, y_max, z_min, z_max;
	double radius_init, dx_init, dy_init, dz_init;	//initial values
	vector<TParam*> lstFeature;	//list of features

	TMolecule()
	{
		coord = NULL;
		x_0 = 0.;
		y_0 = 0.;
		z_0 = 0.;
		x_min = x_max = y_min = y_max = z_min = z_max = 0.;
		dx = 1.;
		dy = 1.;
		dz = 1.;
		radius = 1.;
		radius_init = 1.;
		dx_init = 1.;
		dy_init = 1.;
		dz_init = 1.;
		geom_type = 0;
		uid = 0;
	}

	~TMolecule()
	{
		for (int k = 0; k < lstFeature.size(); k++)
		{
			delete lstFeature[k];
		}
		if (coord != NULL) delete coord;
	}
};

/**
@class   beat_ini
@brief   разбор ini-файла
*/
class BeatIni {

public:
  BeatIni();

  virtual ~BeatIni();

  vector<TMolecule*> TMoleculeList;
  TMolecule defmat;

  vector<TDialog*> lstDlg;	///< list of dialogs
  TDialog features;

  vector<string*> lstKey;	///< list of keys
  vector<string*> lstVal;	///< list of values

  char* trim(char* buf);
  string trim(string* buf);
  //void errorLog(const char* sFormat, ...);
  int IniParse(const char* fname);
  void IniUnload();
  const char* IniFindValue(const char* key);
  //char* IniFindValuePos(const char* key, int& npos);
  int LoadIni(const char* fname);
  void Defaults();
  void LoadDialogs();

  void WriteUnicodeUTF8toFile(const char* myFile, const wstring ws[], size_t nSize, bool append = false);
  void ReadUtf8UnicodeFile(const char* filename);

  /**
  считать из UTF-8 файла в Unicode-16 строку
  */
  static std::wstring readFile(const char* filename);
  /**
  сохранить сообщение в "error.log"
  */
  static void ErrorLog(const wstring & ws);

  /**
  преобразовать из UTF-8 в Unicode-16
  */
  static std::wstring utf8_to_wstring(const std::string& str);
  /**
  преобразовать из Unicode-16 в UTF-8
  */
  static std::string wstring_to_utf8(const std::wstring& str);
  /**
  сформировать вектор из указателей на string
  */
  static void split2vector(char* msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart = true);

protected:
private:
  int nGadgets = 0;///< число датчиков давления. Датчики регистрируют давление, скорость, плотность, температуру, состав смеси. Далее должны следовать группы из трех параметров числом nGadgets. В каждую группу входит 
  string separa;
  string replace_arr[2];
};

} // namespace Grasp
#endif