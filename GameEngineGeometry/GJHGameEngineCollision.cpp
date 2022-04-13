#include "GeoPre.h"
#include "GJHGameEngineCollision.h"
#include "GJHGameEngineTransForm.h"

GJHGameEngineCollision::StarterClass GJHGameEngineCollision::Starter = GJHGameEngineCollision::StarterClass();

bool(__stdcall* GJHGameEngineCollision::ColCheckFunc[(int)COLTYPE::NONE][(int)COLTYPE::NONE])(GJHGameEngineTransform&, GJHGameEngineTransform&) = { nullptr, };

void GJHGameEngineCollision::Init()
{
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::AABB] = &GJHGameEngineCollision::AABBToAABB;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::OBB] = &GJHGameEngineCollision::AABBToOBB;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::AABBToSPHERE;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::POINT] = &GJHGameEngineCollision::AABBToPOINT;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::AABB] = &GJHGameEngineCollision::OBBToAABB;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::OBB] = &GJHGameEngineCollision::OBBToOBB;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::OBBToSPHERE;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::POINT] = &GJHGameEngineCollision::OBBToPOINT;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::AABB] = &GJHGameEngineCollision::SPHEREToAABB;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::OBB] = &GJHGameEngineCollision::SPHEREToOBB;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::SPHEREToSPHERE;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::POINT] = &GJHGameEngineCollision::SPHEREToPOINT;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::AABB] = &GJHGameEngineCollision::POINTToAABB;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::OBB] = &GJHGameEngineCollision::POINTToOBB;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::POINTToSPHERE;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::POINT] = &GJHGameEngineCollision::POINTToPOINT;
	ColCheckFunc[(int)COLTYPE::CIRCULARSECTOR2D][(int)COLTYPE::AABB] = &GJHGameEngineCollision::CIRCULARSECTOR2DToAABB;
	ColCheckFunc[(int)COLTYPE::CIRCULARSECTOR2D][(int)COLTYPE::OBB] = &GJHGameEngineCollision::CIRCULARSECTOR2DToOBB;
	ColCheckFunc[(int)COLTYPE::CIRCULARSECTOR2D][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::CIRCULARSECTOR2DToSPHERE;
	ColCheckFunc[(int)COLTYPE::CIRCULARSECTOR2D][(int)COLTYPE::POINT] = &GJHGameEngineCollision::CIRCULARSECTOR2DToPOINT;
}

bool GJHGameEngineCollision::OBBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.OBB());
}

bool GJHGameEngineCollision::OBBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.AABB());
}
bool GJHGameEngineCollision::OBBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.SPHERE());
}
bool GJHGameEngineCollision::OBBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.OBB().Intersects(BS);
}

bool GJHGameEngineCollision::AABBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.AABB());
}
bool GJHGameEngineCollision::AABBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.OBB());
}
bool GJHGameEngineCollision::AABBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.SPHERE());
}
bool GJHGameEngineCollision::AABBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.AABB().Intersects(BS);
}

bool GJHGameEngineCollision::SPHEREToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.SPHERE());
}

bool GJHGameEngineCollision::SPHEREToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.OBB());
}

bool GJHGameEngineCollision::SPHEREToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.AABB());
}

bool GJHGameEngineCollision::SPHEREToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.SPHERE().Intersects(BS);
}

bool GJHGameEngineCollision::POINTToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	float4 LEFT = { _LeftTransform.SPHERE().Center.x, _LeftTransform.SPHERE().Center.y, _LeftTransform.SPHERE().Center.z };
	float4 RIGHT = { _RightTransform.SPHERE().Center.x, _RightTransform.SPHERE().Center.y, _RightTransform.SPHERE().Center.z };

	return LEFT == RIGHT;
}

bool GJHGameEngineCollision::POINTToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return OBBToPOINT(_RightTransform, _LeftTransform);
}

bool GJHGameEngineCollision::POINTToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return AABBToPOINT(_RightTransform, _LeftTransform);
}

bool GJHGameEngineCollision::POINTToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return SPHEREToPOINT(_RightTransform, _LeftTransform);
}

bool GJHGameEngineCollision::CIRCULARSECTOR2DToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	if (SPHEREToPOINT(_LeftTransform, _RightTransform) == true)
	{
		float4 LeftPos = &_LeftTransform.SPHERE().Center.x;
		float4 RightPos = &_RightTransform.SPHERE().Center.x;
		float4 Dir = RightPos - LeftPos;

		float Angle = atan2f(Dir.y, Dir.x) * GJHGameEngineMath::R2D;

		return true;
	}

	return false;
}

bool GJHGameEngineCollision::CIRCULARSECTOR2DToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	if (SPHEREToPOINT(_LeftTransform, _RightTransform) == true)
	{
		return true;
	}

	return false;
}

bool GJHGameEngineCollision::CIRCULARSECTOR2DToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	if (SPHEREToPOINT(_LeftTransform, _RightTransform) == true)
	{
		return true;
	}

	return false;
}

bool GJHGameEngineCollision::CIRCULARSECTOR2DToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	if (SPHEREToPOINT(_LeftTransform, _RightTransform) == true)
	{
		float4 LeftPos = &_LeftTransform.SPHERE().Center.x;
		float4 RightPos = &_RightTransform.SPHERE().Center.x;
		float4 Dir = RightPos - LeftPos;
		Dir = Dir.GetNormalize();
		float4 TargetDir = &_LeftTransform.OBB().Orientation.x;

		float Angle = float4::CalDot(Dir, TargetDir);
		Angle = acosf(Angle);
		Angle = Angle * GJHGameEngineMath::R2D;

		if (Angle <= 15.f)
		{
			return true;
		}
	}

	return false;
}

GJHGameEngineCollision::GJHGameEngineCollision(GJHGameEngineTransform& _Transform) :
	m_Transform(_Transform),
	Type(COLTYPE::NONE),
	m_Is2D(true),
	Order2D(2),
	m_Data(nullptr),
	m_IsColCheck(true)
{

}

GJHGameEngineCollision::~GJHGameEngineCollision()
{

}

void GJHGameEngineCollision::ColUpdate()
{
	m_Transform.ColUpdate(Type, m_Is2D, Order2D);
}

bool GJHGameEngineCollision::ColCheck(GJHGameEngineCollision& _Other)
{
	bool Check;

	if (m_IsColCheck == false
		|| _Other.GetColCheck() == false)
	{
		Check = false;
	}
	else
	{
		Check = ColCheckFunc[(int)Type][(int)_Other.Type](m_Transform, _Other.m_Transform);
	}

	if (Check == true)
	{
		if (m_ColTrueEvent != nullptr)
		{
			m_ColTrueEvent(&_Other);
		}
	}
	else
	{
		if (m_ColFalseEvent != nullptr)
		{
			m_ColFalseEvent(&_Other);
		}
	}

	return Check;
}