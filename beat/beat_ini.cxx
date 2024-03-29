﻿#include "beat_ini.h"
#include "Utils.h"

namespace Grasp {
  BeatIni::BeatIni(GeObWindow* gw)
  {
	  separa = ";";
	  replace_arr[0] = ".";
	  replace_arr[1] = ",";
	  geob_win = gw;
	  geob_win->Reset();
  }

  BeatIni::~BeatIni() 
  {
	  IniUnload();
  }

  void BeatIni::Defaults()
  {
	//defmat.composit = "0";
	//defmat.objname = "air";
    //число датчиков давления. Датчики регистрируют давление, скорость, плотность, температуру, состав смеси. 
	nGadgets = 0;
  }

  // загрузить файл, создать управляющие структуры
  int BeatIni::LoadIni(const char* fname)
  {
	  Defaults();
	  IniUnload();
	  int rc = IniParse(fname);
	  filename = fname;

	  vector<string*> lst;
	  GeOb* obj = NULL;
	  int npos = 0, geom_must = 8, j;
	  geom_type_enum geom_type = geom_type_enum::GO_DEFAULT;

	  // восстанавливаем кубы etc
	  for (j = 0; j < lstKey.size(); j++)
	  {
		  string key = *lstKey[j];
		  string val = *lstVal[j];
		  if (val == "")
			  continue;

		  if (key == "box")
		  {   //box=0,0,0, 15,15,15, 4, 256, 0,0,0, 10, 293,1000000, box1
			  //x,y,z, dx,dy,dz, nSide, clr, features (speed 3, mass, temp, color), name
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_BOX;
		  }
		  else if (key == "cylinder")
		  {
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_CYLINDER;
		  }
		  else if (key == "cone")
		  {
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_TETRA;
		  }
		  else if (key == "sphere")
		  {
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_SPHERE;
		  }
		  else if (key == "line")
		  {
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_LINES;
		  }
		  else if (key == "gadget")
		  {
			  geom_must = 8;
			  geom_type = geom_type_enum::GO_GADGET;
		  }
		  else if (key == "poly")
		  {
			  /*out << "poly=" << x_0 << "," << y_0 << "," << z_0 << ","
				  << dx << "," << dy << "," << dz << "," << geob->fname.c_str();*/
			  geom_must = 7;
			  geom_type = geom_type_enum::GO_POLY;
		  }
		  else continue;

		  string qs = val;
		  Utils::split2vector(qs, ',', lst);

		  if (lst.size() < geom_must)
			  continue;
		  double x_0 = atof(lst[0]->c_str()), 
			  y_0 = atof(lst[1]->c_str()),
			  z_0 = atof(lst[2]->c_str()),
			  x_1 = atof(lst[3]->c_str()),
			  y_1 = atof(lst[4]->c_str()),
			  z_1 = atof(lst[5]->c_str());
		  int nSide = (geom_type != geom_type_enum::GO_POLY ? atoi(lst[6]->c_str()) : 4);
		  int clr = atoi(lst[7]->c_str());
		  string * name = lst[lst.size() - 1];
		  string fin = (geom_type != geom_type_enum::GO_POLY ? "" : lst[6]->c_str());

		  GeOb * obj = geob_win->CreateObj(geom_type, x_0, y_0, z_0, x_1, y_1, z_1, nSide, clr, fin, true);
		  if (name->find("gid_") == 0)
		  {
			  int gid = atoi(name->c_str() + 4);
			  obj->SetIndex(gid);
		  }

		  TMolecule* mol = new TMolecule();
		  mol->geob_id = obj->GetIndex();
		  char buf[33];
		  sprintf_s(buf, "gid_%d", mol->geob_id);
		  mol->objname = buf;
		  InitFeatures(mol);
		  for (int i = 0; i < (int)mol->lstFeature.size(); i++)
		  {
			  if ((int)lst.size() > i + geom_must)
			  {
				  SetFeature(mol, i, *lst[i + geom_must]);
				  /*if (mol->lstFeature[i]->pname == "color")
				  {	// задать цвет
					  unsigned char _red, _green, _blue;
					  int clrInd = atoi(mol->lstFeature[i]->pcurr.c_str());
					  Facet3::GetColorByIndex(clrInd, _red, _green, _blue);
					  obj->SetColor(_red, _green, _blue);
				  }*/
			  }
		  }
		  TMoleculeList.push_back(mol);
	  }
	  Utils::split2vector(NULL, ',', lst); //чистка

	  // загрузить позицию камеры
	  double camera_distance = 9.0, camera_azimut = 0.7, camera_elevation = 0.5;
	  const char* sKey = IniFindValue("camera_distance");
	  if (sKey != NULL) camera_distance = atof(sKey);
	  sKey = IniFindValue("camera_azimut");
	  if (sKey != NULL) camera_azimut = atof(sKey);
	  sKey = IniFindValue("camera_elevation");
	  if (sKey != NULL) camera_elevation = atof(sKey);
	  geob_win->SetPolar(camera_distance, camera_azimut, camera_elevation);

	  return rc;
  }

  int BeatIni::IniParse(const char* fname)
  {
	  wstring data = Utils::readFile(fname);
	  string d8 = Utils::wstring_to_utf8(data);
	  int len = (int)d8.size();
	  char* buf = new char[len + 1];
	  strcpy_s(buf, len + 1, d8.c_str());

	  vector<string*> vec;
	  Utils::split2vector(buf, '\n', vec);
	  int vl = (int)vec.size();
	  delete[] buf;

	  // trim strings
	  for (int k = 0; k < vl; k++)
	  {
		  string * s = vec[k];
		  const char* da = s->c_str();
		  len = (int)s->size();
		  if (len == 0) continue;
		  buf = new char[len + 1];
		  strcpy_s(buf, len + 1, da);

		  char* buf_2 = Utils::trim(buf);
		  if (buf_2[0] == 0 || buf_2[0] == '#')
		  {
			  delete[] buf;
			  continue;
		  }

		  char* cpos = strchr(buf_2, '=');
		  if (cpos != NULL)
		  {	  //key=value
			  *cpos++ = 0;	//break the string
			  char* sKey = Utils::trim(buf_2);
			  char* sVal = Utils::trim(cpos);
			  lstKey.push_back(new string(sKey));
			  lstVal.push_back(new string(sVal));
		  }
		  else if (buf_2[0] == '[')
		  {   //section starts
			  lstKey.push_back(new string(buf_2));
			  lstVal.push_back(new string());
		  }
		  delete[] buf;
	  }

	  for (int k = 0; k < vl; k++)
	  {
		  delete vec[k];
	  }

	  LoadDialogs();

	  return (int)lstKey.size();
  }

  void BeatIni::LoadDialogs()
  {
	  int i;
	  TDialog* dlg = NULL;
	  std::vector<std::string*> vec;
	  for (i = 0; i < lstKey.size(); i++)
	  {
		  string key = *lstKey[i];
		  string val = *lstVal[i];

		  if (key == "separa")
		  {
			  separa = val;
		  }
		  else if (key == "replace_arr_0")
		  {
			  replace_arr[0] = val;
		  }
		  else if (key == "replace_arr_1")
		  {
			  replace_arr[1] = val;
		  }
		  else if (key == "[Controls]")
		  {
			  dlg = NULL;
		  }
		  else if (key == "features")
		  {			  
			  dlg = &features;
			  dlg->dname = val;
		  }
		  else if (key.find("dialog_") == 0)
		  {
			  dlg = new TDialog();
			  dlg->dname = val;
			  lstDlg.push_back(dlg);
		  }
		  else if (dlg != NULL)
		  {
			  int len = (int)val.size();
			  char* buf = new char[len + 1];
			  strcpy_s(buf, len + 1, val.c_str());
			  Utils::split2vector(buf, '|', vec);
			  delete[] buf;
			  if (vec.size() != 3)
			  {				  
				  continue;
			  }
			  //cout << key << " " << *vec[0] << " " << *vec[1] << " " << *vec[2] << "\n"; 

			  TParam* pParam = new TParam();
			  pParam->pname = key;
			  pParam->pvalue = Utils::trim(vec[0]);	//default
			  pParam->pcurr = Utils::trim(vec[0]);	//current

			  pParam->pcomment = Utils::trim(vec[2]);

			  string tt = Utils::trim(vec[1]);
			  if (tt.find("string") == 0)
				  pParam->ptype = 1;
			  //??else if( tt.indexOf("slider") == 0 )
				  //??pParam->ptype = 2;
			  else if (tt.find("combo") == 0)
				  pParam->ptype = 3;
			  else if (tt.find("int") == 0)
				  pParam->ptype = 4;
			  else pParam->ptype = 0;	//double

			  int i0 = (int)tt.find("{");
			  int i1 = (int)tt.find("}");
			  //cout << "i0 " << i0 << ", i1 " << i1;
			  if (i0 < i1)
			  {
				  len = i1 - i0 - 1;
				  buf = new char[len + 2];
				  const char* ptr = tt.c_str() + i0 + 1;
				  strcpy_s(buf, len + 2, ptr);
				  buf[len] = 0;
				  Utils::split2vector(buf, ',', vec);
				  delete[] buf;

				  for (int k = 0; k < vec.size(); k++)
				  {
					  string* qq = new string(vec[k]->c_str());
					  pParam->lstCombo.push_back(qq);
				  }
			  }

			  dlg->lstParams.push_back(pParam);
		  }
	  }
	  Utils::split2vector(NULL, '|', vec); //clean vec
	  //IniUnload();
  }

  // очистка структур
  void BeatIni::IniUnload()
  {
	  for (int k = 0; k < lstKey.size(); k++)
	  {
		  delete lstKey[k];
	  }
	  lstKey.resize(0);

	  for (int k = 0; k < lstVal.size(); k++)
	  {
		  delete lstVal[k];
	  }
	  lstVal.resize(0);

	  for (int k = 0; k < lstDlg.size(); k++)
	  {
		  delete lstDlg[k];
	  }
	  lstDlg.resize(0);

	  for (int k = 0; k < TMoleculeList.size(); k++)
	  {
		  delete TMoleculeList[k];
	  }
	  TMoleculeList.resize(0);

	  features.Reset();
	  defmat.Reset();
	  geob_win->Reset();
	  GeOb::ResetCounter();
  }

  const char* BeatIni::IniFindValue(const char* look)
  {
	  for (int i = 0; i < lstKey.size(); i++)
	  {
		  string key = *lstKey[i];
		  if (key == look)
		  {
			  return lstVal[i]->c_str();
		  }
	  }
	  return NULL;
  }

  const char* BeatIni::IniFindValuePos(const char* look, int& npos)
  {
	  for (int i = npos; i < lstKey.size(); i++)
	  {
		  string key = *lstKey[i];
		  if (key == look)
		  {
			  npos = i;
			  return lstVal[i]->c_str();
		  }
	  }
	  return NULL;
  }

  // проинициализировать физ.объект
  void BeatIni::InitFeatures(TMolecule* mol)
  {
	  if (mol->lstFeature.size() > 0)
		  return;
	  for (int i = 0; i < features.lstParams.size(); i++)
	  {
		  TParam* parCur = features.lstParams[i];
		  TParam* par = new TParam();
		  par->pname = parCur->pname;
		  par->pvalue = parCur->pvalue;
		  par->pcurr = parCur->pvalue; //default
		  par->pcomment = parCur->pcomment;
		  par->ptype = parCur->ptype;
		  for (int k = 0; k < parCur->lstCombo.size(); k++)
		  {
			  string* qq = new string(parCur->lstCombo[k]->c_str());
			  par->lstCombo.push_back(qq);
		  }
		  mol->lstFeature.push_back(par);
	  }
  }

  // установить свойство
  void BeatIni::SetFeature(TMolecule* mol, int index, string& val)
  {
	  mol->lstFeature[index]->pcurr = val;
  }

  // создать новый физ.объект и связать с геом.
  void BeatIni::AddGeOb(GeOb* cub3)
  {
	  TMolecule* mol = new TMolecule();
	  mol->geob_id = cub3->GetIndex();
	  char buf[33];
	  sprintf_s(buf, "gid_%d", mol->geob_id);
	  mol->objname = buf;
	  InitFeatures(mol);
	  TMoleculeList.push_back(mol);
  }

  // найти физ.объект по индексу
  TMolecule* BeatIni::FindMolecule(int geob_id)
  {
	  for (int i = 0; i < TMoleculeList.size(); i++)
	  {
		  TMolecule * mol = TMoleculeList[i];
		  if (mol->geob_id == geob_id)
			  return mol;
	  }
	  return NULL;
  }

  // найти физ.свойство по индексу и ключу
  TParam* BeatIni::FindMoleculeFeature(int geob_id, const char* key)
  {
	  TMolecule* mol = FindMolecule(geob_id);
	  if (mol == NULL) return NULL;
	  TParam* p = mol->FeatureByName(key);
	  return p;
  }

  // сохранить проект в файл
  bool BeatIni::Save(const char* fname)
  {
	  bool rc = true;
	  std::wofstream outFile;
	  outFile.open(fname, std::ios::out);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  outFile.imbue(loc);

	  outFile << "#BEAT saved" << endl;
	  // сохранить свойства
	  outFile << "[Features]" << endl;
	  outFile << "features=" << Utils::utf8_to_wstring(features.dname) << endl;
	  for (int m = 0; m < features.lstParams.size(); m++)
	  { //velocityX=0 |double {-100000,100000} |скорость м/сек по оси X
		  ParameterFull(outFile, features.lstParams[m]);
	  }

	  // сохранить диалоги параметров
	  outFile << endl << "[Parameters]" << endl;
	  for (int m = 0; m < lstDlg.size(); m++)
	  {
		  outFile << "dialog_" << m << "=" << Utils::utf8_to_wstring(lstDlg[m]->dname) << endl;
		  for (int n = 0; n < lstDlg[m]->lstParams.size(); n++)
		  {
			  ParameterFull(outFile, lstDlg[m]->lstParams[n]);
		  }
	  }

	  // сохранить геометрию
	  outFile << endl << "[Geom]" << endl;
	  int len = (int)TMoleculeList.size();
	  nGadgets = 0;
	  for (int m = 0; m < len; m++)
	  {
		  /*if (TMoleculeList[m]->geom_type == GO_GADGET)
		  {	//special case - gadgets
			  if (nGadgets < GAD_MAX)
			  {
				  gadget_x[nGadgets] = TMoleculeList[m]->x_0;
				  gadget_y[nGadgets] = TMoleculeList[m]->y_0;
				  gadget_z[nGadgets] = TMoleculeList[m]->z_0;
				  nGadgets++;
			  }
			  continue;
		  }*/

		  SaveMol(outFile, TMoleculeList[m]);
	  }

	  // сохранить управление
	  outFile << endl << "[Controls]" << endl;
	  int npos = 0;
	  const char * pp = IniFindValuePos("[Controls]", npos);
	  if (pp != NULL)
	  {
		  for (int m = npos + 1; m < lstKey.size(); m++)
		  {
			  if (*lstKey[m] == "camera_distance") continue;
			  if (*lstKey[m] == "camera_azimut") continue;
			  if (*lstKey[m] == "camera_elevation") continue;
			  outFile << Utils::utf8_to_wstring(*lstKey[m]) << "=" << Utils::utf8_to_wstring(*lstVal[m]) << endl;
		  }
	  }
	  // сохранить камеру
	  double camera_distance = 9.0, camera_azimut = 0.7, camera_elevation = 0.5;
	  geob_win->GetPolar(camera_distance, camera_azimut, camera_elevation);
	  outFile << "camera_distance=" << camera_distance << endl;
	  outFile << "camera_azimut=" << camera_azimut << endl;
	  outFile << "camera_elevation=" << camera_elevation << endl;

	  outFile.close();
	  return rc;
  }
  
  // генерировать ini-файл из проекта
  bool BeatIni::GenerateIni(const char* fname)
  {
	  bool rc = true;
	  std::wofstream outFile;
	  outFile.open(fname, std::ios::out);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  outFile.imbue(loc);

	  const char* geom_header = IniFindValue("geom_header");
	  const char* geom_record = IniFindValue("geom_record");
	  const char* geom_features = IniFindValue("geom_features");

	  outFile << "#BEAT generated" << endl;
	  outFile << "#[Parameters]" << endl;
	  for (int m = 0; m < lstDlg.size(); m++)
	  {
		  for (int n = 0; n < lstDlg[m]->lstParams.size(); n++)
		  {
			  outFile << Utils::utf8_to_wstring(lstDlg[m]->lstParams[n]->pname) << "=" <<
				  Utils::utf8_to_wstring(lstDlg[m]->lstParams[n]->pcurr) << endl;
		  }
	  }

	  if(geom_header != NULL )
		outFile << geom_header << endl;
	  if (geom_features != NULL)
	  {
		  outFile << geom_features;// << "=";
		  for (int m = 0; m < features.lstParams.size(); m++)
		  {
			  if (m > 0) outFile << ",";
			  outFile << Utils::utf8_to_wstring(features.lstParams[m]->pname);
		  }
		  outFile << endl;
	  }

	  int len = (int)TMoleculeList.size();
	  nGadgets = 0;
	  for (int m = 0; m < len; m++)
	  {
		  /*if (TMoleculeList[m]->geom_type == GO_GADGET)
		  {	//special case - gadgets
			  if (nGadgets < GAD_MAX)
			  {
				  gadget_x[nGadgets] = TMoleculeList[m]->x_0;
				  gadget_y[nGadgets] = TMoleculeList[m]->y_0;
				  gadget_z[nGadgets] = TMoleculeList[m]->z_0;
				  nGadgets++;
			  }
			  continue;
		  }*/

		  if(geom_record == NULL)
			  SaveMol(outFile, TMoleculeList[m]);
		  else SaveMolEx(outFile, TMoleculeList[m], geom_record);
	  }

	  outFile.close();
	  return rc;
  }

  void BeatIni::SaveMol(std::wofstream& out, TMolecule* mol)
  {
	  GeOb * geob = geob_win->FindObjById(mol->geob_id);
	  if (geob == NULL) return;

	  double x_0, y_0, z_0, dx, dy, dz;
	  geob->GetShift(x_0, y_0, z_0);
	  geob->GetScale(dx, dy, dz);
	  geom_type_enum geom_type = geob->GetGeomType();
	  int nSide = geob->GetNside();
	  unsigned char _red, _green, _blue;
	  geob->GetColor(_red, _green, _blue);
	  int clr = 65536 * _red + 256 * _green + _blue;

	  if (geom_type == geom_type_enum::GO_SPHERE)
	  {
		  out << "sphere=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_BOX)
	  {
		  out << "box=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_CYLINDER)
	  {
		  out << "cylinder=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_TETRA)
	  {
		  out << "cone=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_LINES)
	  {
		  Facet3* f = geob->GetFacet(0);
		  Vec3* v1 = f->GetPoint(0);
		  x_0 = v1->GetX();
		  y_0 = v1->GetY();
		  z_0 = v1->GetZ();
		  v1 = f->GetPoint(1);
		  dx = v1->GetX();
		  dy = v1->GetY();
		  dz = v1->GetZ();
		  out << "line=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_GADGET)
	  {
		  out << "gadget=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << nSide << "," << clr;
	  }
	  else if (geom_type == geom_type_enum::GO_POLY)
	  {
		  out << "poly=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << geob->fname.c_str();
	  }
	  else if (geom_type == geom_type_enum::GO_DEFAULT)
		  out << "default=" << Utils::utf8_to_wstring(mol->composit);
	  else return;

	  for (int i = 0; i < mol->lstFeature.size(); i++)
	  {
		  TParam* par = mol->lstFeature[i];
		  out << "," << Utils::utf8_to_wstring(par->pcurr);
	  }
	  out << "," << Utils::utf8_to_wstring(mol->objname) << endl;
  }

  void BeatIni::SaveMolEx(std::wofstream& out, TMolecule* mol, const char* geom_record)
  {
	  GeOb* geob = geob_win->FindObjById(mol->geob_id);
	  if (geob == NULL) return;

	  double x_0, y_0, z_0, dx, dy, dz;
	  geob->GetShift(x_0, y_0, z_0);
	  geob->GetScale(dx, dy, dz);
	  geom_type_enum geom_type = geob->GetGeomType();
	  int nSide = geob->GetNside();
	  unsigned char _red, _green, _blue;
	  geob->GetColor(_red, _green, _blue);
	  int clr = 65536 * _red + 256 * _green + _blue;

	  char buf[330];
	  std::size_t pos;
	  string res = geom_record, toReplace, replaceWith;

	  toReplace = "{geom_type}";
	  if (geom_type == geom_type_enum::GO_SPHERE) replaceWith = "sphere";
	  else if (geom_type == geom_type_enum::GO_BOX) replaceWith = "box";
	  else if (geom_type == geom_type_enum::GO_CYLINDER) replaceWith = "cylinder";
	  else if (geom_type == geom_type_enum::GO_TETRA) replaceWith = "cone";
	  else if (geom_type == geom_type_enum::GO_LINES) 
	  {
		  Facet3* f = geob->GetFacet(0);
		  Vec3* v1 = f->GetPoint(0);
		  x_0 = v1->GetX();
		  y_0 = v1->GetY();
		  z_0 = v1->GetZ();
		  v1 = f->GetPoint(1);
		  dx = v1->GetX();
		  dy = v1->GetY();
		  dz = v1->GetZ();
		  replaceWith = "line";
	  }
	  else if (geom_type == geom_type_enum::GO_GADGET) replaceWith = "gadget";
	  else if (geom_type == geom_type_enum::GO_POLY) replaceWith = "poly";
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{x}";
	  sprintf_s(buf, "%.3f", x_0);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{y}";
	  sprintf_s(buf, "%.3f", y_0);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{z}";
	  sprintf_s(buf, "%.3f", z_0);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{dx}";
	  sprintf_s(buf, "%.3f", dx);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{dy}";
	  sprintf_s(buf, "%.3f", dy);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{dz}";
	  sprintf_s(buf, "%.3f", dz);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{side}";
	  sprintf_s(buf, "%d", nSide);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{clr}";
	  sprintf_s(buf, "%d", clr);
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{gid}";
	  sprintf_s(buf, "%d", geob->GetIndex());
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  toReplace = "{file}";
	  sprintf_s(buf, "%s", geob->fname.c_str());
	  replaceWith = buf;
	  pos = res.find(toReplace);
	  if (pos != std::string::npos)
		  res.replace(pos, toReplace.length(), replaceWith);

	  for (int i = 0; i < mol->lstFeature.size(); i++)
	  {
		  TParam* par = mol->lstFeature[i];
		  toReplace = "{" + par->pname + "}";
		  replaceWith = par->pcurr;
		  pos = res.find(toReplace);
		  if (pos != std::string::npos)
			  res.replace(pos, toReplace.length(), replaceWith);
	  }
	  out << Utils::utf8_to_wstring(res) << endl;
  }

  void BeatIni::ParameterFull(std::wofstream& outFile, TParam* par)
  {
	  std::stringstream ss;
	  if (par->lstCombo.size() > 0)
	  {
		  ss << " {";
		  for (int k = 0; k < par->lstCombo.size(); k++)
		  {
			  if (k > 0) ss << ",";
			  ss << par->lstCombo[k]->c_str();
		  }
		  ss << "} ";
	  }

	  outFile << Utils::utf8_to_wstring(par->pname) << "=" <<
		  Utils::utf8_to_wstring(par->pcurr) << "|" <<
		  Utils::utf8_to_wstring(par->PtypeToString()) <<
		  Utils::utf8_to_wstring(ss.str()) << "|" <<
		  Utils::utf8_to_wstring(par->pcomment) << endl;
  }

  // генерировать vpt-файл из проекта
  bool BeatIni::GenerateVtp(const char* fname)
  {
	  bool rc = true;
	  std::string qq;
	  std::stringstream ss, ww;
	  std::wofstream outFile;
	  outFile.open(fname, std::ios::out);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  outFile.imbue(loc);

	  int len = (int)TMoleculeList.size();
	  int nPoint = 0, nCell = 0, np;
	  for (int m = 0; m < len; m++)
	  {
		  TMolecule* mol = TMoleculeList[m];
		  if (mol == NULL) continue;
		  GeOb* geob = geob_win->FindObjById(mol->geob_id);
		  if (geob == NULL) continue;

		  for (int i = 0; i < geob->GetSize(); i++) {
			  //цикл по граням
			  Facet3* f = geob->GetFacet(i);
			  if (f == NULL) continue;

			  nCell++;
			  nPoint += f->GetSize();
		  }
	  }

	  // VTK's default Lookup Table is rainbow - from red to blue

	  outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	  outFile << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\" header_type=\"UInt32\" compressor=\"vtkZLibDataCompressor\">" << endl;
	  outFile << "<PolyData>" << endl;
	  outFile << "<Piece NumberOfPoints=\"" << nPoint << 
		  "\" NumberOfVerts=\"0\" NumberOfLines=\"0\" NumberOfStrips=\"0\" NumberOfPolys=\"" << nCell << "\">";

	  outFile << "<Points>" << endl;
	  outFile << "<DataArray type=\"Float32\" Name=\"Points\" NumberOfComponents=\"3\" format=\"ascii\">" << endl;
	  for (int m = 0; m < len; m++)
	  {
		  TMolecule* mol = TMoleculeList[m];
		  if (mol == NULL) continue;
		  GeOb* geob = geob_win->FindObjById(mol->geob_id);
		  if (geob == NULL) continue;

		  for (int i = 0; i < geob->GetSize(); i++) {
			  //цикл по граням
			  Facet3* f = geob->GetFacet(i);
			  if (f == NULL) continue;

			  np = f->GetSize();
			  for (int j = 0; j < np; j++)
			  {
				  Vec3 * v = f->GetPoint(j);
				  outFile << v->GetX() << " " << v->GetY() << " " << v->GetZ() << " " << endl;
			  }
			  outFile << endl;
		  }
	  }
	  outFile << "</DataArray>" << endl;
	  outFile << "</Points>" << endl;

	  outFile << "<Polys>" << endl;
	  outFile << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" << endl;
	  nPoint = 0;
	  ss.clear();
	  ww.clear();
	  for (int m = 0; m < len; m++)
	  {
		  TMolecule* mol = TMoleculeList[m];
		  if (mol == NULL) continue;
		  GeOb* geob = geob_win->FindObjById(mol->geob_id);
		  if (geob == NULL) continue;

		  for (int i = 0; i < geob->GetSize(); i++) {
			  //цикл по граням
			  Facet3* f = geob->GetFacet(i);
			  if (f == NULL) continue;

			  np = f->GetSize();
			  for (int j = 0; j < np; j++)
			  {
				  outFile << nPoint << " ";
				  nPoint++;
			  }
			  outFile << endl;

			  ss << nPoint << " ";
			  if (i % 5 == 4) ss << endl;

			  ww << (nPoint % 255) << " ";
			  if (i % 5 == 4) ww << endl;
		  }
	  }
	  outFile << "</DataArray>" << endl;
	  outFile << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << endl;
	  qq = ss.str();
	  outFile << qq.c_str();
	  outFile << "</DataArray>" << endl;
	  outFile << "</Polys>" << endl;

	  outFile << "<CellData>" << endl;
	  outFile << "<DataArray type=\"Int32\" Name=\"colour\" format=\"ascii\">" << endl;
	  qq = ww.str();
	  outFile << qq.c_str();
	  outFile << "</DataArray>" << endl;
	  outFile << "</CellData>" << endl;

	  outFile << "</Piece>" << endl;
	  outFile << "</PolyData>" << endl;
	  outFile << "</VTKFile>" << endl;
	  outFile.close();
	  return rc;
  }

} // namespace Grasp