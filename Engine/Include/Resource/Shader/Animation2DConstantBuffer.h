#pragma once
#include "ConstantBufferData.h"

class CAnimation2DConstantBuffer : public CConstantBufferData
{
private:
	Animation2DCBuffer	m_BufferData;
public:
	CAnimation2DConstantBuffer();
	CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer);
	virtual ~CAnimation2DConstantBuffer();
	void SetImageSize(float width, float height)
	{
		m_BufferData.anim2DImageWidth = width;
		m_BufferData.anim2DImageHeight = height;
	}
	void SetImageFrame(const Vector2& start, const Vector2& end)
	{
		m_BufferData.anim2DFrameStart = start;
		m_BufferData.anim2DFrameEnd = end;
	}
	void SetImageType(EAnimation2DType type)
	{
		m_BufferData.anim2DType = (int)type;
	}
	void SetAnim2DEnable(bool enable)
	{
		m_BufferData.anim2DEnable = enable ? 1 : 0;
	}
	void SetFrame(int frame)
	{
		m_BufferData.anim2DFrame = frame;
	}
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CAnimation2DConstantBuffer* Clone();

};

