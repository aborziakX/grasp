#include "beat_ini.h"

namespace Grasp {
  BeatIni::BeatIni() 
  {
    lasttime = 0.0;
  }

  BeatIni::~BeatIni() 
  {
    lasttime = 0.0;
  }

  void BeatIni::Defaults()
  {
	//defmat.composit = "0";
	//defmat.objname = "air";
    //число датчиков давления. Датчики регистрируют давление, скорость, плотность, температуру, состав смеси. Далее должны следовать группы из трех параметров числом nGadgets. В каждую группу входит 
	nGadgets = 0;
  }


  int BeatIni::LoadIni(const char* fname)
  {
	  Defaults();
	  IniUnload();
	  int rc = IniParse(fname);
	  if (rc != 0)
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
	  return 0;
  }
  //return a string without leading and tailing spaces
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

  //log the error to "error.log"
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
  }

  using sysclock_t = std::chrono::system_clock;
  // пишем в лог UTF-8 данные
  void BeatIni::ErrorLog(const wstring ws)
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

	  /* OR 
	  std::wstringstream wss;
      wss << wif.rdbuf();
	  */
  }

  int BeatIni::IniParse(const char* fname)
  {
	  FILE* fp;
	  char buf[1256], * cpos, * sKey, * sVal;

	  if ((fp = fopen(fname, "r")) != NULL)
	  {
		  while (!feof(fp))
		  {
			  if (fgets(buf, sizeof(buf), fp) == NULL)
			  {
				  if (ferror(fp))
				  {
					  sprintf(buf, "ParseIni: failed in %s", fname);
					  errorLog(buf);
					  return -1;
				  }
				  break;
			  }
			  if (buf[0] == '#') continue;	//comment line

			  cpos = strchr(buf, '=');
			  if (cpos != NULL)
			  {	//key=value

				  *cpos++ = 0;	//break the string
				  sKey = trim(buf);

				  sVal = trim(cpos);

				  if (ini_total < INI_MAX)
				  {
					  ini_key[ini_total] = new char[strlen(sKey) + 1];
					  strcpy(ini_key[ini_total], sKey);
					  ini_val[ini_total] = new char[strlen(sVal) + 1];
					  strcpy(ini_val[ini_total], sVal);
					  ini_total++;
				  }
			  }
		  }
		  fclose(fp);
		  return 0;
	  }

	  return -1;
  }

  void BeatIni::IniUnload()
  {
	  for (int i = 0; i < ini_total; i++)
	  {
		  delete ini_key[i];
		  delete ini_val[i];
	  }
	  ini_total = 0;
  }

  char* BeatIni::IniFindValue(const char* key)
  {
	  for (int i = 0; i < ini_total; i++)
	  {
		  if (strcmp(key, ini_key[i]) == 0)
			  return ini_val[i];
	  }
	  return NULL;
  }

} // namespace Grasp