#include "GJHDirectMesh.h"
#include "GJHGameEngineDevice.h"

GJHDirectMesh::GJHDirectMesh()
{

}

GJHDirectMesh::~GJHDirectMesh()
{

}

bool GJHDirectMesh::VertexBufferPush(const GJHGameEngineString& _Name)
{
	std::shared_ptr<GJHDirectVertexBuffer> VB = GJHDirectVertexBuffer::Find(_Name);

	if (VB == nullptr)
	{
		MsgAssert("if (VB == nullptr)");
		return false;
	}

	m_VertexBuffers.push_back(VB);

	return true;
}

bool GJHDirectMesh::IndexBufferPush(const GJHGameEngineString& _Name)
{
	std::shared_ptr<GJHDirectIndexBuffer> IB = GJHDirectIndexBuffer::Find(_Name);

	if (IB == nullptr)
	{
		MsgAssert("if (IB == nullptr)");
		return false;
	}

	m_IndexBuffers.push_back(IB);

	return true;
}

bool GJHDirectMesh::Create(const GJHGameEngineString& _VBName, const GJHGameEngineString& _IBName)
{
	if (VertexBufferPush(_VBName) == false)
	{
		return false;
	}

	if (IndexBufferPush(_IBName) == false)
	{
		return false;
	}

	return true;
}

void GJHDirectMesh::Setting()
{
	if (m_VertexBuffers.size() != 0 && m_VertexBuffers[0] != nullptr)
	{
		m_VertexBuffers[0]->Setting();
	}

	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		m_IndexBuffers[0]->Setting();
	}

	return;
}

void GJHDirectMesh::RenderIndexed()
{
	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		GJHGameEngineDevice::GetContext()->DrawIndexed(m_IndexBuffers[0]->GetIndexCount(), 0, 0);
	}
}

void GJHDirectMesh::RenderIndexedInstanced(int _Index)
{
	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		GJHGameEngineDevice::GetContext()->DrawIndexedInstanced(m_IndexBuffers[0]->GetIndexCount(), _Index, 0, 0, 0);
	}
}