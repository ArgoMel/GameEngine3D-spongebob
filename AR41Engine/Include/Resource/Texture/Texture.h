#pragma once
#include "../../Ref.h"

struct TextureResourceInfo
{
	DirectX::ScratchImage* image;
	ID3D11ShaderResourceView* SRV;
	unsigned int	width;
	unsigned int	height;
	TCHAR* fileName;
	char* pathName;

	TextureResourceInfo() 
		: image(nullptr)
		, SRV(nullptr)
		, width(0)
		, height(0)
	{
		fileName = new TCHAR[MAX_PATH];
		pathName = new char[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		memset(pathName, 0, sizeof(char) * MAX_PATH);
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(fileName);
		SAFE_DELETE_ARRAY(pathName);
		SAFE_DELETE(image);
	}
};

class CTexture : public CRef
{
private:
	friend class CTextureManager;
	bool CreateResource(int index);
	bool CreateResourceArray();
protected:
	class CScene* m_Scene;
	ID3D11ShaderResourceView* m_ArraySRV;
	EImageType	m_ImageType;
	std::vector<TextureResourceInfo*>	m_vecTextureInfo;
	CTexture();
	virtual ~CTexture();
public:
	ID3D11ShaderResourceView* GetResource(int index = 0)	const
	{
		return m_vecTextureInfo[index]->SRV;
	}
	unsigned int GetWidth(int index = 0)	const
	{
		return m_vecTextureInfo[index]->width;
	}
	unsigned int GetHeight(int index = 0)	const
	{
		return m_vecTextureInfo[index]->height;
	}
	TCHAR* GetFullPath(int index = 0)	const
	{
		return m_vecTextureInfo[index]->fileName;
	}
	char* GetPathName(int index = 0)	const
	{
		return m_vecTextureInfo[index]->pathName;
	}
	int GetImageCount()	const
	{
		return (int)m_vecTextureInfo.size();
	}
	EImageType GetImageType()	const
	{
		return m_ImageType;
	}
	void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	virtual void Render();
	void SetShader(int reg, int shaderBufferType, int index);
	void ResetShader(int reg, int shaderBufferType);
	void Save(FILE* file);
	void Load(FILE* file);
};

