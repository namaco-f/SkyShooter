#pragma once

#include <Dxlib.h>
#include <string>
#include <functional>
#include "../Common/Vector2/Vector2f.h"
#include "../Common/Vector2/Vector2.h"

/// <summary>
/// デバッグ用
/// </summary>
class DebugUtility
{
public:
	
	union Printable
	{
		int i;
		float f;
		char c;
		char* s;
	};

	struct Test
	{
		int Type;
		Printable Printable;
	};

	//デバッグ表記初期カラー
	static constexpr unsigned int DEF_DEBUG_STRING_COLOR = 0xffffff;

	DebugUtility(void);
	virtual ~DebugUtility(void);

	void DebugStringFormat(
		const int x, const TCHAR* str, unsigned int col);

	void DebugStringFormat(
		const int x,const TCHAR* str, unsigned int col,int val);
	
	void DebugStringFormat(
		const int x,const TCHAR* str, unsigned int col, float val);
	
	void DebugStringFormat(
		const int x, const TCHAR* str, unsigned int col,const Vector2& val);
	
	void DebugStringFormat(
		const int x, const TCHAR* str, unsigned int col, const Vector2f& val);
	
	void DebugStringFormat(
		const int x, const TCHAR* str, unsigned int col ,const VECTOR& val);

	const int GetDebugStringCnt(void) const;

	void DebugStringCntReset(void);

private:
	int debugStringCnt_;



};

