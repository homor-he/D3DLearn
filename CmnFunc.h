#ifndef __COMMONFUNC_H__
#define __COMMONFUNC_H__

#pragma once

#define LEN_LOG 2048

#include "windows.h"
#include <string>
#include <vector>
#include "dxerr.h"

void WriteLog(const char* szContent, ...);

void OutPutError(const char* szContent, ...);

string FormatString(vector<string> infoMsgs);

#endif // !__COMMONFUNC_H__
