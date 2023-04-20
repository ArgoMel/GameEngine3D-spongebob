#pragma once
#include "../../EngineInfo.h"

class CTextureManager
{
private:
	friend class CResourceManager;
	CSharedPtr<class CShader>	m_DebugShader;
	std::unordered_map<std::string, CSharedPtr<class CTexture>>	m_mapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*>	m_mapSampler;
	CTextureManager();
	~CTextureManager();
	bool CreateSampler(const std::string& name, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU,
		D3D11_TEXTURE_ADDRESS_MODE addressV, D3D11_TEXTURE_ADDRESS_MODE addressW, float borderColor[4]);
	ID3D11SamplerState* FindSampler(const std::string& name);
	void SetSampler(const std::string& name, int reg, int shaderBufferType = (int)EShaderBufferType::All);
public:
	bool Init();
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, 
		DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);
	bool CreateTargetNoMS(const std::string& name, unsigned int width, unsigned int height, 
		DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);
	void Render();
	class CTexture* FindTexture(const std::string& name);
	void ReleaseTexture(const std::string& name);
};

