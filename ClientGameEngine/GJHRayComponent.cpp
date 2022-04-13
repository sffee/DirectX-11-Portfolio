#include "GJHRayComponent.h"
#include "GJHRenderer.h"
#include <GJHGameEngineActor.h>
#include "GJHGameEngineCollision.h"
#include "GJHWorldComponent.h"
#include <GJHDirectTexture2D.h>

#define DETECT_TARGET 9999

int GJHRayComponent::DoorLayerIndex = 6;
bool GJHRayComponent::m_IsDebugRender = false;

GJHRayComponent::GJHRayComponent() :
	m_DebugAngle(0.f),
	m_DebugRenderScaleX(0.f),
	m_Pause(false)
{

}

GJHRayComponent::~GJHRayComponent()
{

}

void GJHRayComponent::Start()
{
	GJHActorTransform::Start();

	ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();
	if (Trans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
	}

	this->AttachParent(Trans);

	m_DebugRender = GetActor()->CreateComponent<GJHRenderer>();
	m_DebugRender->CreateRenderPlayer(L"BoxMat", L"Ray");
	m_DebugRender->CBufferSettingLink(L"ColorData", m_DebugRenderColor);

	m_DebugRenderColor = float4::RED;
}

void GJHRayComponent::Update()
{
	
}

void GJHRayComponent::SetLocalPosition(const float4& _Pos)
{
	GJHActorTransform::SetLocalPosition(_Pos);

	if (m_IsDebugRender == true)
	{
		m_DebugRender->SetLocalPositionY(_Pos.y);
	}
}

void GJHRayComponent::SetColor(const float4& _Color)
{
	m_DebugRenderColor = _Color;
}

bool GJHRayComponent::CheckActor(const float4& _FindActorPos, const float4& _ThisActorPos,
								 std::shared_ptr<GJHDirectTexture2D> _ColMapTexture, int _Dir, float _Angle, int _Range)
{
	float4 FindActorPos = _FindActorPos;
	float4 ThisActorPos = _ThisActorPos;

	int ActorLen = (int)float4::CalLen(FindActorPos - _ThisActorPos);

	m_DebugRenderScaleX = 0;

	if (_Range < ActorLen)
		return false;

	m_DebugAngle = _Angle;

	float4 DirVec = float4::GlobalDirToDeg(_Angle);

	float4 Pos = GetWorldPosition();
	int PosX = Pos.ix();
	int PosY = Pos.iy();

	int NearestColLen = DETECT_TARGET;

	// 맵 충돌 검출
	int RayLengthMul = 1;
	while (true)
	{
		int SearchPosX = PosX + (int)(DirVec.x * RayLengthMul);
		int SearchPosY = abs(PosY) + (int)(-DirVec.y * RayLengthMul);

		float4 ColColor = _ColMapTexture->GetPixel(SearchPosX, SearchPosY);
		if (ColColor == float4::RED
			|| ColColor == float4::GREEN)
		{
			NearestColLen = (int)float4::CalLen(DirVec * RayLengthMul);
			break;
		}

		if (_Range < RayLengthMul || ActorLen < RayLengthMul)
			break;

		RayLengthMul++;
	}

	// 충돌체 검출
	std::list<std::shared_ptr<GJHGameEngineCollision>>& ColList = GetMasterWorld()->GetCollisionGroup(DoorLayerIndex);
	for (auto& Col : ColList)
	{
		if (NearestColLen != DETECT_TARGET && ActorLen < NearestColLen)
			break;

		if (Col->GetColCheck() == false)
			continue;

		float4 ColPos = Col->GetTransform().GetWorldPosition();
		if (_Range < float4::CalLen(FindActorPos - ColPos))
			continue;

		float4 ColScale = Col->GetTransform().GetWorldScale();
		float4 ColPoint;

		// 좌우에 있는 충돌체 검출
		if (_Dir == 1)
		{
			float4 ColLeftUpLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * 0.5f);
			float4 ColLeftDownLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * -0.5f);

			ColPoint = GetLineInterectionPoint(ThisActorPos, ThisActorPos + DirVec, ColLeftUpLine, ColLeftDownLine);
		}
		else
		{
			float4 ColRightUpLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * 0.5f);
			float4 ColRightDownLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * -0.5f);

			ColPoint = GetLineInterectionPoint(ThisActorPos, ThisActorPos + DirVec, ColRightUpLine, ColRightDownLine);
		}

		if (ColPoint != float4::ZERO)
		{
			float Len = float4::CalLen(ThisActorPos - ColPoint);
			if (Len < NearestColLen)
				NearestColLen = (int)float4::CalLen(DirVec * Len);

			continue;
		}

		// 좌우 검출 실패시 상하 충돌체 검출
		if (0.f < DirVec.y)
		{
			float4 ColUpLeftLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * 0.5f);
			float4 ColUpRightLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * 0.5f);

			ColPoint = GetLineInterectionPoint(ThisActorPos, ThisActorPos + DirVec, ColUpLeftLine, ColUpRightLine);
		}
		else
		{
			float4 ColDownLeftLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * -0.5f);
			float4 ColDownRightLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * -0.5f);

			ColPoint = GetLineInterectionPoint(ThisActorPos, ThisActorPos + DirVec, ColDownLeftLine, ColDownRightLine);
		}

		if (ColPoint != float4::ZERO)
		{
			float Len = float4::CalLen(ThisActorPos - ColPoint);
			if (Len < NearestColLen)
				NearestColLen = (int)float4::CalLen(DirVec * Len);
		}
	}

	if (NearestColLen == DETECT_TARGET || ActorLen < NearestColLen)
	{
		m_DebugRenderScaleX = (float)ActorLen;
		return true;
	}

	m_DebugRenderScaleX = (float)NearestColLen;

	return false;
}

float4 GJHRayComponent::GetLineInterectionPoint(const float4& _FindActorPosStart, const float4& _FindActorPosEnd, const float4& _ColObjPosStart, const float4& _ColObjPosEnd)
{
	float x1 = _ColObjPosStart.x;
	float y1 = _ColObjPosStart.y;
	float x2 = _ColObjPosEnd.x;
	float y2 = _ColObjPosEnd.y;

	float x3 = _FindActorPosStart.x;
	float y3 = _FindActorPosStart.y;
	float x4 = _FindActorPosEnd.x;
	float y4 = _FindActorPosEnd.y;

	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (den == 0)
	{
		return float4::ZERO;
	}

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	if (0 < t && t < 1 && 0 < u)
	{
		float4 InterectionPoint;
		InterectionPoint.x = x1 + t * (x2 - x1);
		InterectionPoint.y = y1 + t * (y2 - y1);

		return InterectionPoint;
	}

	return float4::ZERO;
}

bool GJHRayComponent::CheckActorColToAngle(const float4& _StartPos, const float4& _ColPos, const float4& _ColScale, float _Angle, int _Dir)
{
	float4 StartPos = _StartPos;
	float4 DirVec = float4::GlobalDirToDeg(_Angle);

	float4 ColPos = _ColPos;
	float4 ColScale = _ColScale;

	float4 UDColPoint;
	float4 LRColPoint;

	if (_Dir == 1)
	{
		float4 ColLeftUpLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * 0.5f);
		float4 ColLeftDownLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * -0.5f);

		UDColPoint = GetLineInterectionPoint(StartPos, StartPos + DirVec, ColLeftUpLine, ColLeftDownLine);
	}
	else
	{
		float4 ColRightUpLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * 0.5f);
		float4 ColRightDownLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * -0.5f);

		UDColPoint = GetLineInterectionPoint(StartPos, StartPos + DirVec, ColRightUpLine, ColRightDownLine);
	}

	if (0.f < DirVec.y)
	{
		float4 ColUpLeftLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * 0.5f);
		float4 ColUpRightLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * 0.5f);

		LRColPoint = GetLineInterectionPoint(StartPos, StartPos + DirVec, ColUpLeftLine, ColUpRightLine);
	}
	else
	{
		float4 ColDownLeftLine = ColPos + float4(ColScale.x * -0.5f, ColScale.y * -0.5f);
		float4 ColDownRightLine = ColPos + float4(ColScale.x * 0.5f, ColScale.y * -0.5f);

		LRColPoint = GetLineInterectionPoint(StartPos, StartPos + DirVec, ColDownLeftLine, ColDownRightLine);
	}

	if (UDColPoint == float4::ZERO
		&& LRColPoint == float4::ZERO)
	{
		return false;
	}

	return true;
}

void GJHRayComponent::DebugRender()
{
	if (m_Pause == true)
	{
		return;
	}

	m_DebugRender->SetLocalRotationZ(m_DebugAngle);
	m_DebugRender->SetWorldScaleX(m_DebugRenderScaleX);
	m_DebugRender->SetRenderEnable(m_IsDebugRender);
}

float GJHRayComponent::GetDebugAngle()
{
	return m_DebugAngle;
}

void GJHRayComponent::PauseDebugRender()
{
	m_Pause = true;
	m_DebugRenderScaleX = 0.f;
	m_DebugRender->SetWorldScaleX(m_DebugRenderScaleX);
}

void GJHRayComponent::ResumeDebugRender()
{
	m_Pause = false;
}

void GJHRayComponent::ClearmDebugRenderScaleX()
{
	m_DebugRenderScaleX = 0.f;
}