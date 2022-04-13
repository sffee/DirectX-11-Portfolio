#include "GeoPre.h"
#include "GJHGameEngineTransform.h"
#include "GJHGameEngineWorld.h"

GJHGameEngineTransform::GJHGameEngineTransform() :
	m_LocalScale(float4::ONE),
	m_WorldScale(float4::ONE),
	CheckTransUpdate{ false },
	m_ParentTrans(),
	m_ParentWorld(nullptr)
{
	for (bool& Check : CheckTransUpdate)
	{
		Check = true;
	}
}

GJHGameEngineTransform::GJHGameEngineTransform(GJHGameEngineWorld* _ParentWorld) :
	m_LocalScale(float4::ONE),
	m_WorldScale(float4::ONE),
	CheckTransUpdate{ false },
	m_ParentTrans(),
	m_ParentWorld(_ParentWorld)
{

}

GJHGameEngineTransform::~GJHGameEngineTransform()
{

}

void GJHGameEngineTransform::AttachParent(std::shared_ptr<GJHGameEngineTransform> _Parent)
{
	m_ParentWorld->ParentTransCheckOn();
	m_ParentTrans = _Parent;

	_Parent->m_ChildTransList.push_back(this);

	CalWorldScale();
	CalWorldRotation();
	CalWorldPosition();
	CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
}

void GJHGameEngineTransform::CalTransData()
{
	if (CheckTransUpdate[(int)MatrixType::Matrix_Scale] == true)
	{
		TransData.Scale.Scale(m_LocalScale);
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_Rotation] == true)
	{
		TransData.Rotation.RotationXYZDeg(m_LocalRotation);
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_Position] == true)
	{
		TransData.Position.Position(m_LocalPosition);
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_World] == true)
	{
		if (m_ParentTrans.use_count() != 0)
		{
			SharedPtr<GJHGameEngineTransform> Parent = m_ParentTrans.lock();
			TransData.Parent = Parent->TransData.World;
		}

		TransData.CalWorld();
		CheckTransUpdate[(int)MatrixType::Matrix_World] = false;
	}

	for (auto& _Child : m_ChildTransList)
	{
		_Child->CalTransData();
	}
}

void GJHGameEngineTransform::Release()
{

}

void GJHGameEngineTransform::ColUpdate(COLTYPE _Type, bool _Is2D, int _Order)
{
	m_ColData.Obb.Center = m_WorldPosition.DXXmFloat3;

	if (_Is2D == true)
	{
		float* OrederPtr = &m_ColData.Obb.Center.x;
		OrederPtr[_Order] = 0.f;
	}

	m_ColData.Obb.Extents = m_WorldScale.HalfVector3D().DXXmFloat3;

	if (_Type != COLTYPE::CIRCULARSECTOR2D)
	{
		m_ColData.Obb.Orientation = m_WorldRotation.DegToQuaternion().DXXmFloat4;
	}
	else
	{
		m_ColData.Obb.Orientation.x = GetLocalXDir().x;
		m_ColData.Obb.Orientation.y = GetLocalXDir().y;
		m_ColData.Obb.Orientation.z = GetLocalXDir().z;
	}
}

DirectX::BoundingBox& GJHGameEngineTransform::AABB()
{
	return m_ColData.Aabb;
}

DirectX::BoundingOrientedBox& GJHGameEngineTransform::OBB()
{
	return m_ColData.Obb;
}

DirectX::BoundingSphere& GJHGameEngineTransform::SPHERE()
{
	return m_ColData.Sphere;
}

void GJHGameEngineTransform::Reset()
{
	m_LocalScale = float4::ONE;
	m_LocalRotation = float4::ZERO;
	m_LocalPosition = float4::ZERO;
	m_WorldScale = float4::ONE;
	m_WorldRotation = float4::ZERO;
	m_WorldPosition = float4::ZERO;

	for (size_t i = 0; i < (int)MatrixType::Matrix_Max; i++)
	{
		CheckTransUpdate[i] = false;
	}

	m_ParentTrans = std::shared_ptr<GJHGameEngineTransform>(nullptr);
	m_ChildTransList.clear();
}