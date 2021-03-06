#include "CmnFunc.h"

void WriteLog(const char* szContent, ...)
{
#if ((defined DEBUG) || (defined _DEBUG))
	char szResult[LEN_LOG] = { 0 };
	va_list args;
	va_start(args, szContent);
	vsprintf_s(szResult, LEN_LOG, szContent, args);
	va_end(args);
	sprintf_s(szResult, "%s\n", szResult);
	OutputDebugString(szResult);
#endif // DEBUG
}

void OutPutError(const char * szContent, ...)
{
	char szResult[LEN_LOG] = { 0 };
	va_list args;
	va_start(args, szContent);
	vsprintf_s(szResult, LEN_LOG, szContent, args);
	va_end(args);
	sprintf_s(szResult, "%s\n", szResult);
	MessageBox(0, szResult, "Error", 0);
#if ((defined DEBUG) || (defined _DEBUG))
	OutputDebugString(szResult);
#endif // DEBUG
}

string FormatString(vector<string> infoMsgs)
{
	string info;
	
	for (const auto& m : infoMsgs)
	{
		info.push_back('*');
		info += m;
		info.push_back('\n');
	}

	/*if (!info.empty())
	{
		info.pop_back();
	}*/
	return info;
}

wstring String2WString(const std::string & s)
{
	string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}
