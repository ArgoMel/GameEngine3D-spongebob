#pragma once
#include "ConstantBufferData.h"

class CTileMapConstantBuffer : public CConstantBufferData
{
private:
	TileMapCBuffer	m_BufferData;
public:
	CTileMapConstantBuffer();
	CTileMapConstantBuffer(const CTileMapConstantBuffer& buffer);
	virtual ~CTileMapConstantBuffer();
	const Vector2& GetStartFrame()	const
	{
		return m_BufferData.start;
	}
	const Vector2& GetEndFrame()	const
	{
		return m_BufferData.end;
	}
	void SetImageSize(const Vector2& size)
	{
		m_BufferData.imageSize = size;
	}
	void SetStart(const Vector2& start)
	{
		m_BufferData.start = start;
	}
	void SetEnd(const Vector2& end)
	{
		m_BufferData.end = end;
	}
	void SetTileSize(const Vector2& size)
	{
		m_BufferData.tileSize = size;
	}
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
		m_BufferData.matWVP.Transpose();
	}
	void SetFrame(int frame)
	{
		m_BufferData.frame = frame;
	}
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTileMapConstantBuffer* Clone();
};

