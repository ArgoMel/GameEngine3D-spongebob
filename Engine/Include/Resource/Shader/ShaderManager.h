#pragma once
#include "../../EngineInfo.h"

class CShaderManager
{
private:
	friend class CResourceManager;
	class CColliderConstantBuffer* m_ColliderCBuffer;
	std::unordered_map<std::string, CSharedPtr<class CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CShader>>	m_mapGlobalShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>>	m_mapCBuffer;
	CShaderManager();
	~CShaderManager();
public:
	class CColliderConstantBuffer* GetColliderCBuffer()	const
	{
		return m_ColliderCBuffer;
	}
	bool Init();
	class CShader* FindShader(const std::string& name);
	void ReleaseShader(const std::string& name);
	bool CreateConstantBuffer(const std::string& name, int size, int reg, 
		int shaderBufferType = (int)EShaderBufferType::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& name);
	template <typename T>
	bool CreateShader(const std::string& name, bool globalShader = false)
	{
		T* shader = (T*)FindShader(name);
		if (shader)
		{
			return true;
		}
		shader = new T;
		shader->SetName(name);
		if (!shader->Init())
		{
			SAFE_DELETE(shader);
			return false;
		}
		m_mapShader.insert(std::make_pair(name, shader));
		if (globalShader)
		{
			m_mapGlobalShader.insert(std::make_pair(name, shader));
		}
		return true;
	}
};

