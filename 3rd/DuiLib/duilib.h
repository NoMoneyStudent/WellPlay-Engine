#pragma once

#pragma warning(disable:4091) //���������������ʲôʲô�ġ�����

#include "UIlib.h"

using namespace DuiLib;
using namespace std;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_d.lib")
#   else
#       pragma comment(lib, "DuiLibA_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib.lib")
#   else
#       pragma comment(lib, "DuiLibA.lib")
#   endif
#endif