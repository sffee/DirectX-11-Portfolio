#include "GJHDirectTexture2D.h"
#include <GJHGameEngineDebug.h>
#include "GJHGameEngineDevice.h"
#include <GJHGameEngineDirectory.h>
#include <GJHGameEngineFile.h>

GJHDirectTexture2D::GJHDirectTexture2D() :
	m_pRTV(nullptr),
	m_pDSV(nullptr),
	m_pSRV(nullptr),
	m_pTexture2D(nullptr),
	m_Info({ 0 })
{

}

GJHDirectTexture2D::~GJHDirectTexture2D()
{
	if (m_pRTV != nullptr)
	{
		m_pRTV->Release();
		m_pRTV = nullptr;
	}

	if (m_pDSV != nullptr)
	{
		m_pDSV->Release();
		m_pDSV = nullptr;
	}

	if (m_pSRV != nullptr)
	{
		m_pSRV->Release();
		m_pSRV = nullptr;
	}

	if (m_pTexture2D != nullptr)
	{
		m_pTexture2D->Release();
		m_pTexture2D = nullptr;
	}
}

void GJHDirectTexture2D::CreateTexture2D(const D3D11_TEXTURE2D_DESC& _Desc)
{
	m_Info = _Desc;

	GJHGameEngineDevice::GetDevice()->CreateTexture2D(&_Desc, 0, &m_pTexture2D);

	if (m_pTexture2D == nullptr)
	{
		MsgAssert("if (m_pTexture2D == nullptr)");
	}
}

void GJHDirectTexture2D::CreateRes(const int _BindFlag)
{
	if (m_pTexture2D == nullptr
		&& m_Image.GetImages() == nullptr)
	{
		MsgAssert("if (m_pTexture2D == nullptr)");
	}

	if ((_BindFlag & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET) != 0)
	{
		if (m_pDSV != nullptr)
		{
			MsgAssert("if (m_pDSV != nullptr)");
		}

		if (m_pRTV != nullptr)
		{
			MsgAssert("if (m_pRTV != nullptr)");
		}

		if (GJHGameEngineDevice::GetDevice()->CreateRenderTargetView(m_pTexture2D, 0, &m_pRTV) != S_OK)
		{
			MsgAssert("����� �̿������ ������ ���߽��ϴ�.");
		}
	}

	if ((_BindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE) != 0)
	{
		if (m_pDSV != nullptr)
		{
			MsgAssert("if (m_pDSV != nullptr)");
		}

		if (GJHGameEngineDevice::GetDevice()->CreateShaderResourceView(m_pTexture2D, 0, &m_pSRV) != S_OK)
		{
			MsgAssert("����� �̿������ ������ ���߽��ϴ�.");
		}
	}

	if ((_BindFlag & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL) != 0)
	{
		if (m_pDSV != nullptr)
		{
			MsgAssert("if (m_pDSV != nullptr)");
		}

		if (m_pRTV != nullptr)
		{
			MsgAssert("if (m_pRTV != nullptr)");
		}

		if (m_pSRV != nullptr)
		{
			MsgAssert("if (m_pSRV != nullptr)");
		}
		
		if (GJHGameEngineDevice::GetDevice()->CreateDepthStencilView(m_pTexture2D, 0, &m_pDSV) != S_OK)
		{
			MsgAssert("���� ���� ���� ������ �����߽��ϴ�.");
		}
	}
}

bool GJHDirectTexture2D::LoadRes(const GJHGameEngineString& _Path, const GJHGameEngineString& _FileName)
{
	GJHGameEngineFile File = _Path;
	GJHGameEngineString Ext = File.Extension();

	Ext.SetUpper();

	m_TextureName = _FileName;

	if (Ext == L"PNG")
	{
		if (DirectX::LoadFromWICFile(_Path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image) != S_OK)
		{
			GJHGameEngineDebug::AssertMsg("if (DirectX::LoadFromWICFile(_Path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image) != S_OK)");
		}
	}
	else if (Ext == L"TGA")
	{
		if (DirectX::LoadFromTGAFile(_Path.c_str(), nullptr, m_Image) != S_OK)
		{
			GJHGameEngineDebug::AssertMsg("if (DirectX::LoadFromTGAFile(_Path.c_str(), nullptr, m_Image) != S_OK)");
		}
	}
	else if (Ext == L"DDS")
	{
		if (DirectX::LoadFromDDSFile(_Path.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image) != S_OK)
		{
			GJHGameEngineDebug::AssertMsg("if (DirectX::LoadFromDDSFile(_Path.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image) != S_OK)");
		}
	}
	else
	{
		GJHGameEngineDebug::AssertMsg("Image FIle Load Error");
	}

	m_Info.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	if (DirectX::CreateShaderResourceView(GJHGameEngineDevice::GetDevice(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(), &m_pSRV) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("DirectX::CreateShaderResourceView != S_OK");
	}

	m_Info.Width = static_cast<UINT>(m_Image.GetMetadata().width);
	m_Info.Height = static_cast<UINT>(m_Image.GetMetadata().height);

	return true;
}

void GJHDirectTexture2D::Setting(ShaderType _Type, int _Reg)
{
	switch (_Type)
	{
	case ShaderType::Vertex:
		VSSetting(_Reg);
		break;
	case ShaderType::Hull:
	case ShaderType::Geometry:
	case ShaderType::Domain:
		GJHGameEngineDebug::AssertMsg("case ShaderType Error");
		break;
	case ShaderType::Pixel:
		PSSetting(_Reg);
		break;
	case ShaderType::End:
		break;
	default:
		break;
	}
}

void GJHDirectTexture2D::VSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->VSSetShaderResources(_Reg, 1, &m_pSRV);
}

void GJHDirectTexture2D::GSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->GSSetShaderResources(_Reg, 1, &m_pSRV);
}

void GJHDirectTexture2D::PSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->PSSetShaderResources(_Reg, 1, &m_pSRV);
}

float4 GJHDirectTexture2D::GetPixel(int _X, int _Y)
{
	if (_X < 0 || GetSizeX() <= (unsigned int)_X)
	{
		return float4();
	}

	if (_Y < 0 || GetSizeY() <= (unsigned int)_Y)
	{
		return float4();
	}

	uint8_t* Ptr = m_Image.GetPixels();

	DXGI_FORMAT Format = m_Image.GetMetadata().format;

	float4 ReturnColor;

	switch (Format)
	{
	case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
	{
		int* ColorPtr = reinterpret_cast<int*>(Ptr);

		ColorPtr += _Y * GetSizeX64();
		ColorPtr += _X;

		Ptr = reinterpret_cast<uint8_t*>(ColorPtr);

		ReturnColor.b = static_cast<float>(Ptr[0]) / 255.f;
		ReturnColor.g = static_cast<float>(Ptr[1]) / 255.f;
		ReturnColor.r = static_cast<float>(Ptr[2]) / 255.f;
		ReturnColor.a = static_cast<float>(Ptr[3]) / 255.f;
		break;
	}
	case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
	{
		float4* ColorPtr = reinterpret_cast<float4*>(Ptr);

		ColorPtr += _Y * GetSizeX64();
		ColorPtr += _X;

		Ptr = reinterpret_cast<uint8_t*>(ColorPtr);

		ReturnColor.r = static_cast<float>(Ptr[0]) / 255.f;
		ReturnColor.g = static_cast<float>(Ptr[4]) / 255.f;
		ReturnColor.b = static_cast<float>(Ptr[8]) / 255.f;
		ReturnColor.a = static_cast<float>(Ptr[12]) / 255.f;

		break;
	}
	default:
		break;
	}

	return ReturnColor;
}

Color255 GJHDirectTexture2D::GetPixel255(int _X, int _Y)
{
	uint8_t* Ptr = m_Image.GetPixels();

	DXGI_FORMAT Format = m_Image.GetMetadata().format;

	Color255 ReturnColor;

	switch (Format)
	{
	case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
	{
		int* ColorPtr = reinterpret_cast<int*>(Ptr);

		ColorPtr += _Y * GetSizeX64();
		ColorPtr += _X;

		Ptr = reinterpret_cast<uint8_t*>(ColorPtr);

		ReturnColor.b = Ptr[0];
		ReturnColor.g = Ptr[1];
		ReturnColor.r = Ptr[2];
		ReturnColor.a = Ptr[3];
		break;
	}
	case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
	{
		int* ColorPtr = reinterpret_cast<int*>(Ptr);

		ColorPtr += _Y * GetSizeX64();
		ColorPtr += _X;

		Ptr = reinterpret_cast<uint8_t*>(ColorPtr);

		ReturnColor.r = static_cast<char>(Ptr[0] * 255.f);
		ReturnColor.g = static_cast<char>(Ptr[4] * 255.f);
		ReturnColor.b = static_cast<char>(Ptr[8] * 255.f);
		ReturnColor.a = static_cast<char>(Ptr[12] * 255.f);

		break;
	}
	default:
		break;
	}

	return ReturnColor;
}

void GJHDirectTexture2D::CaptureTexture()
{
	HRESULT result;
	result = DirectX::CaptureTexture(GJHGameEngineDevice::GetDevice(), GJHGameEngineDevice::GetContext(), m_pTexture2D, m_Image);
}

void GJHDirectTexture2D::SavePngFile(const GJHGameEngineString& _FileName)
{
	CaptureTexture();

	DirectX::SaveToWICFile(m_Image.GetImages(), m_Image.GetImageCount(), DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG), _FileName.c_str());
}