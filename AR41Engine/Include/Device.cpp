#include "Device.h"
DEFINITION_SINGLE(CDevice)

CDevice::CDevice() 
	: m_Device(nullptr)
	, m_Context(nullptr)
	, m_SwapChain(nullptr)
	, m_TargetView(nullptr)
	, m_DepthView(nullptr)
	, m_RS{}
	, m_hWnd(0)
	, m_2DTarget(nullptr)
	, m_2DTargetWorld(nullptr)
	, m_2DFactory(nullptr)
	, m_SampleCount(1)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_2DTarget);
	SAFE_RELEASE(m_2DTargetWorld);
	SAFE_RELEASE(m_2DFactory);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_SwapChain);
	if (m_Context)
	{
		m_Context->ClearState();
	}
	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);
}

Vector2 CDevice::GetResolutionRatio() const
{
	Vector2 clientRS = GetClientResolution();
	return Vector2(m_RS.width / clientRS.x, m_RS.height / clientRS.y);
}

Vector2 CDevice::GetClientResolution() const
{
	RECT	windowRC;
	GetClientRect(m_hWnd, &windowRC);
	float width = (float)(windowRC.right - windowRC.left);
	float height = (float)(windowRC.bottom - windowRC.top);
	return Vector2(width, height);
}

bool CDevice::Init(HWND hWnd, unsigned int deviceWidth, unsigned int deviceHeight, bool windowMode)
{
	m_hWnd = hWnd;
	m_RS.width = deviceWidth;
	m_RS.height = deviceHeight;
	GetResolutionRatio();
	unsigned int	flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	D3D_FEATURE_LEVEL	fLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL	fLevelResult;
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, &fLevel, 1, D3D11_SDK_VERSION,
		&m_Device, &fLevelResult, &m_Context)))
	{
		return false;
	}
	m_SampleCount = 4;
	UINT	check = 0;
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &check);
	if (check < 1)
	{
		m_SampleCount = 1;
	}
	DXGI_SWAP_CHAIN_DESC	swapDesc = {};
	swapDesc.BufferDesc.Width = deviceWidth;
	swapDesc.BufferDesc.Height = deviceHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SampleDesc.Count = m_SampleCount;
	swapDesc.Windowed = windowMode;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	IDXGIDevice* DXGIDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);
	IDXGIAdapter* adapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);
	IDXGIFactory* factory = nullptr;
	adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(factory->CreateSwapChain(m_Device, &swapDesc, &m_SwapChain)))
	{
		SAFE_RELEASE(factory);
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(DXGIDevice);
		return false;
	}
	SAFE_RELEASE(factory);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(DXGIDevice);
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		return false;
	}
	if (FAILED(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_TargetView)))
	{
		return false;
	}
	SAFE_RELEASE(backBuffer);
	D3D11_TEXTURE2D_DESC	depthDesc = {};
	depthDesc.Width = deviceWidth;
	depthDesc.Height =deviceHeight;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = m_SampleCount;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.MipLevels = 1;
	ID3D11Texture2D* depthBuffer = nullptr;
	if (FAILED(m_Device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer)))
	{
		return false;
	}
	if (FAILED(m_Device->CreateDepthStencilView(depthBuffer, nullptr, &m_DepthView)))
	{
		return false;
	}
	SAFE_RELEASE(depthBuffer);
	D3D11_VIEWPORT	VP = {};
	VP.Width = (float)deviceWidth;
	VP.Height = (float)deviceHeight;
	VP.MaxDepth = 1.f;
	m_Context->RSSetViewports(1, &VP);
	// 2D Factory 생성
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
	{
		return false;
	}
	IDXGISurface* backSurface = nullptr;	// 3D BackBuffer의 Surface를 얻어온다.
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backSurface));
	// 2D용 렌더타겟을 만들어준다 출력하는 BackBuffer를 3D BackBuffer로 지정해주어서 출력한다
	D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	assert(backSurface!=0);
	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(backSurface, props, &m_2DTarget)))
	{
		return false;
	}
	SAFE_RELEASE(backSurface);
	return true;
}

void CDevice::ClearRenderTarget(float clearColor[4])
{
	m_Context->ClearRenderTargetView(m_TargetView, clearColor);
}

void CDevice::ClearDepthStencil(float depth, unsigned char stencil)
{
	m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,depth, stencil);
}

void CDevice::RenderStart()
{
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	m_SwapChain->Present(0, 0);
}
