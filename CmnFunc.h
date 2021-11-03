#ifndef __COMMONFUNC_H__
#define __COMMONFUNC_H__

#pragma once

#define LEN_LOG 2048

#include "windows.h"
#include <string>
#include "dxerr.h"

void WriteLog(const char* szContent, ...);

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                               \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, #x, true);  \
		}                                                       \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#endif // !__COMMONFUNC_H__
