#pragma once
#include "ConstantBufferData.h"
class CMaterialConstantBuffer : public CConstantBufferData
{
private:
	MaterialCBuffer	m_BufferData;
public:
	bool GetIsBump()
	{
		return m_BufferData.ambientColor.w;
	}
	bool GetIsSpecular()
	{
		return m_BufferData.baseColor.w;
	}
	bool GetIsEmissive()
	{
		return m_BufferData.emissiveColor.w;
	}
	void SetBaseColor(const Vector4& color)
	{
		m_BufferData.baseColor = color;
	}
	void SetAmbientColor(const Vector4& color)
	{
		m_BufferData.ambientColor = color;
	}
	void SetSpecularColor(const Vector4& color)
	{
		m_BufferData.specularColor = color;
	}
	void SetEmissiveColor(const Vector4& color)
	{
		m_BufferData.emissiveColor = color;
	}
	void SetOpacity(float opacity)
	{
		m_BufferData.opacity = opacity;
	}
	void SetEnableBump(bool enable)
	{
		m_BufferData.ambientColor.w = enable ? 1.f : 0.f;
	}
	void SetEnableSpecular(bool enable)
	{
		m_BufferData.baseColor.w = enable ? 1.f : 0.f;
	}
	void SetEnableEmissive(bool enable)
	{
		m_BufferData.emissiveColor.w = enable ? 1.f : 0.f;
	}
	void SetAnimation3D(bool enable)
	{
		m_BufferData.animation3DEnable = enable ? 1 : 0;
	}
	void SetImageType(EImageType type)
	{
		m_BufferData.textureType = (int)type;
	}
	void SetTextureWidth(float width)
	{
		m_BufferData.textureWidth = width;
	}
	void SetTextureHeight(float height)
	{
		m_BufferData.textureHeight = height;
	}
	void SetReceiveDecal(bool receive)
	{
		m_BufferData.receiveDecal = receive ? 1 : 0;
	}
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer);
	virtual ~CMaterialConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CMaterialConstantBuffer* Clone();
};

