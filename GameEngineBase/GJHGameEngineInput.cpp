#include "BasePre.h"
#include "GJHGameEngineInput.h"

GJHGameEngineInput GJHGameEngineInput::Inst;

bool GJHGameEngineInput::GJHGameEngineKey::KeyCheck()
{
	for (size_t i = 0; i < m_CheckKey.size(); i++)
	{
		if (GetAsyncKeyState(m_CheckKey[i]) == 0)
		{
			return false;
		}
	}

	return true;
}

void GJHGameEngineInput::GJHGameEngineKey::Update()
{
    if (KeyCheck() == true)
    {
        if (m_Press == false)
        {
            m_Down = true;
            m_Press = true;
            m_Up = false;
            m_Free = false;
        }
        else
        {
            m_Down = false;
            m_Press = true;
            m_Up = false;
            m_Free = false;
        }
    }
    else
    {
        if (m_Press == true)
        {
            m_Down = false;
            m_Press = false;
            m_Up = true;
            m_Free = false;
        }
        else
        {
            m_Down = false;
            m_Press = false;
            m_Up = false;
            m_Free = true;
        }

    }
}

void GJHGameEngineInput::GJHGameEngineKey::Reset()
{
	m_Down = false;
	m_Press = false;
	m_Up = false;
	m_Free = true;
}

bool GJHGameEngineInput::IsKey(const GJHGameEngineString& _Name)
{
    if (GetInst().FindKey(_Name) != nullptr)
    {
        return true;
    }

    return false;
}

bool GJHGameEngineInput::Down(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Down;
}

bool GJHGameEngineInput::Up(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Up;
}

bool GJHGameEngineInput::Press(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Press;
}

bool GJHGameEngineInput::Free(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Free;
}

GJHGameEngineInput::GJHGameEngineInput()
{

}

GJHGameEngineInput::~GJHGameEngineInput()
{

}

std::shared_ptr<GJHGameEngineInput::GJHGameEngineKey> GJHGameEngineInput::FindKey(const GJHGameEngineString& _KeyName)
{
	std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>>::iterator FindIter = m_AllKey.find(_KeyName);

	if (FindIter == m_AllKey.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GJHGameEngineInput::Update()
{
    for (std::pair<GJHGameEngineString, KeyPtr>&& _Key : m_AllKey)
    {
        _Key.second->Update();
    }
}