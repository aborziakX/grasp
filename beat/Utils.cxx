#include "Utils.h"

#ifndef _WINDOWS
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
  Utils::Utils()
  {
  }

  Utils::~Utils()
  {
  }

  // возвращает строку без пробелов вначале и вконце
  char* Utils::trim(char* buf)
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
  string Utils::trim(string* s)
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
  void Utils::errorLog(const char* sFormat, ...)
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
  void Utils::ErrorLog(const wstring & ws)
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
  void Utils::WriteUnicodeUTF8toFile(const char* myFile, const wstring ws[], size_t nSize, bool append)
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
  void Utils::ReadUtf8UnicodeFile(const char* filename)
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
  std::wstring Utils::readFile(const char* filename)
  {
	  std::wifstream wif(filename);
	  wif.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
	  std::wstringstream wss;
	  wss << wif.rdbuf();
	  wif.close();
	  return wss.str();
  }

  // convert UTF-8 string to wstring
  std::wstring Utils::utf8_to_wstring(const std::string& str)
  {
#ifdef CP11
	  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv; // конвертер
	  return myconv.from_bytes(str);
#else
	  if (str.empty()) {
		  return L"";
	  }
	  int len = (int)str.size() + 1; // найти длину строки
	  setlocale(LC_CTYPE, "en_US.UTF-8"); // настроить локаль под UTF-8
	  wchar_t* p = new wchar_t[len]; // выделить память для широкой строки
	  mbstowcs(p, str.c_str(), len); // преобразовать из UTF-8
	  std::wstring w_str(p); // создать строку в стеке из указателя
	  delete[] p; // очистка памяти
	  return w_str; // вернуть широкую
#endif
  }

  // convert wstring to UTF-8 string
  std::string Utils::wstring_to_utf8(const std::wstring& str)
  {
#ifdef CP11
	  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv; // конвертер
	  return myconv.to_bytes(str);
#else
	  if (str.empty()) {
		  return "";
	  }
	  int len = (int)str.size() * 4 + 1; // найти длину строки с запасом
	  setlocale(LC_CTYPE, "en_US.UTF-8"); // настроить локаль под UTF-8
	  char* p = new char[len]; // выделить память для строки
	  wcstombs(p, str.c_str(), len); // преобразовать из Unicode-16
	  std::string str2(p); // создать строку в стеке из указателя
	  delete[] p; // очистка памяти
	  return str2; // вернуть строку
#endif
  }

  // из char* msg, разделителя char sepa, сформировать vec - вектор из указателей на string
  // данные в msg помогуть быть изменены!
  void Utils::split2vector(char* msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart)
  {
	  if (bCleanByStart)
	  { // очистить ранее созданный вектор из указателей на строки
		  for (int k = 0; k < vec.size(); k++)
		  {
			  delete vec[k];
		  }
		  vec.resize(0);
	  }
	  if (msg == NULL || strlen(msg) == 0)
		  return;
	  char ss[2] = { sepa, 0 }; // конструкция для поиска разделителя
	  char * p = msg, * p_2;
	  while (true)
	  {
		  p_2 = strstr(p, ss); // найти разделитель
		  if (p_2 == NULL)
		  {	// не найден
			  vec.push_back(new std::string(p));
			  break;
		  }
		  else
		  {
			  *p_2 = 0; // заменить разделить на 0
			  vec.push_back(new std::string(p));
			  p = p_2 + 1;
		  }
	  }
  }

  void Utils::split2vector(string& msg, char sepa, std::vector<std::string*>& vec, bool bCleanByStart)
  {
	  int len = (int)msg.size();
	  char* buf = new char[len + 1];
	  strcpy_s(buf, len + 1, msg.c_str());
	  split2vector(buf, sepa, vec, bCleanByStart);
	  delete[] buf;
  }

  // количество миллисекунд, прошедших с 1 января 1970 года 00:00:00 по UTC
  unsigned long Utils::getTime()
  {
	  namespace sc = std::chrono;
	  auto time = sc::system_clock::now(); // текущее время
	  auto since_epoch = time.time_since_epoch(); // с начала эпохи
	  // I don't know what system_clock returns
	  // I think it's uint64_t nanoseconds since epoch
	  // Either way this duration_cast will do the right thing
	  auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
	  unsigned long now = millis.count(); // just like java (new Date()).getTime(); //является количеством миллисекунд, прошедших с 1 января 1970 года 00:00:00 по UTC
	  return now;
  }
} // namespace Grasp