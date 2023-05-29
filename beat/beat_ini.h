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
#include <errno.h>

#include "GeObWindow.h"

#ifndef _WINDOWS
#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif

using namespace std;

namespace Grasp {

/**
@struct   TParam
@brief   параметр ini-файла
*/
struct TParam
{
	string pname;	/** название ini */
	string pvalue;	/** значение по умолчанию */
	string pcurr;	/** текущее значение */
	int ptype; /** тип: 0 - double, 1 - string, 2 - slider, 3 - combo, 4 - int */
	string pcomment;	/** комментарий=label text */
	vector<string*> lstCombo;	/** список значений для combo */

	const char* PtypeToString()
	{
		if (ptype == 0) return "double";
		else if (ptype == 1) return "string";
		else if (ptype == 2) return "slider";
		else if (ptype == 3) return "combo";
		return "int";
	}

	void Reset()
	{
		for (int m = 0; m < lstCombo.size(); m++)
		{
			delete lstCombo[m];
		}
		lstCombo.resize(0);
	}
	~TParam()
	{
		Reset();
	}
};

/**
@struct   TDialog
@brief   описание диалога для выбора значений параметров ini-файла
*/
struct TDialog
{
	string dname;	/** заголовок диалога */
	vector<TParam*> lstParams;	/** список параметров */

	void Reset()
	{
		for (int k = 0; k < lstParams.size(); k++)
		{
			delete lstParams[k];
		}
		lstParams.resize(0);
	}
	~TDialog()
	{
		Reset();
	}
};

/**
@struct   TMolecule
@brief   физичиские свойства геом.объекта
*/
struct TMolecule
{
	string objname, composit, fname;
	int geob_id; /** unique id of GeOb */
	vector<TParam*> lstFeature;	/** список свойств */

	TMolecule()
	{
		geob_id = 0;
	}

	static const char* PtypeToString(geom_type_enum geom_type)
	{
		if (geom_type == geom_type_enum::GO_SPHERE) return "sphere";
		else if (geom_type == geom_type_enum::GO_BOX) return "box";
		else if (geom_type == geom_type_enum::GO_CYLINDER) return "cylinder";
		else if (geom_type == geom_type_enum::GO_TETRA) return "tetra";
		else if (geom_type == geom_type_enum::GO_LINES) return "lines";
		else if (geom_type == geom_type_enum::GO_GADGET) return "gadget";
		return "default";
	}

	void Reset()
	{
		for (int k = 0; k < lstFeature.size(); k++)
		{
			delete lstFeature[k];
		}
		lstFeature.resize(0);
	}
	~TMolecule()
	{
		Reset();
	}

	TParam* FeatureByName(const char * pname)
	{
		for (int k = 0; k < lstFeature.size(); k++)
		{
			if (lstFeature[k]->pname == pname) return lstFeature[k];
		}
		return NULL;
	}
};

/**
@class   beat_ini
@brief   разбор ini-файла
*/
class BeatIni {

public:
  BeatIni(GeObWindow * gw);

  virtual ~BeatIni();
  string filename; /** название файла проекта */

  vector<TMolecule*> TMoleculeList;/** список физических объектов */
  TMolecule defmat;

  vector<TDialog*> lstDlg;	/** список диалогов */
  TDialog features; /** диалог для свойств */

  vector<string*> lstKey;	/** список ключей */
  vector<string*> lstVal;	/** список значений */

  //void errorLog(const char* sFormat, ...);
  /** 
  загрузить файл, создать управляющие структуры 
  */
  int LoadIni(const char* fname); 
  /**
  сохранить проект в файл
  */
  bool Save(const char* fname);
  /**
  генерировать ini-файл из проекта
  */
  bool GenerateIni(const char* fname);

  void InitFeatures(TMolecule* mol); /** проинициализировать физ.объект */
  void SetFeature(TMolecule* mol, int index, string& val); /** установить свойство */

  void AddGeOb(GeOb* cub3); /** создать новый физ.объект и связать с геом. */
  TMolecule * FindMolecule(int index); /** найти физ.объект по индексу */

  void WriteUnicodeUTF8toFile(const char* myFile, const wstring ws[], size_t nSize, bool append = false);
  void ReadUtf8UnicodeFile(const char* filename);

  static char* trim(char* buf); /** возвращает строку без пробелов вначале и вконце */
  static string trim(string* buf); /** возвращает строку без пробелов вначале и вконце */

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
  static void split2vector(string& msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart = true);

protected:
	void Defaults();
	void LoadDialogs();
	int IniParse(const char* fname);
	void IniUnload(); /** очистка структур */
	const char* IniFindValue(const char* key);
	const char* IniFindValuePos(const char* key, int& npos);
	void SaveMol(std::wofstream& out, TMolecule* mol);
	void SaveMolEx(std::wofstream& out, TMolecule* mol, const char* geom_record);
	void ParameterFull(std::wofstream& out, TParam* par);

private:
  int nGadgets = 0;/** число датчиков давления. Датчики регистрируют давление, скорость, плотность, температуру, состав смеси.  */
  string separa;
  string replace_arr[2];
  GeObWindow* geob_win = NULL;
};

} // namespace Grasp
#endif