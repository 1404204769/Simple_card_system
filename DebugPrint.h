#pragma once
#ifndef _DEBUGPRINT_H_
#define _DEBUGPRINT_H_
#include "atlbase.h"
#include "atlstr.h"

static void OutputDebugPrintf(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(CA2W(strBuffer));
}
#endif //!_DEBUGPRINT_H_
