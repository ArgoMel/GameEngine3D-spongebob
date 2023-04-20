#pragma once
#include "EngineInfo.h"

class CDevice
{
	DECLARE_SINGLE(CDevice)
private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_TargetView;
	ID3D11DepthStencilView* m_DepthView;
	HWND	m_hWnd;
	Resolution	m_RS;
	ID2D1RenderTarget* m_2DTarget;
	ID2D1RenderTarget* m_2DTargetWorld;
	ID2D1Factory* m_2DFactory;
	int		m_SampleCount;
public:
	ID3D11Device* GetDevice()	const
	{
		return m_Device;
	}
	ID3D11DeviceContext* GetContext()	const
	{
		return m_Context;
	}
	IDXGISwapChain* GetSwapChain()	const
	{
		return m_SwapChain;
	}
	Resolution GetResolution()	const
	{
		return m_RS;
	}
	Vector2 GetResolutionRatio()	const;
	Vector2 GetClientResolution()	const;
	ID2D1RenderTarget* Get2DTarget()	const
	{
		return m_2DTarget;
	}
	ID2D1RenderTarget* Get2DWorldTarget()	const
	{
		return m_2DTargetWorld;
	}
	ID2D1Factory* Get2DFactory()	const
	{
		return m_2DFactory;
	}
	int GetSampleCount()	const
	{
		return m_SampleCount;
	}
	bool Init(HWND hWnd, unsigned int deviceWidth, unsigned int deviceHeight,bool windowMode);
	void ClearRenderTarget(float clearColor[4]);
	void ClearDepthStencil(float depth, unsigned char stencil);
	void RenderStart();
	void Flip();
};

