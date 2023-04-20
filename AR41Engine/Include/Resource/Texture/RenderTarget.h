#pragma once
#include "Texture.h"
#include "../Shader/TransformConstantBuffer.h"

class CRenderTarget : public CTexture
{
protected:
	friend class CTextureManager;
	CRenderTarget();
	virtual ~CRenderTarget();
	ID3D11RenderTargetView* m_TargetView;
	ID3D11Texture2D* m_TargetTex;
	ID3D11DepthStencilView* m_DepthView;
	ID3D11RenderTargetView* m_PrevTargetView;
	ID3D11DepthStencilView* m_PrevDepthView;
	IDXGISurface* m_Surface;
	CTransformConstantBuffer* m_CBuffer;
	CSharedPtr<class CMesh>	m_Mesh;
	Matrix		m_matProj;
	Matrix		m_matDebugWVP;
	Vector3		m_Scale;
	Vector3		m_Pos;
	float		m_ClearColor[4];
	bool		m_DebugRender;
public:
	ID3D11RenderTargetView* GetTargetView()	const
	{
		return m_TargetView;
	}
	ID3D11DepthStencilView* GetDepthView()	const
	{
		return m_DepthView;
	}
	IDXGISurface* GetSurface()	const
	{
		return m_Surface;
	}
	Matrix GetWVP()	const
	{
		return m_matDebugWVP;
	}
	bool GetDebugRender()	const
	{
		return m_DebugRender;
	}
	void SetDebugRender(bool render)
	{
		m_DebugRender = render;
	}
	void SetPos(const Vector3& pos)
	{
		m_Pos = pos;
		Matrix	matScale;
		Matrix	matPos;
		matScale.Scaling(m_Scale);
		matPos.Translation(m_Pos);
		m_matDebugWVP = matScale * matPos;
	}
	void SetScale(const Vector3& scale)
	{
		m_Scale = scale;
		Matrix	matScale;
		Matrix	matPos;
		matScale.Scaling(m_Scale);
		matPos.Translation(m_Pos);
		m_matDebugWVP = matScale * matPos;
	}
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, 
		DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);
	bool CreateTargetNoMS(const std::string& name, unsigned int width, unsigned int height, 
		DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);
	void ClearTarget();
	void SetTarget(ID3D11DepthStencilView* depthView);
	void SetTarget();
	void ResetTarget();
	void SetTargetShader();
	void ResetTargetShader();
	void SetTargetShader(int reg);
	void ResetTargetShader(int reg);
	virtual void Render();
};

