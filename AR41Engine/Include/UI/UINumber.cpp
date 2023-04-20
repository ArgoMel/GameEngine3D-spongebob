#include "UINumber.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUINumber::CUINumber()
    : m_Space(5.f)
{
	m_WidgetTypeName = "UINumber";
}

CUINumber::CUINumber(const CUINumber& number)
	: CUIWidget(number)
    , m_Space(number.m_Space)
{
	m_TextureInfo = number.m_TextureInfo;
}

CUINumber::~CUINumber()
{
}

void CUINumber::SetTexture(CTexture* texture)
{
	m_TextureInfo.texture = texture;
}

bool CUINumber::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
        {
            return false;
        }
        m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
        {
            return false;
        }
        m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUINumber::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
        {
            return false;
        }
        m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
        {
            return false;
        }
        m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUINumber::SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
    const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
        {
            return false;
        }
        m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
        {
            return false;
        }
        m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUINumber::SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
        {
            return false;
        }
        m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
        {
            return false;
        }
        m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

void CUINumber::SetImageTint(const Vector4& tint)
{
    m_TextureInfo.tint = tint;
}

void CUINumber::SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_TextureInfo.tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUINumber::AddFrameData(const Vector2& start, const Vector2& end)
{
    Animation2DFrameData    frame;
    frame.start = start;
    frame.end = end;
    m_TextureInfo.vecFrameData.push_back(frame);
    m_TextureInfo.frameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.playTime;
}

void CUINumber::SetPlayTime(float playTime)
{
    m_TextureInfo.playTime = playTime;
    m_TextureInfo.frameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.playTime;
}

void CUINumber::SetPlayScale(float playScale)
{
    m_TextureInfo.playScale = playScale;
}

void CUINumber::Start()
{
    CUIWidget::Start();
}

bool CUINumber::Init()
{
    if (!CUIWidget::Init())
    {
        return false;
    }
    return true;
}

void CUINumber::Update(float deltaTime)
{
    CUIWidget::Update(deltaTime);
    if (m_Number == 0)
    {
        m_vecNumber.clear();
        m_vecNumber.push_back(0);
    }
    else
    {
        unsigned int number = m_Number;
        std::stack<int> numberStack;
        while (number > 0)
        {
            numberStack.push(number % 10);
            number /= 10;
        }
        m_vecNumber.clear();
        while (!numberStack.empty())
        {
            m_vecNumber.push_back(numberStack.top());
            numberStack.pop();
        }
    }
}

void CUINumber::PostUpdate(float deltaTime)
{
    CUIWidget::PostUpdate(deltaTime);
}

void CUINumber::Render()
{
    size_t  size = m_vecNumber.size();
    float   space = 0.f;
    for (size_t i = 0; i < size; ++i)
    {
        bool textureEnable = m_TextureInfo.texture ? true : false;
        m_CBuffer->SetTextureEnable(textureEnable);
        if (textureEnable)
        {
            if (!m_TextureInfo.vecFrameData.empty())
            {
                int textureFrame = 0;
                if (m_TextureInfo.texture->GetImageType() == EImageType::Frame)
                {
                    textureFrame = (int)m_vecNumber[i];
                }
                m_TextureInfo.texture->SetShader(0, (int)EShaderBufferType::Pixel, textureFrame);
                m_AnimCBuffer->SetAnim2DEnable(true);
                m_AnimCBuffer->SetFrame(m_vecNumber[i]);
                m_AnimCBuffer->SetImageFrame(m_TextureInfo.vecFrameData[m_vecNumber[i]].start,
                    m_TextureInfo.vecFrameData[m_vecNumber[i]].end);
                m_AnimCBuffer->SetImageSize((float)m_TextureInfo.texture->GetWidth(),
                    (float)m_TextureInfo.texture->GetHeight());
                m_AnimCBuffer->SetImageType((EAnimation2DType)m_TextureInfo.texture->GetImageType());
            }
            else
            {
                m_TextureInfo.texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
                m_AnimCBuffer->SetAnim2DEnable(false);
            }
        }
        m_Tint = m_TextureInfo.tint;
        m_RenderPos.x += space;
        space = m_Size.x + m_Space;
        CUIWidget::Render();
    }
}

CUINumber* CUINumber::Clone()
{
    return new CUINumber(*this);
}

void CUINumber::Save(FILE* file)
{
    CUIWidget::Save(file);
    fwrite(&m_TextureInfo.tint, sizeof(Vector4), 1, file);
    fwrite(&m_TextureInfo.frameTime, sizeof(float), 1, file);
    fwrite(&m_TextureInfo.playTime, sizeof(float), 1, file);
    fwrite(&m_TextureInfo.playScale, sizeof(float), 1, file);
    int frameCount = (int)m_TextureInfo.vecFrameData.size();
    fwrite(&frameCount, sizeof(int), 1, file);
    if (frameCount > 0)
    {
        fwrite(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
    }
    bool    texEnable = false;
    if (m_TextureInfo.texture)
    {
        texEnable = true;
    }
    fwrite(&texEnable, sizeof(bool), 1, file);
    if (texEnable)
    {
        std::string texName = m_TextureInfo.texture->GetName();
        int length = (int)texName.length();
        fwrite(&length, sizeof(int), 1, file);
        fwrite(texName.c_str(), 1, length, file);
        m_TextureInfo.texture->Save(file);
    }
    fwrite(&m_Number, sizeof(unsigned int), 1, file);
    fwrite(&m_Space, sizeof(float), 1, file);
}

void CUINumber::Load(FILE* file)
{
    CUIWidget::Load(file);
    fread(&m_TextureInfo.tint, sizeof(Vector4), 1, file);
    fread(&m_TextureInfo.frameTime, sizeof(float), 1, file);
    fread(&m_TextureInfo.playTime, sizeof(float), 1, file);
    fread(&m_TextureInfo.playScale, sizeof(float), 1, file);
    int frameCount = 0;
    fread(&frameCount, sizeof(int), 1, file);
    m_TextureInfo.vecFrameData.resize((size_t)frameCount);
    if (frameCount > 0)
    {
        fread(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
    }
    bool    texEnable = false;
    fread(&texEnable, sizeof(bool), 1, file);
    if (texEnable)
    {
        char    texName[256] = {};
        int length = 0;
        fread(&length, sizeof(int), 1, file);
        assert(length >= 0);
        fread(texName, 1, length, file);
        EImageType  imageType;
        fread(&imageType, sizeof(EImageType), 1, file);
        int	textureSRVCount = 0;
        fread(&textureSRVCount, sizeof(int), 1, file);
        if (textureSRVCount == 1)
        {
            TCHAR	fileName[MAX_PATH] = {};
            char	pathName[MAX_PATH] = {};
            fread(fileName, sizeof(TCHAR), MAX_PATH, file);
            fread(pathName, sizeof(char), MAX_PATH, file);
            if (m_Scene)
            {
                m_Scene->GetResource()->LoadTexture(texName, fileName, pathName);
                m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
            }
            else
            {
                CResourceManager::GetInst()->LoadTexture(texName, fileName, pathName);
                m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
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
                    m_Scene->GetResource()->LoadTexture(texName, vecFileName, resultPathName);
                    m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
                }
                else
                {
                    CResourceManager::GetInst()->LoadTexture(texName, vecFileName, resultPathName);
                    m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
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
                    m_Scene->GetResource()->LoadTextureArray(texName, vecFileName, resultPathName);
                    m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
                }
                else
                {
                    CResourceManager::GetInst()->LoadTextureArray(texName, vecFileName, resultPathName);
                    m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
                }
                for (int i = 0; i < textureSRVCount; ++i)
                {
                    SAFE_DELETE_ARRAY(vecFileName[i]);
                }
            }
        }
    }
    fread(&m_Number, sizeof(unsigned int), 1, file);
    fread(&m_Space, sizeof(float), 1, file);
}
