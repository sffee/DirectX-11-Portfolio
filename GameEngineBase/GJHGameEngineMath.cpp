#include "BasePre.h"
#include "GJHGameEngineMath.h"

double GJHGameEngineMath::DPI = 3.141592653589793238462643383279502884197169399375105820974944;
float GJHGameEngineMath::FPI = 3.141592653589793238462643383279502884197169399375105820974944f;
float GJHGameEngineMath::D2R = FPI / 180.0f;
float GJHGameEngineMath::R2D = 180.0f / FPI;

GJHGameEngineVector GJHGameEngineVector::ZERO = { 0.f, 0.f, 0.f, 0.0f };
GJHGameEngineVector GJHGameEngineVector::ONE = { 1.f, 1.f, 1.f };

GJHGameEngineVector GJHGameEngineVector::RED = { 1.f, 0.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::GREEN = { 0.f, 1.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::BLUE = { 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::CYAN = { 0.f, 1.f, 1.f };

GJHGameEngineVector GJHGameEngineVector::LEFT = { -1.f, 0.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::RIGHT = { 1.f, 0.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::UP = { 0.f, 1.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::DOWN = { 0.f, -1.f, 0.f };
GJHGameEngineVector GJHGameEngineVector::FOWARD = { 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::BACK = { 0.f, 0.f, -1.f };

GJHGameEngineVector operator*(const GJHGameEngineVector& _Left, const GJHGameEngineMatrix& _Right)
{
	GJHGameEngineVector Result;

	for (size_t i = 0; i < 4; i++)
	{
		Result.Arr[i] = _Left.Arr[0] * _Right.Arr[0][i] +
			_Left.Arr[1] * _Right.Arr[1][i] +
			_Left.Arr[2] * _Right.Arr[2][i] +
			_Left.Arr[3] * _Right.Arr[3][i];
	}

	return Result;
}