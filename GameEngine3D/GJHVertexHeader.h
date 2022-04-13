#pragma once
#include <GJHGameEngineMath.h>

struct VertexBase
{
public:
	float4 Pos;
	float4 Uv;
	float4 Color;
};

struct ParticleVTX
{
public:
	float4 Pos;
	float4 Uv;
	int Index;
};