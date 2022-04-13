#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHDirectResourcesManager.h"
#include "GJHDirectVertexBuffer.h"
#include "GJHDirectIndexBuffer.h"
#include "GJHDirectMesh.h"
#include "GJHVertexHeader.h"
#include "GJHDirectConstantBuffer.h"
#include <GJHGameEngineMath.h>

void GJHGameEngineDevice::MeshInit()
{
	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(5);
		VBVector[0] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.5f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.5f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { -0.5f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[4] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"RectDebug", VBVector);
		std::vector<unsigned int> IBVector = { 0, 1, 2, 3, 4, 5 };
		GJHDirectIndexBuffer::Create(L"RectDebug", IBVector);
		GJHDirectMesh::Create(L"RectDebug");
	}
	
	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"Rect", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"Rect", IBVector);
		GJHDirectMesh::Create(L"Rect");
	}

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 0.f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.f, 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.f, -1.f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -1.f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"Ray", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"Ray", IBVector);
		GJHDirectMesh::Create(L"Ray");
	}

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"RayCenter", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"RayCenter", IBVector);
		GJHDirectMesh::Create(L"RayCenter");
	}

	{
		std::vector<ParticleVTX> VBVector;
		VBVector.resize(1);
		VBVector[0] = ParticleVTX{ {0.0f, 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f}, 0 };
		GJHDirectVertexBuffer::Create(L"Point", VBVector);

		std::vector<unsigned int> IBVector = { 0 };
		GJHDirectIndexBuffer::Create(L"Point", IBVector);

		GJHDirectMesh::Create(L"Point");
	}

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 1.f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { -1.f, 1.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { -1.f, -1.f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -1.f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"FullRect", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"FullRect", IBVector);
		GJHDirectMesh::Create(L"FullRect");
	}
}