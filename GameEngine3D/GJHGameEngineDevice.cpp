#include "GJHGameEngineDevice.h"
#include <GJHGameEngineDebug.h>
#include "GJHDirectRenderTarget.h"
#include <GJHGameEngineDirectory.h>
#include <GJHGameEngineFile.h>
#include "GJHMaterial.h"

UINT GJHGameEngineDevice::m_MC;
UINT GJHGameEngineDevice::m_MQ;
HWND GJHGameEngineDevice::m_Hwnd;
float4 GJHGameEngineDevice::m_Size;
IDXGISwapChain* GJHGameEngineDevice::m_pSwapChain = nullptr;

ID3D11Device* GJHGameEngineDevice::m_pDevice = nullptr;
ID3D11DeviceContext* GJHGameEngineDevice::m_pContext = nullptr;

std::shared_ptr<GJHDirectRenderTarget> GJHGameEngineDevice::m_BackBuffer = nullptr;

void GJHGameEngineDevice::Release()
{
	GJHDirectBuffer::ConstBufferPoolRelease();

	if (m_BackBuffer != nullptr)
	{
		m_BackBuffer = nullptr;
	}

	if (m_pSwapChain != nullptr)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}

	if (m_pContext != nullptr)
	{
		m_pContext->Release();
		m_pContext = nullptr;
	}

	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

void GJHGameEngineDevice::Init(HWND _Hwnd, float4 _Size)
{
	if (_Hwnd == nullptr)
	{
		MsgAssert("if (_Hwnd == nullptr)");
	}

	m_Size = _Size;
	m_Hwnd = _Hwnd;

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (S_OK != D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		iFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&eLV,
		&m_pContext
	))
	{
		MsgAssert("����̽� �ʱ�ȭ�� �߸��Ǿ����ϴ�.");
	}

	m_MC = 4;

	if (m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R32G32B32A32_UINT, m_MC, &m_MQ) != S_OK)
	{
		m_MC = 1;
		if (m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UINT, m_MC, &m_MQ) != S_OK)
		{
			MsgAssert("��Ƽ���ø� üũ�� ����� ���� �ʾҽ��ϴ�.");
		}
	}

	CreateSwapChain();
	CreateDeviceRenderTarget();

	MeshInit();
	ShaderInit();
	RasterizerInit();
	DepthStencilInit();
	SamplerInit();
	BlendInit();
	MaterialInit();
	ResourceLoading();
	GJHDirectRenderTarget::Init();
}

void GJHGameEngineDevice::VtxShaderReset()
{
	GetContext()->VSSetShader(nullptr, nullptr, 0);
}

void GJHGameEngineDevice::PixShaderReset()
{
	GetContext()->PSSetShader(nullptr, nullptr, 0);
}

void GJHGameEngineDevice::GeoShaderReset()
{
	GetContext()->GSSetShader(nullptr, nullptr, 0);
}

void GJHGameEngineDevice::ComShaderReset()
{
	GetContext()->CSSetShader(nullptr, nullptr, 0);
}

void GJHGameEngineDevice::ShaderResReset()
{
	ID3D11ShaderResourceView* const ShaderResourceViews[8] = { nullptr };
	GetContext()->VSSetShaderResources(0, 8, ShaderResourceViews);
	GetContext()->DSSetShaderResources(0, 8, ShaderResourceViews);
	GetContext()->GSSetShaderResources(0, 8, ShaderResourceViews);
	GetContext()->HSSetShaderResources(0, 8, ShaderResourceViews);
	GetContext()->PSSetShaderResources(0, 8, ShaderResourceViews);
}

void GJHGameEngineDevice::RasterizerReset()
{
	GetContext()->RSSetState(nullptr);
}

void GJHGameEngineDevice::DepthStencilReset()
{
	GetContext()->OMSetDepthStencilState(nullptr, 0);
}

void GJHGameEngineDevice::BlendReset()
{
	GetContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}


void GJHGameEngineDevice::Reset()
{
	VtxShaderReset();
	GetContext()->DSSetShader(nullptr, nullptr, 0);
	GeoShaderReset();
	GetContext()->HSSetShader(nullptr, nullptr, 0);
	PixShaderReset();
	ComShaderReset();

	ShaderResReset();
	RasterizerReset();
	DepthStencilReset();
	BlendReset();
	GJHMaterial::GlobalResourcesReset();
}

void GJHGameEngineDevice::CreateDeviceRenderTarget()
{
	ID3D11Texture2D* BackBufferTexture2D = nullptr;

	if (m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTexture2D) != S_OK)
	{
		MsgAssert("if (m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTexture2D) != S_OK)");
	}

	m_BackBuffer = std::make_shared<GJHDirectRenderTarget>();
	m_BackBuffer->CreateRenderTarget(BackBufferTexture2D, float4::ZERO);
	m_BackBuffer->CreateDepthTarget(m_Size);
}

void GJHGameEngineDevice::CreateSwapChain()
{
	if (m_Size.IsZeroUnderX() == true
		|| m_Size.IsZeroUnderY() == true)
	{
		MsgAssert("����̽� ���� ũ�Ⱑ �߸��Ǿ����ϴ�.");
	}

	DXGI_SWAP_CHAIN_DESC ScInfo = { 0, };

	ScInfo.BufferDesc.Width = m_Size.uix();
	ScInfo.BufferDesc.Height = m_Size.uiy();

	ScInfo.BufferDesc.RefreshRate.Numerator = 60;
	ScInfo.BufferDesc.RefreshRate.Denominator = 1;

	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	ScInfo.SampleDesc.Quality = 0;
	ScInfo.SampleDesc.Count = 1;
	ScInfo.OutputWindow = m_Hwnd;

	ScInfo.BufferCount = 2;

	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ScInfo.Windowed = true;

	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pD);
	if (pD == nullptr)
	{
		MsgAssert("if (pD == nullptr)");
	}

	pD->GetParent(__uuidof(IDXGIAdapter), (void**)&pA);
	if (pA == nullptr)
	{
		MsgAssert("if (pA == nullptr)");
	}

	pA->GetParent(__uuidof(IDXGIFactory), (void**)&pF);
	if (pF == nullptr)
	{
		MsgAssert("if (pF == nullptr)");
	}

	if (pF->CreateSwapChain(m_pDevice, &ScInfo, &m_pSwapChain) != S_OK)
	{
		MsgAssert("if (pF->CreateSwapChain(m_pDevice, &ScInfo, &m_pSwapChain) != S_OK)");
	}

	pF->Release();
	pA->Release();
	pD->Release();
}

void GJHGameEngineDevice::BackBufferSetting()
{
	m_BackBuffer->Clear();

	m_BackBuffer->Setting();
}

void GJHGameEngineDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

void GJHGameEngineDevice::ClearRenderingPipeLine()
{
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
	m_pContext->DSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);

	ID3D11RenderTargetView* m_pArrTargetView[8] = { nullptr };

	GJHGameEngineDevice::GetContext()->OMSetRenderTargets(1, &m_pArrTargetView[0], nullptr);
}

void GJHGameEngineDevice::ResourceLoading()
{
	GJHGameEngineDirectory NewDir;
	NewDir.MoveUp();
	NewDir.Move(L"EngineRes");
	auto Arr = NewDir.AllFile(L"*.png");

	for (size_t i = 0; i < Arr.size(); i++)
	{
		GJHDirectTexture2D::Load(Arr[i].FullPath());
	}
}

GJHGameEngineDevice::GJHGameEngineDevice()
{

}

GJHGameEngineDevice::~GJHGameEngineDevice()
{

}