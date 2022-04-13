#pragma once
#include <GJHGameEngineComponent.h>
#include <stack>
#include <functional>

class GJHCamera;
class GJHActorTransform;
class GJHRecordComponent : public GJHGameEngineComponent
{
public:
	struct FrameInfo
	{
		float Time;

		float4 ActorPos;
		float4 ActorScale;
		float4 RenderPos;
		float4 RenderScale;
		float4 RenderRotation;
		float RenderAlpha;

		GJHGameEngineString AnimationName;
		int AnimationFrameIndex;
		bool ShowAnimation;
		bool RenderEnable;

		bool UseCamera;
		float4 CamPos;

		FrameInfo() :
			UseCamera(false)
		{
		}
	};

public:
	static float m_RewindEndTime;
	static bool m_RewindOn;
	static float m_RewindSpeed;

public:
	static std::shared_ptr<GJHActorTransform> m_CamTrans;

	static void SetCamTrans(std::shared_ptr<GJHActorTransform> _CamTrans)
	{
		m_CamTrans = _CamTrans;
	}

public:
	static void RewindStart(float _EndTime)
	{
		m_RewindOn = true;
		m_RewindEndTime = _EndTime;
	}

	static void RewindEnd()
	{
		m_RewindOn = false;
	}

	static void SubRewindEndTime(float _Time)
	{
		m_RewindEndTime -= _Time;
	}
	
public:
	bool m_RewindStart;

private:
	float m_RecordCoolTime;
	std::stack<FrameInfo> m_FrameInfoStack;

private:
	std::function<void()> m_RewindEndFunc;

public:
	void SetRewindEndFunc(std::function<void()> _Func)
	{
		m_RewindEndFunc = _Func;
	}

public:
	GJHRecordComponent();
	~GJHRecordComponent();

public:
	GJHRecordComponent(const GJHRecordComponent& _Other) = delete;
	GJHRecordComponent(const GJHRecordComponent&& _Other) = delete;

public:
	GJHRecordComponent operator=(const GJHRecordComponent& _Other) = delete;
	GJHRecordComponent operator=(const GJHRecordComponent&& _Other) = delete;

public:
	void Update() override;
	void Record(const FrameInfo& _Info);
	void Rewind();
	void Clear();
};