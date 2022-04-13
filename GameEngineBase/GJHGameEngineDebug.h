#pragma once
#include <assert.h>
#include <crtdbg.h>
#include "GJHGameEngineString.h"

#define MsgAssert(AMSG) assert(!AMSG)

class GJHGameEngineDebug
{
public:
	static void LeackCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	static void AssertMsg(const char* _Msg)
	{
		MsgAssert(_Msg);
	}

	static void OutPutMsg(const GJHGameEngineString& _Text)
	{
		OutputDebugStringW(_Text.c_str());
		OutputDebugStringW(L"\n");
	}

private:
	GJHGameEngineDebug();
	~GJHGameEngineDebug();
};