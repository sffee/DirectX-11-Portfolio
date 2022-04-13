#pragma once
#include <map>
#include <GJHGameEngineString.h>
#include <memory>
#include "GJHResBase.h"
#include <GJHGameEngineDebug.h>

template<typename T>
class GJHDirectResourcesManager
{
private:
	static std::map<GJHGameEngineString, std::shared_ptr<T>> m_ResMap;

public:
	GJHDirectResourcesManager() { }
	~GJHDirectResourcesManager() { }
	GJHDirectResourcesManager(const GJHDirectResourcesManager& _Other) = delete;
	GJHDirectResourcesManager(const GJHDirectResourcesManager&& _Other) = delete;

public:
	GJHDirectResourcesManager operator=(const GJHDirectResourcesManager& _Other) = delete;
	GJHDirectResourcesManager operator=(const GJHDirectResourcesManager&& _Other) = delete;

public:
	static std::shared_ptr<T> Find(const GJHGameEngineString& _Name)
	{
		if (m_ResMap.find(_Name) == m_ResMap.end())
		{
			return nullptr;
		}

		return m_ResMap[_Name];
	}

	template <typename ConvertType>
	static std::shared_ptr<ConvertType> FindAndCasting(const GJHGameEngineString& _Name)
	{
		return std::dynamic_pointer_cast<ConvertType, GJHResBase>(Find(_Name));
	}

protected:
	static std::shared_ptr<T> InsertRes(const GJHGameEngineString& _Name, T* _Res)
	{
		if (m_ResMap.find(_Name) != m_ResMap.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_ResMap.find(_Name) != m_ResMap.end())");
		}

		std::shared_ptr<T> ResSharedPtr(_Res);
		m_ResMap[_Name] = ResSharedPtr;

		return ResSharedPtr;
	}
};

template<typename T>
std::map<GJHGameEngineString, std::shared_ptr<T>> GJHDirectResourcesManager<T>::m_ResMap;