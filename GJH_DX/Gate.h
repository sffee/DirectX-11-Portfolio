#pragma once
#include <GJHContentsComponent.h>
#include "ObjectBase.h"

class Gate : public ObjectBase
{
private:
	std::shared_ptr<GJHCollisionComponent> m_Collision;
	GJHGameEngineString m_NextSceneName;

private:
	std::shared_ptr<GJHSpriteRenderer> m_ArrowRender;
	std::shared_ptr<GJHActorTransform> m_ArrowTrans;
	//std::shared_ptr<GJHSpriteRenderer> m_TextRender;
	//std::shared_ptr<GJHActorTransform> m_TextTrans;

private:
	std::shared_ptr<GJHActorTransform> m_CamTrans;

private:
	float m_Time;
	bool m_StartMapMove;

	int m_Dir;
	float m_RenderGap;
	bool m_CheckRenderGap;
	float m_GapTime;

public:
	Gate();
	~Gate();

public:
	Gate(const Gate& _Other) = delete;
	Gate(const Gate&& _Other) = delete;

public:
	Gate operator=(const Gate& _Other) = delete;
	Gate operator=(const Gate&& _Other) = delete;

public:
	void Start(const GJHGameEngineString& _NextSceneName, int _Dir, std::shared_ptr<GJHCamera> _Cam);
	void Update() override;
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

private:
	void SetGoPosition();
};