#pragma once
#include "GJHActorTransform.h"

class GJHRenderer;
class GJHDirectTexture2D;
class GJHRayComponent : public GJHActorTransform
{
private:
	static int DoorLayerIndex;

private:
	static bool m_IsDebugRender;

public:
	static void ToggleDebugRender()
	{
		m_IsDebugRender = !m_IsDebugRender;
	}

	static bool IsDebugRender()
	{
		return m_IsDebugRender;
	}

private:
	std::shared_ptr<GJHRenderer> m_DebugRender;
	float4 m_DebugRenderColor;
	float m_DebugRenderScaleX;
	float m_DebugAngle;
	bool m_Pause;

public:
	GJHRayComponent();
	~GJHRayComponent();

public:
	GJHRayComponent(const GJHRayComponent& _Other) = delete;
	GJHRayComponent(const GJHRayComponent&& _Other) = delete;

public:
	GJHRayComponent operator=(const GJHRayComponent& _Other) = delete;
	GJHRayComponent operator=(const GJHRayComponent&& _Other) = delete;

public:
	void SetLocalPosition(const float4& _Pos);
	void SetColor(const float4& _Color);

public:
	bool CheckActor(const float4& _FindActorPos, const float4& _ThisActorPos, std::shared_ptr<GJHDirectTexture2D> _ColMapTexture, int _Dir, float _Angle, int _Range);
	float4 GetLineInterectionPoint(const float4& _FindActorPosStart, const float4& _FindActorPosEnd, const float4& _ColObjPosStart, const float4& _ColObjPosEnd);

public:
	bool CheckActorColToAngle(const float4& _StartPos, const float4& _ColPos, const float4& _ColScale, float _Angle, int _Dir);

public:
	void Start() override;
	void Update() override;
	void DebugRender() override;

public:
	float GetDebugAngle();
	void ClearmDebugRenderScaleX();
	void PauseDebugRender();
	void ResumeDebugRender();
};