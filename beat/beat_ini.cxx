#include "beat_ini.h"

#ifndef _WINDOWS

#ifndef errno_t
#define errno_t int
#endif

errno_t strcpy_s(char* dst, size_t size, const char* src)
{
	if (!dst || !src) return EINVAL;
	for (; size > 0; --size)
	{
		if (!(*dst++ = *src++)) return 0;
	}
	return ERANGE;
}
#endif

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
		  else continue;

		  string qs = val;
		  split2vector(qs, ',', lst);

		  if (lst.size() < geom_must)
			  continue;
		  double x_0 = atof(lst[0]->c_str()), 
			  y_0 = atof(lst[1]->c_str()),
			  z_0 = atof(lst[2]->c_str()),
			  x_1 = atof(lst[3]->c_str()),
			  y_1 = atof(lst[4]->c_str()),
			  z_1 = atof(lst[5]->c_str());
		  int nSide = atoi(lst[6]->c_str());
		  int clr = atoi(lst[7]->c_str());
		  string * name = lst[lst.size() - 1];

		  GeOb * obj = geob_win->CreateObj(geom_type, x_0, y_0, z_0, x_1, y_1, z_1, nSide, clr, true);
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
	  split2vector(NULL, ',', lst); //чистка

	  return rc;
  }

  // возвращает строку без пробелов вначале и вконце
  char* BeatIni::trim(char* buf)
  {
	  int iStart = 0, iEnd = (int)strlen(buf) - 1;

	  for (; iStart <= iEnd; iStart++)
	  {
		  char s = buf[iStart];
		  if (s != ' ' && s != '\n' && s != '\r') break;
	  }

	  //skip trailing spaces
	  for (; iStart <= iEnd; iEnd--)
	  {
		  char s = buf[iEnd];
		  if (s != ' ' && s != '\n' && s != '\r') break;
	  }

	  buf[iEnd + 1] = 0;
	  return buf + iStart;
  }

  // возвращает строку без пробелов вначале и вконце
  string BeatIni::trim(string* s)
  {
	  int len = (int)s->size();
	  char* buf = new char[len + 1];
	  strcpy_s(buf, len + 1, s->c_str());
	  char * buf_2 = trim(buf);
	  string s2(buf_2);
	  delete[] buf;
	  return s2;
  }

  /*//log the error to "error.log"
  void BeatIni::errorLog(const char* sFormat, ...)
  {
	  FILE* fp;
	  time_t ltime;
	  va_list lst;
	  va_start(lst, sFormat);

	  printf("[error]");
	  vprintf(sFormat, lst);
	  printf("\n");

	  if ((fp = fopen("error.log", "a")) != NULL)
	  {
		  time(&ltime);
		  fprintf(fp, "\n%s", ctime(&ltime));

		  vfprintf(fp, sFormat, lst);
		  fclose(fp);
	  }
  }*/

  using sysclock_t = std::chrono::system_clock;
  // пишем в лог UTF-8 данные
  void BeatIni::ErrorLog(const wstring & ws)
  {
	  std::wofstream outFile("error.log", std::ios_base::app);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  outFile.imbue(loc);

	  std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

	  char buf[33] = { 0 };
	  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S ", std::localtime(&now));

	  outFile << buf << ws << "\n";
	  outFile.close();
  }
  //demo
  void BeatIni::WriteUnicodeUTF8toFile(const char* myFile, const wstring ws[], size_t nSize, bool append) 
  {
	  std::wofstream outFile;
	  if (append)
		  outFile.open(myFile, std::ios_base::app);
	  else
		  outFile.open(myFile, std::ios::out);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  outFile.imbue(loc);
	  //wstring q(L"Хз ");
	  for (size_t nIndex = 0; nIndex < nSize; ++nIndex)
	  {
		  outFile << ws[nIndex] << "\n";
	  }
	  outFile.close();
  }
  //demo
  void BeatIni::ReadUtf8UnicodeFile(const char* filename)
  {
	  std::ifstream wif(filename);
	  std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	  wif.imbue(loc);
	  string sLine;
	  int nCount = 1;

	  do
	  {
		  getline(wif, sLine);
		  cout << "\nLine line-" << nCount << " is:" << sLine;
		  nCount++;
	  } while (!wif.eof());

	  wif.close();
  }

  // считать из UTF-8 файла в Unicode-16 строку
  std::wstring BeatIni::readFile(const char* filename)
  {
	  std::wifstream wif(filename);
	  wif.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
	  std::wstringstream wss;
	  wss << wif.rdbuf();
	  wif.close();
	  return wss.str();
  }

  // convert UTF-8 string to wstring
  std::wstring BeatIni::utf8_to_wstring(const std::string& str)
  {
#ifdef CP11
	  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	  return myconv.from_bytes(str);
#else
	  if (str.empty()) {
		  return L"";
	  }
	  int len = (int)str.size() + 1;
	  setlocale(LC_CTYPE, "en_US.UTF-8");
	  wchar_t* p = new wchar_t[len];
	  mbstowcs(p, str.c_str(), len);
	  std::wstring w_str(p);
	  delete[] p;
	  return w_str;
#endif
  }

  // convert wstring to UTF-8 string
  std::string BeatIni::wstring_to_utf8(const std::wstring& str)
  {
#ifdef CP11
	  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	  return myconv.to_bytes(str);
#else
	  if (str.empty()) {
		  return "";
	  }
	  int len = (int)str.size() * 4 + 1;
	  setlocale(LC_CTYPE, "en_US.UTF-8");
	  char* p = new char[len];
	  wcstombs(p, str.c_str(), len);
	  std::string str2(p);
	  delete[] p;
	  return str2;
#endif
  }

  // сформировать вектор из указателей на string
  void BeatIni::split2vector(char* msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart)
  {
	  if (bCleanByStart)
	  {
		  for (int k = 0; k < vec.size(); k++)
		  {
			  delete vec[k];
		  }
		  vec.resize(0);
	  }
	  if (msg == NULL || strlen(msg) == 0)
		  return;
	  char ss[2] = { sepa, 0 };
	  char * p = msg, * p_2;
	  while (true)
	  {
		  p_2 = strstr(p, ss);
		  if (p_2 == NULL)
		  {
			  vec.push_back(new std::string(p));
			  break;
		  }
		  else
		  {
			  *p_2 = 0;
			  vec.push_back(new std::string(p));
			  p = p_2 + 1;
		  }
	  }
  }

  void BeatIni::split2vector(string& msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart)
  {
	  int len = (int)msg.size();
	  char* buf = new char[len + 1];
	  strcpy_s(buf, len + 1, msg.c_str());
	  split2vector(buf, sepa, vec, bCleanByStart);
	  delete[] buf;
  }

  int BeatIni::IniParse(const char* fname)
  {
	  wstring data = readFile(fname);
	  string d8 = wstring_to_utf8(data);
	  int len = (int)d8.size();
	  char* buf = new char[len + 1];
	  strcpy_s(buf, len + 1, d8.c_str());

	  vector<string*> vec;
	  split2vector(buf, '\n', vec);
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

		  char* buf_2 = trim(buf);
		  if (buf_2[0] == 0 || buf_2[0] == '#')
		  {
			  delete[] buf;
			  continue;
		  }

		  char* cpos = strchr(buf_2, '=');
		  if (cpos != NULL)
		  {	  //key=value
			  *cpos++ = 0;	//break the string
			  char* sKey = trim(buf_2);
			  char* sVal = trim(cpos);
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
			  split2vector(buf, '|', vec);
			  delete[] buf;
			  if (vec.size() != 3)
			  {				  
				  continue;
			  }
			  //cout << key << " " << *vec[0] << " " << *vec[1] << " " << *vec[2] << "\n"; 

			  TParam* pParam = new TParam();
			  pParam->pname = key;
			  pParam->pvalue = trim(vec[0]);	//default
			  pParam->pcurr = trim(vec[0]);	//current

			  pParam->pcomment = trim(vec[2]);

			  string tt = trim(vec[1]);
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
				  split2vector(buf, ',', vec);
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
	  split2vector(NULL, '|', vec); //clean vec
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
	  outFile << "features=" << utf8_to_wstring(features.dname) << endl;
	  for (int m = 0; m < features.lstParams.size(); m++)
	  { //velocityX=0 |double {-100000,100000} |скорость м/сек по оси X
		  ParameterFull(outFile, features.lstParams[m]);
	  }

	  // сохранить диалоги параметров
	  outFile << endl << "[Parameters]" << endl;
	  for (int m = 0; m < lstDlg.size(); m++)
	  {
		  outFile << "dialog_" << m << "=" << utf8_to_wstring(lstDlg[m]->dname) << endl;
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
			  outFile << utf8_to_wstring(*lstKey[m]) << "=" << utf8_to_wstring(*lstVal[m]) << endl;
		  }
	  }

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
			  outFile << utf8_to_wstring(lstDlg[m]->lstParams[n]->pname) << "=" <<
				  utf8_to_wstring(lstDlg[m]->lstParams[n]->pcurr) << endl;
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
			  outFile << utf8_to_wstring(features.lstParams[m]->pname);
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
	  /*else if (geom_type == GO_POLY)
	  {
		  out << "poly=" << x_0 << "," << y_0 << "," << z_0 << ","
			  << dx << "," << dy << "," << dz << "," << mol->fname << "," << mol->composit;
	  }*/
	  else if (geom_type == geom_type_enum::GO_DEFAULT)
		  out << "default=" << utf8_to_wstring(mol->composit);
	  else return;

	  for (int i = 0; i < mol->lstFeature.size(); i++)
	  {
		  TParam* par = mol->lstFeature[i];
		  out << "," << utf8_to_wstring(par->pcurr);
	  }
	  out << "," << utf8_to_wstring(mol->objname) << endl;
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

	  for (int i = 0; i < mol->lstFeature.size(); i++)
	  {
		  TParam* par = mol->lstFeature[i];
		  toReplace = "{" + par->pname + "}";
		  replaceWith = par->pcurr;
		  pos = res.find(toReplace);
		  if (pos != std::string::npos)
			  res.replace(pos, toReplace.length(), replaceWith);
	  }
	  out << utf8_to_wstring(res) << endl;
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

	  outFile << utf8_to_wstring(par->pname) << "=" <<
		  utf8_to_wstring(par->pcurr) << "|" <<
		  utf8_to_wstring(par->PtypeToString()) <<
		  utf8_to_wstring(ss.str()) << "|" <<
		  utf8_to_wstring(par->pcomment) << endl;
  }

} // namespace Grasp