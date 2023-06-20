﻿#ifndef UTILS_H
#define UTILS_H

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
@class   beat_ini
@brief   разбор ini-файла
*/
class Utils {

public:
  Utils();

  virtual ~Utils();

  // demo
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

private:
};

} // namespace Grasp
#endif