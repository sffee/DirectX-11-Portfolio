#pragma once

#include <vector>
#include <map>
#include "GJHGameEngineString.h"
#include "GJHGameEngineMemory.h"

class GJHGameEngineInput
{
private:
	class GJHGameEngineKey : public SharedPtrThis<GJHGameEngineKey>
	{
	private:
		bool m_Down;
		bool m_Press;
		bool m_Up;
		bool m_Free;

		std::vector<int> m_CheckKey;

		friend GJHGameEngineInput;

	public:
		template<typename ... REST>
		void PushKey(int _CurKey, REST ... _Arg)
		{
			m_CheckKey.push_back(_CurKey);
			PushKey(_Arg...);
		}

		void PushKey()
		{
		}

	private:
		bool KeyCheck();
		void Update();
		void Reset();
	};

private:
	static GJHGameEngineInput Inst;

public:
	static GJHGameEngineInput& GetInst()
	{
		return Inst;
	}

	static bool IsKey(const GJHGameEngineString& _Name);
	static bool Down(const GJHGameEngineString& _Name);
	static bool Up(const GJHGameEngineString& _Name);
	static bool Press(const GJHGameEngineString& _Name);
	static bool Free(const GJHGameEngineString& _Name);

private:
	std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>> m_AllKey;

	typedef std::shared_ptr<GJHGameEngineKey> KeyPtr;

public:
	GJHGameEngineInput();
	~GJHGameEngineInput();
	GJHGameEngineInput(const GJHGameEngineInput& _Other) = delete;
	GJHGameEngineInput(const GJHGameEngineInput&& _Other) = delete;

public:
	GJHGameEngineInput operator=(const GJHGameEngineInput& _Other) = delete;
	GJHGameEngineInput operator=(const GJHGameEngineInput&& _Other) = delete;

private:
	std::shared_ptr<GJHGameEngineKey> FindKey(const GJHGameEngineString& _KeyName);

public:
	template<typename ... REST>
	void CreateKey(const GJHGameEngineString& _KeyName, REST ... _Arg)
	{
		if (FindKey(_KeyName) != nullptr)
		{
			assert("if (FindKey(_KeyName) != nullptr)");
			return;
		}

		std::shared_ptr<GJHGameEngineKey> NewKey = std::make_shared<GJHGameEngineKey>();
		NewKey->Reset();
		NewKey->PushKey(_Arg...);
		m_AllKey.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>>::value_type(_KeyName, NewKey));
	}

	void Update();
};