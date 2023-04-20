#include "Material.h"
#include "../ResourceManager.h"
#include "../Shader/MaterialConstantBuffer.h"
#include "../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Render/RenderManager.h"

CMaterial::CMaterial()
	: m_BaseColor(Vector4::white)
	, m_AmbientColor(0.2f, 0.2f, 0.2f, 1.f)
	, m_SpecularColor(Vector4::white)
	, m_EmissiveColor(Vector4::black)
	, m_Opacity(1.f)
	, m_Scene(nullptr)
	, m_Animation3D(false)
	, m_ReceiveDecal(true)
{
	SetTypeID<CMaterial>();
	m_CBuffer = new CMaterialConstantBuffer;
	m_CBuffer->Init();
	m_CBuffer->SetBaseColor(m_BaseColor);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
	m_CBuffer->SetOpacity(m_Opacity);
}

CMaterial::CMaterial(const CMaterial& material)
	: CRef(material)
{
	m_ReceiveDecal = material.m_ReceiveDecal;
	m_Shader = material.m_Shader;
	m_BaseColor = material.m_BaseColor;
	m_AmbientColor = material.m_AmbientColor;
	m_SpecularColor = material.m_SpecularColor;
	m_EmissiveColor = material.m_EmissiveColor;
	m_Opacity = material.m_Opacity;
	m_Animation3D = material.m_Animation3D;
	m_CBuffer = material.m_CBuffer->Clone();
	auto	iter = material.m_vecTextureInfo.begin();
	auto	iterEnd = material.m_vecTextureInfo.end();
	for (; iter != iterEnd; ++iter)
	{
		MaterialTextureInfo* info = new MaterialTextureInfo;
		info->name = (*iter)->name;
		info->samplerType = (*iter)->samplerType;
		info->texture = (*iter)->texture;
		info->reg = (*iter)->reg;
		info->shaderBufferType = (*iter)->shaderBufferType;
		m_vecTextureInfo.push_back(info);
	}
	m_Scene = nullptr;
	size_t size = std::size(m_RenderState);
	for (size_t i = 0; i < size; ++i)
	{
		if (material.m_RenderState[i])
		{
			m_RenderState[i] = material.m_RenderState[i];
		}
	}
}

CMaterial::~CMaterial()
{
	auto	iter = m_vecTextureInfo.begin();
	auto	iterEnd = m_vecTextureInfo.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	SAFE_DELETE(m_CBuffer);
}

CRenderState* CMaterial::GetRenderState(int index) const
{
	size_t size = std::size(m_RenderState);
	if ((int)size <= index)
	{
		return nullptr;
	}
	return m_RenderState[index];
}

CTexture* CMaterial::GetTexture(int index) const
{
	if ((int)m_vecTextureInfo.size() <= index)
	{
		return nullptr;
	}
	return m_vecTextureInfo[index]->texture;
}

void CMaterial::SetRenderState(const std::string& name)
{
	CRenderState* renderState = CRenderManager::GetInst()->FindRenderState<CRenderState>(name);
	if (!renderState)
	{
		return;
	}
	m_RenderState[(int)renderState->GetType()] = renderState;
}

void CMaterial::CopyInstancingData(InstancingBufferInfo& data) const
{
	data.baseColor = m_BaseColor;
	data.ambientColor = m_AmbientColor;
	data.specularColor = m_SpecularColor;
	data.emissiveColor = m_EmissiveColor;
	data.opacity = m_Opacity;
	data.animation3D = m_Animation3D ? 1 : 0;
	data.receiveDecal = m_ReceiveDecal ? 1 : 0;
}

void CMaterial::SetShader(const std::string& name)
{
	if (m_Scene)
	{
		m_Shader = m_Scene->GetResource()->FindShader(name);
	}
	else
	{
		m_Shader = CResourceManager::GetInst()->FindShader(name);
	}
}

void CMaterial::SetMaterial()
{
	if (m_Shader)
	{
		m_Shader->SetShader();
	}
	if (m_Animation3D)
	{
		m_ReceiveDecal = false;
		m_CBuffer->SetReceiveDecal(false);
	}
	size_t size=std::size(m_RenderState);
	for (int i = 0; i < size; ++i)
	{
		if (m_RenderState[i])
		{
			m_RenderState[i]->SetState();
		}
	}
	if (!m_vecTextureInfo.empty())
	{
		m_CBuffer->SetImageType(m_vecTextureInfo[0]->texture->GetImageType());
		m_CBuffer->SetTextureWidth((float)m_vecTextureInfo[0]->texture->GetWidth());
		m_CBuffer->SetTextureHeight((float)m_vecTextureInfo[0]->texture->GetHeight());
	}
	m_CBuffer->UpdateBuffer();
	size = m_vecTextureInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecTextureInfo[i]->texture->SetShader(m_vecTextureInfo[i]->reg, m_vecTextureInfo[i]->shaderBufferType
			, m_vecTextureInfo[i]->index);
	}
}

void CMaterial::ResetMaterial()
{
	size_t size = std::size(m_RenderState);
	for (int i = 0; i < size; ++i)
	{
		if (m_RenderState[i])
		{
			m_RenderState[i]->ResetState();
		}
	}
	size = m_vecTextureInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecTextureInfo[i]->texture->ResetShader(m_vecTextureInfo[i]->reg, m_vecTextureInfo[i]->shaderBufferType);
	}
}

void CMaterial::SetShadowMaterial()
{
	m_CBuffer->UpdateBuffer();
}

void CMaterial::SetInstancingMaterial()
{
	size_t size = std::size(m_RenderState);
	for (int i = 0; i < size; ++i)
	{
		if (m_RenderState[i])
		{
			m_RenderState[i]->SetState();
		}
	}
	size = m_vecTextureInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecTextureInfo[i]->texture->SetShader(m_vecTextureInfo[i]->reg, m_vecTextureInfo[i]->shaderBufferType, 
			m_vecTextureInfo[i]->index);
	}
}

void CMaterial::ResetInstancingMaterial()
{
	size_t size = std::size(m_RenderState);
	for (int i = 0; i < size; ++i)
	{
		if (m_RenderState[i])
		{
			m_RenderState[i]->ResetState();
		}
	}
	size = m_vecTextureInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecTextureInfo[i]->texture->ResetShader(m_vecTextureInfo[i]->reg, m_vecTextureInfo[i]->shaderBufferType);
	}
}

CMaterial* CMaterial::Clone() const
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE* file)
{
	CRef::Save(file);
	int length = (int)m_Shader->GetName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Shader->GetName().c_str(), 1, length, file);
	fwrite(&m_BaseColor, sizeof(Vector4), 1, file);
	fwrite(&m_AmbientColor, sizeof(Vector4), 1, file);
	fwrite(&m_SpecularColor, sizeof(Vector4), 1, file);
	fwrite(&m_EmissiveColor, sizeof(Vector4), 1, file);
	fwrite(&m_Opacity, 4, 1, file);
	fwrite(&m_Animation3D, 4, 1, file);
	for (int i = 0; i < std::size(m_RenderState); ++i)
	{
		bool	renderStateEnable = false;
		if (m_RenderState[i])
		{
			renderStateEnable = true;
		}
		fwrite(&renderStateEnable, 1, 1, file);
		if (renderStateEnable)
		{
			length = (int)m_RenderState[i]->GetName().length();
			fwrite(&length, 4, 1, file);
			fwrite(m_RenderState[i]->GetName().c_str(), 1, length, file);
		}
	}
	int	textureCount = (int)m_vecTextureInfo.size();
	fwrite(&textureCount, 4, 1, file);
	for (int i = 0; i < textureCount; ++i)
	{
		length = (int)m_vecTextureInfo[i]->name.length();
		fwrite(&length, 4, 1, file);
		fwrite(m_vecTextureInfo[i]->name.c_str(), 1, length, file);
		fwrite(&m_vecTextureInfo[i]->samplerType, sizeof(ESamplerType), 1, file);
		fwrite(&m_vecTextureInfo[i]->reg, sizeof(int), 1, file);
		fwrite(&m_vecTextureInfo[i]->shaderBufferType, sizeof(int), 1, file);
		m_vecTextureInfo[i]->texture->Save(file);
	}
}

void CMaterial::Load(FILE* file)
{
	CRef::Load(file);
	int length = 0;
	char	shaderName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(shaderName, 1, length, file);
	SetShader(shaderName);
	fread(&m_BaseColor, sizeof(Vector4), 1, file);
	fread(&m_AmbientColor, sizeof(Vector4), 1, file);
	fread(&m_SpecularColor, sizeof(Vector4), 1, file);
	fread(&m_EmissiveColor, sizeof(Vector4), 1, file);
	fread(&m_Opacity, 4, 1, file);
	fread(&m_Animation3D, 4, 1, file);
	m_CBuffer->SetBaseColor(m_BaseColor);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
	m_CBuffer->SetOpacity(m_Opacity);
	m_CBuffer->SetAnimation3D(m_Animation3D);
	size_t size = std::size(m_RenderState);
	for (size_t i = 0; i < size; ++i)
	{
		bool	renderStateEnable = false;
		fread(&renderStateEnable, 1, 1, file);
		if (renderStateEnable)
		{
			length = 0;
			char	renderStateName[256] = {};
			fread(&length, 4, 1, file);
			assert(length >= 0);
			fread(renderStateName, 1, length, file);
			SetRenderState(renderStateName);
		}
	}
	int	textureCount = 0;
	fread(&textureCount, 4, 1, file);
	for (size_t i = 0; i < m_vecTextureInfo.size(); ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
	m_vecTextureInfo.clear();
	for (int i = 0; i < textureCount; ++i)
	{
		MaterialTextureInfo* info = new MaterialTextureInfo;
		m_vecTextureInfo.push_back(info);
		length = 0;
		char	textureName[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(textureName, 1, length, file);
		info->name = textureName;
		fread(&info->samplerType, sizeof(ESamplerType), 1, file);
		fread(&info->reg, sizeof(int), 1, file);
		fread(&info->shaderBufferType, sizeof(int), 1, file);
		EImageType	imageType;
		fread(&imageType, sizeof(EImageType), 1, file);
		int	textureSRVCount = 0;
		fread(&textureSRVCount, sizeof(int), 1, file);
		TCHAR	folderName[8] = {};
		lstrcpy(folderName, TEXT("Texture"));
		if (textureSRVCount == 1)
		{
			TCHAR	fileName[MAX_PATH] = {};
			char	pathName[MAX_PATH] = {};
			fread(fileName, sizeof(TCHAR), MAX_PATH, file);
			fread(pathName, sizeof(char), MAX_PATH, file);
			if (m_Scene)
			{
				m_Scene->GetResource()->LoadTexture(info->name, fileName, pathName);
				info->texture = m_Scene->GetResource()->FindTexture(info->name);
			}
			else
			{
				CResourceManager::GetInst()->LoadTexture(info->name, fileName, pathName);
				info->texture = CResourceManager::GetInst()->FindTexture(info->name);
			}
		}
		else
		{
			if (imageType == EImageType::Frame)
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	resultPathName;
				for (int i = 0; i < textureSRVCount; ++i)
				{
					TCHAR* fileName = new TCHAR[MAX_PATH];
					char	pathName[MAX_PATH] = {};
					fread(fileName, sizeof(TCHAR), MAX_PATH, file);
					fread(pathName, sizeof(char), MAX_PATH, file);
					resultPathName = pathName;
					vecFileName.push_back(fileName);
				}
				if (m_Scene)
				{
					m_Scene->GetResource()->LoadTexture(info->name, vecFileName, resultPathName);
					info->texture = m_Scene->GetResource()->FindTexture(info->name);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(info->name, vecFileName, resultPathName);
					info->texture = CResourceManager::GetInst()->FindTexture(info->name);
				}
				for (int i = 0; i < textureSRVCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
			else
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	resultPathName;
				for (int i = 0; i <textureSRVCount; ++i)
				{
					TCHAR* fileName = new TCHAR[MAX_PATH];
					char	pathName[MAX_PATH] = {};
					fread(fileName, sizeof(TCHAR), MAX_PATH, file);
					fread(pathName, sizeof(char), MAX_PATH, file);
					resultPathName = pathName;
					vecFileName.push_back(fileName);
				}
				if (m_Scene)
				{
					m_Scene->GetResource()->LoadTextureArray(info->name, vecFileName, resultPathName);
					info->texture = m_Scene->GetResource()->FindTexture(info->name);
				}
				else
				{
					CResourceManager::GetInst()->LoadTextureArray(info->name, vecFileName, resultPathName);
					info->texture = CResourceManager::GetInst()->FindTexture(info->name);
				}
				for (int i = 0; i < textureSRVCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
		}
	}
}

void CMaterial::SetBaseColor(const Vector4& color)
{
	m_BaseColor = color;
	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);
	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_BaseColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetAmbientColor(const Vector4& color)
{
	m_AmbientColor = color;
	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = Vector4(r, g, b, a);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetAmbientColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_AmbientColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetSpecularColor(const Vector4& color)
{
	m_SpecularColor.x = color.x;
	m_SpecularColor.y = color.y;
	m_SpecularColor.z = color.z;
	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularColor(float r, float g, float b, float a)
{
	m_SpecularColor = Vector4(r, g, b, m_SpecularColor.w);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_SpecularColor = Vector4(r / 255.f, g / 255.f, b / 255.f, m_SpecularColor.w);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularPower(float power)
{
	m_SpecularColor.w = power;
	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetEmissiveColor(const Vector4& color)
{
	m_EmissiveColor = color;
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColor(float r, float g, float b, float a)
{
	m_EmissiveColor = Vector4(r, g, b, a);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_EmissiveColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
	m_CBuffer->SetOpacity(m_Opacity);
}

void CMaterial::AddOpacity(float opacity)
{
	m_Opacity += opacity;
	m_CBuffer->SetOpacity(m_Opacity);
}

bool CMaterial::GetIsBump()
{
	return m_CBuffer->GetIsBump();
}

bool CMaterial::GetIsSpecular()
{
	return m_CBuffer->GetIsSpecular();
}

bool CMaterial::GetIsEmissive()
{
	return m_CBuffer->GetIsEmissive();
}

bool CMaterial::GetIsAnimation3D()
{
	return m_Animation3D;
}

void CMaterial::EnableBump()
{
	m_CBuffer->SetEnableBump(true);
}

void CMaterial::EnableSpecular()
{
	m_CBuffer->SetEnableSpecular(true);
}

void CMaterial::EnableEmissive()
{
	m_CBuffer->SetEnableEmissive(true);
}

void CMaterial::EnableAnimation3D()
{
	m_Animation3D = true;
	m_CBuffer->SetAnimation3D(true);
}

void CMaterial::UnEnableBump()
{
	m_CBuffer->SetEnableBump(false);
}

void CMaterial::UnEnableSpecular()
{
	m_CBuffer->SetEnableSpecular(false);
}

void CMaterial::UnEnableEmissive()
{
	m_CBuffer->SetEnableEmissive(false);
}

void CMaterial::UnEnableAnimation3D()
{
	m_Animation3D = false;
	m_CBuffer->SetAnimation3D(false);
}

void CMaterial::SetReceiveDecal(bool receive)
{
	m_ReceiveDecal = receive;
	m_CBuffer->SetReceiveDecal(receive);
}

void CMaterial::AddTexture(int reg, int shaderBufferType, const std::string& name, CTexture* texture)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	info->texture = texture;
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTexture(int reg, int shaderBufferType, const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTextureFullPath(int reg, int shaderBufferType, const std::string& name, const TCHAR* fullPath)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTexture(int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTextureFullPath(int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTextureArray(int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::AddTextureArrayFullPath(int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* info = new MaterialTextureInfo;
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_vecTextureInfo.push_back(info);
}

void CMaterial::SetTexture(int index, int reg, int shaderBufferType, const std::string& name, CTexture* texture)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	info->texture = texture;
}

void CMaterial::SetTexture(int index, int reg, int shaderBufferType, const std::string& name, 
	const TCHAR* fileName, const std::string& pathName)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTextureFullPath(int index, int reg, int shaderBufferType, const std::string& name, 
	const TCHAR* fullPath)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTexture(int index, int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTextureFullPath(int index, int reg, int shaderBufferType, const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTextureArray(int index, int reg, int shaderBufferType, const std::string& name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTextureArrayFullPath(int index, int reg, int shaderBufferType, const std::string& name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->reg = reg;
	info->shaderBufferType = shaderBufferType;
	info->name = name;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		info->texture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CMaterial::SetTextureSamplerType(int index, ESamplerType type)
{
	if (index < 0 || index >= (int)m_vecTextureInfo.size())
	{
		return;
	}
	MaterialTextureInfo* info = m_vecTextureInfo[index];
	info->samplerType = type;
}

void CMaterial::SetTextureFrameIndex(int texIndex, int frameIndex)
{
	m_vecTextureInfo[texIndex]->index = frameIndex;
}
