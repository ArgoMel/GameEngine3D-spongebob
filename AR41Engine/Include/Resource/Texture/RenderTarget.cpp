#include "RenderTarget.h"
#include "../ResourceManager.h"
#include "../Mesh/Mesh.h"
#include "../Shader/GlobalConstantBuffer.h"
#include "../../Device.h"
#include "../../Engine.h"

CRenderTarget::CRenderTarget()
	: m_TargetView(nullptr)
	, m_TargetTex(nullptr)
	, m_DepthView(nullptr)
	, m_PrevTargetView(nullptr)
	, m_PrevDepthView(nullptr)
	, m_Surface(nullptr)
	, m_ClearColor{}
	, m_DebugRender(true)
	, m_Scale(100.f, 100.f, 1.f)
{
	m_CBuffer = new CTransformConstantBuffer;
	m_ImageType = EImageType::RenderTarget;
}

CRenderTarget::~CRenderTarget()
{
	SAFE_DELETE(m_CBuffer);
	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_TargetTex);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDepthView);
}

bool CRenderTarget::CreateTarget(const std::string& name, unsigned int width, unsigned int height, 
	DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat)
{
	SetName(name);
	int	sampleCount = CDevice::GetInst()->GetSampleCount();
// Target侩 Texture 积己
	D3D11_TEXTURE2D_DESC	desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = sampleCount;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = pixelFormat;
	desc.Usage = D3D11_USAGE_DEFAULT;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&desc, nullptr, &m_TargetTex)))
	{
		return false;
	}
	TextureResourceInfo* info = new TextureResourceInfo;
	info->width = width;
	info->height = height;
	m_vecTextureInfo.push_back(info);
	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_TargetTex, nullptr, &info->SRV)))
	{
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex, nullptr, &m_TargetView)))
	{
		return false;
	}
	if (depthFormat != DXGI_FORMAT_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC	depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.ArraySize = 1;
		depthDesc.MipLevels = 1;
		depthDesc.SampleDesc.Count = sampleCount;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.Format = depthFormat;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		ID3D11Texture2D* depthTex = nullptr;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&depthDesc, nullptr, &depthTex)))
		{
			return false;
		}
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(depthTex, nullptr, &m_DepthView)))
		{
			return false;
		}
		SAFE_RELEASE(depthTex);
	}
	m_CBuffer->Init();
	m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)width, 0.f,(float)height, 0.f, 1000.f);
	m_CBuffer->SetProjMatrix(m_matProj);
	m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");
	return true;
}

bool CRenderTarget::CreateTargetNoMS(const std::string& name, unsigned int width, unsigned int height,
	DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat)
{
	SetName(name);
	//int	sampleCount = CDevice::GetInst()->GetSampleCount(); 
	// Target侩 Texture 积己
	D3D11_TEXTURE2D_DESC	desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;//
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = pixelFormat;
	desc.Usage = D3D11_USAGE_DEFAULT;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&desc, nullptr, &m_TargetTex)))
	{
		return false;
	}
	TextureResourceInfo* info = new TextureResourceInfo;
	info->width = width;
	info->height = height;
	m_vecTextureInfo.push_back(info);
	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_TargetTex, nullptr, &info->SRV)))
	{
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex, nullptr, &m_TargetView)))
	{
		return false;
	}
	if (depthFormat != DXGI_FORMAT_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC	depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.ArraySize = 1;
		depthDesc.MipLevels = 1;
		depthDesc.SampleDesc.Count = 1;//
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.Format = depthFormat;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		ID3D11Texture2D* depthTex = nullptr;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&depthDesc, nullptr, &depthTex)))
		{
			return false;
		}
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(depthTex, nullptr, &m_DepthView)))
		{
			return false;
		}
		SAFE_RELEASE(depthTex);
	}
	m_CBuffer->Init();
	m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)width, 0.f,(float)height, 0.f, 1000.f);
	m_CBuffer->SetProjMatrix(m_matProj);
	m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");
	return true;
}

void CRenderTarget::ClearTarget()
{
	CDevice::GetInst()->GetContext()->ClearRenderTargetView(m_TargetView, m_ClearColor);
	if (m_DepthView)
	{
		CDevice::GetInst()->GetContext()->ClearDepthStencilView(m_DepthView, 
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* depthView)
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDepthView);
	ID3D11DepthStencilView* depth = m_PrevDepthView;
	if (depthView)
	{
		depth = depthView;
	}
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, depth);
}

void CRenderTarget::SetTarget()
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDepthView);
	ID3D11DepthStencilView* depth = m_PrevDepthView;
	if (m_DepthView)
	{
		depth = m_DepthView;
	}
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, depth);
}

void CRenderTarget::ResetTarget()
{
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_PrevTargetView, m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDepthView);
}
// Debug侩
void CRenderTarget::SetTargetShader()
{
	CDevice::GetInst()->GetContext()->PSSetShaderResources(10, 1, &m_vecTextureInfo[0]->SRV);
}

void CRenderTarget::ResetTargetShader()
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->PSSetShaderResources(10, 1, &SRV);
}

void CRenderTarget::SetTargetShader(int reg)
{
	CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &m_vecTextureInfo[0]->SRV);
}

void CRenderTarget::ResetTargetShader(int reg)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &SRV);
}

void CRenderTarget::Render()
{
	m_CBuffer->SetWorldMatrix(m_matDebugWVP);
	m_CBuffer->SetProjMatrix(m_matProj);
	m_CBuffer->UpdateBuffer();
	CGlobalConstantBuffer* buffer = CEngine::GetInst()->GetGlobalCBuffer();
	buffer->SetShadowMapResolution((float)GetWidth(), (float)GetHeight());
	buffer->UpdateBuffer();
	SetTargetShader();
	m_Mesh->Render();
	ResetTargetShader();
}
