#pragma once
#include "ConstantBufferData.h"

class CAnimationUpdateConstantBuffer : public CConstantBufferData
{
private:
	AnimationCBuffer	m_BufferData;
public:
	void SetBoneCount(int boneCount)
	{
		m_BufferData.boneCount = boneCount;
	}
	void SetCurrentFrame(int currentFrame)
	{
		m_BufferData.currentFrame = currentFrame;
	}
	void SetNextFrame(int nextFrame)
	{
		m_BufferData.nextFrame = nextFrame;
	}
	void SetRatio(float ratio)
	{
		m_BufferData.ratio = ratio;
	}
	void SetFrameCount(int frameCount)
	{
		m_BufferData.frameCount = frameCount;
	}
	void SetRowIndex(int rowIndex)
	{
		m_BufferData.rowIndex = rowIndex;
	}
	void SetChangeAnimation(bool change)
	{
		m_BufferData.changeAnimation = change ? 1 : 0;
	}
	void SetChangeRatio(float changeRatio)
	{
		m_BufferData.changeRatio = changeRatio;
	}
	void SetChangeFrameCount(int changeFrameCount)
	{
		m_BufferData.changeFrameCount = changeFrameCount;
	}
	CAnimationUpdateConstantBuffer();
	CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& buffer);
	virtual ~CAnimationUpdateConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CAnimationUpdateConstantBuffer* Clone();
};

