#pragma once
#include "ConstantBufferData.h"

class CUIConstantBuffer : public CConstantBufferData
{
private:
	UICBuffer	m_BufferData;
public:
	void SetTint(const Vector4& color)
	{
		m_BufferData.UITint = color;
	}
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.UIWVP = matWVP;
		m_BufferData.UIWVP.Transpose();
	}
	void SetMeshSize(const Vector2& size)
	{
		m_BufferData.UIMeshSize = size;
	}
	void SetPivot(const Vector2& pivot)
	{
		m_BufferData.UIPivot = pivot;
	}
	void SetTextureEnable(bool enable)
	{
		m_BufferData.UITextureEnable = enable ? 1 : 0;
	}
	void SetOpacity(float opacity)
	{
		m_BufferData.UIOpacity = opacity;
	}
	CUIConstantBuffer();
	CUIConstantBuffer(const CUIConstantBuffer& buffer);
	virtual ~CUIConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CUIConstantBuffer* Clone();
};

