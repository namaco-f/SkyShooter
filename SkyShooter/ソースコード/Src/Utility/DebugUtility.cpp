

#include <stdio.h>
#include <stdarg.h>
#include "../Application.h"
#include "DebugUtility.h"

DebugUtility::DebugUtility(void)
{
	debugStringCnt_ = 0;
}

DebugUtility::~DebugUtility(void)
{
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col)
{
	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col, str
	);
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col, int val)
{

	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col,str,val
	);
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col, float val)
{
	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col, str, val
	);
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col, const Vector2& val)
{
	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col, str, val.x, val.y
	);
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col, const Vector2f& val)
{
	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col, str, val.x, val.y
	);
}

void DebugUtility::DebugStringFormat(const int x, const TCHAR* str, unsigned int col, const VECTOR& val)
{
	DrawFormatString(
		x, Application::DEBUG_STR_DIFF_Y_INT * debugStringCnt_++,
		col, str, val.x, val.y,val.z
	);
}

const int DebugUtility::GetDebugStringCnt(void) const
{
	return debugStringCnt_;
}

void DebugUtility::DebugStringCntReset(void)
{
	debugStringCnt_ = 0;
}
