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

string TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

void SplitString(const string & targetString, const string & splitChar, vector<string> & vecStr)
{
	if (splitChar.empty())
	{
		vecStr.emplace_back(targetString);
		return;
	}
	string copy = targetString;
	size_t start = 0;
	size_t npos = copy.find(splitChar);
	if (npos != string::npos)
	{
		do
		{
			vecStr.push_back(move(copy.substr(start, npos - start)));
			start = npos + splitChar.size();
			npos = copy.find(splitChar, start);
		} while (npos != string::npos);
		vecStr.push_back(move(copy.substr(start, copy.size() - start)));
	}
	else
		vecStr.emplace_back(targetString);
}
