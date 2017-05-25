#pragma once
#include <qstring.h>
#include <string>
#include "LogViewControll.h"

inline QString MakeQString(const std::wstring& data)
{
	return QString::fromStdWString(data);
}

namespace Utility
{
	extern LogViewControll* currentLog;

	inline void Log(const QString& data, int mode)
	{
		if (currentLog != nullptr)
			currentLog->Log(data, mode);
	}
}