﻿#include "beat_ini.h"

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
    //число датчиков давления. Датчики регистрируют давление, скорость, плотность, температуру, состав смеси. Далее должны следовать группы из трех параметров числом nGadgets. В каждую группу входит 
	nGadgets = 0;
  }

  //загрузить файл, создать управляющие структуры
  int BeatIni::LoadIni(const char* fname)
  {
	  Defaults();
	  IniUnload();
	  int rc = IniParse(fname);
	  return rc;
	  /*char buf[330];
	  char* val = IniFindValue("pProblemName");
	  if (val != NULL) pProblemName = val;
	  val = IniFindValue("NX_SHOCK");
	  if (val != NULL) NX_SHOCK = atoi(val);
	  val = IniFindValue("NY_SHOCK");
	  if (val != NULL) NY_SHOCK = atoi(val);
	  val = IniFindValue("NZ_SHOCK");
	  if (val != NULL) NZ_SHOCK = atoi(val);

	  val = IniFindValue("nCheckFiles");
	  if (val != NULL) nCheckFiles = atoi(val);
	  val = IniFindValue("nBoundary_0");
	  if (val != NULL) nBoundary[0] = atoi(val);
	  val = IniFindValue("nBoundary_1");
	  if (val != NULL) nBoundary[1] = atoi(val);
	  val = IniFindValue("nBoundary_2");
	  if (val != NULL) nBoundary[2] = atoi(val);
	  val = IniFindValue("nBoundary_3");
	  if (val != NULL) nBoundary[3] = atoi(val);
	  val = IniFindValue("nBoundary_4");
	  if (val != NULL) nBoundary[4] = atoi(val);
	  val = IniFindValue("nBoundary_5");
	  if (val != NULL) nBoundary[5] = atoi(val);

	  val = IniFindValue("nNXProcessors");
	  if (val != NULL) nNXProcessors = atoi(val);
	  val = IniFindValue("nNYProcessors");
	  if (val != NULL) nNYProcessors = atoi(val);
	  val = IniFindValue("nNZProcessors");
	  if (val != NULL) nNZProcessors = atoi(val);

	  val = IniFindValue("x_0");
	  if (val != NULL) m_x_0 = atoi(val);
	  val = IniFindValue("y_0");
	  if (val != NULL) m_y_0 = atoi(val);
	  val = IniFindValue("z_0");
	  if (val != NULL) m_z_0 = atoi(val);
	  val = IniFindValue("x_1");
	  if (val != NULL) m_x_1 = atoi(val);
	  val = IniFindValue("y_1");
	  if (val != NULL) m_y_1 = atoi(val);
	  val = IniFindValue("z_1");
	  if (val != NULL) m_z_1 = atoi(val);

	  val = IniFindValue("NS");
	  if (val != NULL) NS = atoi(val);

	  for (int i = 0; i < NS; i++)
	  {
		  //material_0=air.mat
		  sprintf(buf, "material_%d", i);
		  val = IniFindValue(buf);
		  if (val != NULL) mat_name[i] = val;
	  }

	  val = IniFindValue("nGadgets");
	  if (val != NULL) nGadgets = atoi(val);
	  for (int i = 0; i < nGadgets; i++)
	  {
		  //gadgetX_0=0
		  double x_0 = 0, y_0 = 0, z_0 = 0;
		  sprintf(buf, "gadgetX_%d", i);
		  val = IniFindValue(buf);
		  if (val != NULL) x_0 = atof(val);

		  sprintf(buf, "gadgetY_%d", i);
		  val = IniFindValue(buf);
		  if (val != NULL) y_0 = atof(val);

		  sprintf(buf, "gadgetZ_%d", i);
		  val = IniFindValue(buf);
		  if (val != NULL) z_0 = atof(val);

		  sprintf(buf, "gadget_%d", i);
		  QString qs = buf;
		  pApp->AddGadX(qs, x_0, y_0, z_0);
	  }

	  int npos = 0, geom_must = 5;
	  while (npos >= 0)
	  {
		  //sphere=0,0,15.2,15.2,0,1e+06,294,2,0,0,s1
		  val = IniFindValuePos("sphere", npos);
		  qDebug() << val;
		  if (val == NULL)
			  break;

		  npos++;
		  QString qs = val;

		  QStringList lst = qs.split(',');
		  if (lst.count() < geom_must)
			  continue;
		  double x_0 = lst[0].toDouble(), y_0 = lst[1].toDouble(), z_0 = lst[2].toDouble(), radius = lst[3].toDouble();
		  QString composit = lst[4];

		  pApp->AddSphX(lst[lst.count() - 1], x_0, y_0, z_0, radius, composit);

		  TMolecule* mol = TMoleculeList[TMoleculeList.length() - 1];
		  for (int i = 0; i < mol->lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(mol, i, lst[i + geom_must]);
		  }
	  }

	  npos = 0;
	  geom_must = 7;
	  while (npos >= 0)
	  {
		  //box=0,0,0,15,15,15,0,293,1000000,0,0,0,box1
		  val = IniFindValuePos("box", npos);
		  qDebug() << val;
		  if (val == NULL)
			  break;

		  npos++;
		  QString qs = val;

		  QStringList lst = qs.split(',');
		  if (lst.count() < geom_must)
			  continue;
		  double x_0 = lst[0].toDouble(), y_0 = lst[1].toDouble(), z_0 = lst[2].toDouble(),
			  x_1 = lst[3].toDouble(), y_1 = lst[4].toDouble(), z_1 = lst[5].toDouble();
		  QString composit = lst[6];

		  pApp->AddBoxX(lst[lst.count() - 1], x_0, y_0, z_0, x_1, y_1, z_1, composit);

		  TMolecule* mol = TMoleculeList[TMoleculeList.length() - 1];
		  for (int i = 0; i < mol->lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(mol, i, lst[i + geom_must]);
		  }
	  }

	  npos = 0;
	  geom_must = 7;
	  while (npos >= 0)
	  {
		  //cylinder=20,20,20,5,30,1,0,293,100000,0,0,0,cyl1
		  val = IniFindValuePos("cylinder", npos);
		  qDebug() << val;
		  if (val == NULL)
			  break;

		  npos++;
		  QString qs = val;

		  QStringList lst = qs.split(',');
		  if (lst.count() < geom_must)
			  continue;
		  double x_0 = lst[0].toDouble(), y_0 = lst[1].toDouble(), z_0 = lst[2].toDouble(), radius = lst[3].toDouble(),
			  height = lst[4].toDouble();
		  int axe = lst[5].toInt();
		  QString composit = lst[6];

		  pApp->AddCylX(lst[lst.count() - 1], x_0, y_0, z_0, radius, height, axe, composit);

		  TMolecule* mol = TMoleculeList[TMoleculeList.length() - 1];
		  for (int i = 0; i < mol->lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(mol, i, lst[i + geom_must]);
		  }
	  }

	  npos = 0;
	  geom_must = 13;
	  while (npos >= 0)
	  {
		  //tetra=20,20,20,5,30,1,0,293,100000,0,0,0,cyl1
		  val = IniFindValuePos("tetra", npos);
		  qDebug() << val;
		  if (val == NULL)
			  break;

		  npos++;
		  QString qs = val;

		  QStringList lst = qs.split(',');
		  if (lst.count() < geom_must)
			  continue;

		  double coord[12];
		  for (int k = 0; k < 12; k++)
			  coord[k] = lst[k].toDouble();
		  QString composit = lst[12];

		  pApp->AddTetraX(lst[lst.count() - 1], coord, composit);

		  TMolecule* mol = TMoleculeList[TMoleculeList.length() - 1];
		  for (int i = 0; i < mol->lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(mol, i, lst[i + geom_must]);
		  }
	  }

	  npos = 0;
	  geom_must = 8;
	  while (npos >= 0)
	  {
		  //poly=0,0,0,1,1,1,C:/C_devel/science/vtk/Beat/build/ach_cyl.vtp,-1,100000,293,0,0,0,poly0
		  val = IniFindValuePos("poly", npos);
		  qDebug() << val;
		  if (val == NULL)
			  break;

		  npos++;
		  QString qs = val;

		  QStringList lst = qs.split(',');
		  if (lst.count() < geom_must)
			  continue;
		  double x_0 = lst[0].toDouble(), y_0 = lst[1].toDouble(), z_0 = lst[2].toDouble(),
			  x_1 = lst[3].toDouble(), y_1 = lst[4].toDouble(), z_1 = lst[5].toDouble();
		  QString fname = lst[6];
		  QString composit = lst[7];

		  pApp->AddPolyX(lst[lst.count() - 1], x_0, y_0, z_0, x_1, y_1, z_1, fname, composit);

		  TMolecule* mol = TMoleculeList[TMoleculeList.length() - 1];
		  for (int i = 0; i < mol->lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(mol, i, lst[i + geom_must]);
		  }
	  }

	  geom_must = 1;
	  val = IniFindValue("default");
	  if (val != NULL)
	  {
		  //default=0,293,0,0,0,media
		  QString qs = val;
		  QStringList lst = qs.split(',');
		  defmat.composit = lst[0] != "" ? lst[0] : "0";
		  defmat.objname = lst[lst.count() - 1];
		  for (int i = 0; i < defmat.lstFeature.count(); i++)
		  {
			  if (lst.count() > i + geom_must)
				  SetFeature(&defmat, i, lst[i + geom_must]);
		  }
	  }

	  //set TDialog params
	  for (int m = 0; m < lstDlg.count(); m++)
	  {
		  TDialog* dlg = lstDlg[m];
		  for (int k = 0; k < lstDlg[m]->lstParams.count(); k++)
		  {
			  TParam* par = dlg->lstParams[k];
			  val = IniFindValue(par->pname.toAscii());
			  if (val != NULL) par->pcurr = val;
			  else par->pcurr = par->pvalue;
		  }
	  }*/
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

  //считать из UTF-8 файла в Unicode-16 строку
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

  //сформировать вектор из указателей на string
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

  //проинициализировать физ.объект
  void BeatIni::InitFeatures(TMolecule* mol)
  {
	  if (mol->lstFeature.size() > 0)
		  return;
	  for (int i = 0; i < features.lstParams.size(); i++)
	  {
		  TParam* par = new TParam();
		  par->pname = features.lstParams[i]->pname;
		  par->pvalue = features.lstParams[i]->pvalue;
		  par->pcurr = features.lstParams[i]->pvalue; //default
		  par->pcomment = features.lstParams[i]->pcomment;
		  mol->lstFeature.push_back(par);
	  }
  }

  //установить свойство
  void BeatIni::SetFeature(TMolecule* mol, int index, string& val)
  {
	  mol->lstFeature[index]->pcurr = val;
  }

  //создать новый физ.объект и связать с геом.
  void BeatIni::AddGeOb(GeOb* cub3)
  {
	  TMolecule* mol = new TMolecule();
	  mol->geob_id = cub3->GetIndex();
	  char buf[33];
	  sprintf_s(buf, "phy %d", mol->geob_id);
	  mol->objname = buf;
	  InitFeatures(mol);
	  TMoleculeList.push_back(mol);
  }

  // найти физ.объект по индексу
  TMolecule* BeatIni::FindMolecule(int index)
  {
	  for (int i = 0; i < TMoleculeList.size(); i++)
	  {
		  TMolecule * mol = TMoleculeList[i];
		  if (mol->geob_id == index)
			  return mol;
	  }
	  return NULL;
  }



} // namespace Grasp