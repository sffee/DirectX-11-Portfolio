#pragma once
#include "GJHDirectShader.h"
#include "GJHDirectResourcesManager.h"
#include <GJHGameEnginePath.h>

class GJHDirectPixelShader : public GJHDirectShader, public GJHDirectResourcesManager<GJHDirectPixelShader>
{
public:
	static bool Create(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh = 5, int _VersionLow = 0)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		
		GJHDirectPixelShader* NewRes = new GJHDirectPixelShader();

		if (NewRes->CreateShader(_Path, _FunctionName, _VersionHigh, _VersionLow) == false)
		{
			return false;
		}

		InsertRes(FileName, NewRes);

		return true;
	}

private:
	ID3D11PixelShader* m_ShaderPtr;

public:
	GJHDirectPixelShader();
	~GJHDirectPixelShader();

public:
	GJHDirectPixelShader(const GJHDirectPixelShader& _Other) = delete;
	GJHDirectPixelShader(const GJHDirectPixelShader&& _Other) = delete;

public:
	GJHDirectPixelShader operator=(const GJHDirectPixelShader& _Other) = delete;
	GJHDirectPixelShader operator=(const GJHDirectPixelShader&& _Other) = delete;

public:
	bool CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow);
	void Setting() override;
};