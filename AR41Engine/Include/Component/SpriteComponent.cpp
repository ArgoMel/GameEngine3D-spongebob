#include "SpriteComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"

CSpriteComponent::CSpriteComponent()
{
	SetTypeID<CSpriteComponent>();
	m_ComponentTypeName = "SpriteComponent";
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& component)
	: CPrimitiveComponent(component)
{
	if (component.m_Animation)
	{
		m_Animation = component.m_Animation->Clone();
	}
}

CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::GetAnimationNames(std::vector<std::string>& vecNames)
{
	if (m_Animation)
	{
		m_Animation->GetAnimationNames(vecNames);
	}
}

bool CSpriteComponent::SetTexture(CTexture* texture)
{
	if (m_vecMaterial.empty())
	{
		return false;
	}
	m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel,texture->GetName(), texture);
	return true;
}

bool CSpriteComponent::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_vecMaterial.empty())
	{
		return false;
	}
	m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel, name, fileName, pathName);
	return true;
}

bool CSpriteComponent::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (m_vecMaterial.empty())
	{
		return false;
	}
	m_vecMaterial[0]->SetTextureFullPath(0, 0, (int)EShaderBufferType::Pixel, name, fullPath);
	return true;
}

bool CSpriteComponent::SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	if (m_vecMaterial.empty())
	{
		return false;
	}
	m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel, name, vecFileName, pathName);
	return true;
}

bool CSpriteComponent::SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (m_vecMaterial.empty())
	{
		return false;
	}
	m_vecMaterial[0]->SetTextureFullPath(0, 0, (int)EShaderBufferType::Pixel, name, vecFullPath);
	return true;
}

void CSpriteComponent::SetTextureFrameIndex(int index)
{
	m_vecMaterial[0]->SetTextureFrameIndex(0, index);
}

CTexture* CSpriteComponent::GetTexture(int index) const
{
	if (m_vecMaterial.empty())
	{
		return nullptr;
	}
	return m_vecMaterial[0]->GetTexture(index);
}

void CSpriteComponent::ClearAnimation()
{
	m_Animation = nullptr;
	SetTextureFrameIndex(0);
}

void CSpriteComponent::Start()
{
	CPrimitiveComponent::Start();
	if (m_Animation)
	{
		m_Animation->Start();
	}
}

bool CSpriteComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	SetMesh("LBUVRect");
	m_Transform->Set2D(true);
	return true;
}

void CSpriteComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	if (m_Animation)
	{
		m_Animation->Update(deltaTime);
	}
}

void CSpriteComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CSpriteComponent::Render()
{
	if (m_Animation)
	{
		m_Animation->SetShader();
	}
	else
	{
		CAnimation2DConstantBuffer* buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();
		buffer->SetAnim2DEnable(false);
		buffer->UpdateBuffer();
	}
	CPrimitiveComponent::Render();
}

CSpriteComponent* CSpriteComponent::Clone() const
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
	bool	animation = false;
	if (m_Animation)
	{
		animation = true;
	}
	fwrite(&animation, 1, 1, file);
	if (m_Animation)
	{
		int	length = (int)m_Animation->m_ClassName.length();
		fwrite(&length, 4, 1, file);
		fwrite(m_Animation->m_ClassName.c_str(), 1, length, file);
		m_Animation->Save(file);
	}
}

void CSpriteComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
	bool	animation = false;
	fread(&animation, 1, 1, file);
	if (animation)
	{
		int	length = 0;
		char	className[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(className, 1, length, file);
		CAnimation2D* CDO = CAnimation2D::FindCDO(className);
		m_Animation = CDO->Clone();
		m_Animation->m_Owner = this;
		m_Animation->Load(file);
	}
}
