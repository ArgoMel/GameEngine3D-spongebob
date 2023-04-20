#pragma once
#include "../EngineInfo.h"
#include "../Component/PrimitiveComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Shader/InstancingConstantBuffer.h"

class CRenderInstancing
{
private:
	friend class CRenderManager;
	class CMesh* m_Key;	//메쉬의 메모리 주소
	CStructuredBuffer* m_Buffer;
	CInstancingConstantBuffer* m_CBuffer;
	CSharedPtr<CShader>	m_InstancingShader;
	std::string	m_LayerName;
	int		m_PoolIndex;
	int		m_BufferCount;
	std::list<CPrimitiveComponent*>	m_RenderList;
	CRenderInstancing();
	~CRenderInstancing();
public:
	class CMesh* GetKey()	const
	{
		return m_Key;
	}
	int GetPoolIndex()	const
	{
		return m_PoolIndex;
	}
	int GetInstancingCount()	const
	{
		return (int)m_RenderList.size();
	}
	void AddRenderList(CPrimitiveComponent* component)
	{
		m_RenderList.push_back(component);
		component->SetInstanceID((int)(m_RenderList.size() - 1));
		if (m_RenderList.size() > (size_t)m_BufferCount)
		{
			m_BufferCount *= 2;
			m_Buffer->Init("InstancingBuffer", sizeof(InstancingBufferInfo), 
				m_BufferCount, 50, true, (int)EShaderBufferType::All);
		}
	}
	void Clear()
	{
		m_RenderList.clear();
	}
	void Render();
	void RenderShadowMap();
};

