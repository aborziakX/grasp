// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C53225E7_8EB9_49E4_8E77_E46F9B1B94E7__INCLUDED_)
#define AFX_STDAFX_H__C53225E7_8EB9_49E4_8E77_E46F9B1B94E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
//STL
#include <string>
#include <vector>
using namespace std;
 
#if defined(LINUX)
#define SEPARATOR "/"
#else
#define SEPARATOR "\\"
#pragma warning (disable: 4786)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C53225E7_8EB9_49E4_8E77_E46F9B1B94E7__INCLUDED_)
