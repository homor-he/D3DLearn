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