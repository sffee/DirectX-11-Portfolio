#include "InstanceObject.h"

InstanceObject::InstanceObject()
{

}

InstanceObject::~InstanceObject()
{

}

void InstanceObject::Start(InstanceObjectData _Data, std::shared_ptr<GJHSpriteRenderer> _Renderer)
{
	ObjectBase::Start(_Renderer, float4(1.f, 1.f));

	m_Data = _Data;

	m_IsRecord = _Data.IsRecord;
	m_IsRenderUpdate = _Data.IsRenderUpdate;

	SetDirection(m_Data.Dir);

	CreateAnimation(m_Data.AnimationName, m_Data.TextureName, m_Data.AnimationFrameTime, m_Data.AnimationLoop);
	ChangeAnimation(m_Data.AnimationName);

	m_Animation->SetUseTimeScale(_Data.UseTimeScale);

	if (_Data.DirVecMovePos != 0.f)
	{
		m_Trans->SetWorldMove(float4::GlobalDirToDeg(_Data.Angle) * _Data.DirVecMovePos);
	}

	m_ApplyGravity = m_Data.ApplyGravity;
	m_IsGravityCheckMapColor = m_Data.IsGravityCheckMapColor;
	
	if (_Data.IsJump == true)
	{
		Jump(_Data.Gravity, _Data.GravityAddValue);
	}

	m_CheckPlatform = false;
	m_CheckWall = _Data.CheckWall;

	if (m_Data.MeshName != L"")
	{
		m_Render->SetMesh(m_Data.MeshName);
	}

	m_IsEnd = false;

	m_MoveSpeed = _Data.MoveSpeed;
	m_RenderScale = _Data.RenderScale;
}

void InstanceObject::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		if (m_Data.AnimationName == L"GunFire4")
		{
			int a = 0;
		}

		if (m_Render->GetRenderEnable() == true)
		{
			ObjectBase::Update();
		}
		else
		{
			ObjectBase::RecordUpdate();
		}

		if (m_IsEnd == true)
		{
			return;
		}

		if (m_Data.IsDeath == true)
		{
			m_Data.LifeTime -= DELTATIME();
			if (m_Data.LifeTime <= 0.f
				&& AnimationEnd())
			{
				GetActor()->Death();
			}
		}
		else
		{
			if (m_Data.AnimationEndHide == true
				&& AnimationEnd())
			{
				End();
				m_Render->SetRenderEnable(false);
			}
		}
	}
	else
	{
		if (m_IsRecord == false)
		{
			m_Render->SetRenderEnable(false);
		}
	}
}

void InstanceObject::Start(float4 _Pos)
{
	m_IsEnd = false;
	m_Render->SetRenderEnable(true);
	m_Animation->Reset();

	if (_Pos != float4::ZERO)
	{
		m_Trans->SetWorldPosition(_Pos);
	}
}

void InstanceObject::End()
{
	m_IsEnd = true;
}