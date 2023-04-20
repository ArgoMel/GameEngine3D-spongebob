#include "UIButton.h"
#include "../Input.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"


CUIButton::CUIButton()
	: m_State(EButtonState::Normal)
	, m_MouseHovered(false)
{
    m_WidgetTypeName = "UIButton";
	SetImageTint(EButtonState::Normal, 200, 200, 200, 255);
	SetImageTint(EButtonState::Hovered, 255, 255, 255, 255);
	SetImageTint(EButtonState::Click, 150, 150, 150, 255);
	SetImageTint(EButtonState::Disable, 100, 100, 100, 255);
}

CUIButton::CUIButton(const CUIButton& button)
	: CUIWidget(button)
{
	m_State = button.m_State;
	m_MouseHovered = false;
	for (int i = 0; i < (int)EButtonState::Max; ++i)
	{
		m_TextureInfo[i] = button.m_TextureInfo[i];
	}
	for (int i = 0; i < (int)EButtonEventState::Max; ++i)
	{
		m_Sound[i] = button.m_Sound[i];
	}
}

CUIButton::~CUIButton()
{
}

void CUIButton::SetTexture(EButtonState state, CTexture* texture)
{
    m_TextureInfo[(int)state].texture = texture;
}

bool CUIButton::SetTexture(EButtonState state, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUIButton::SetTextureFullPath(EButtonState state, const std::string& name, const TCHAR* fullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUIButton::SetTexture(EButtonState state, const std::string& name, const std::vector<const TCHAR*>& vecFileName
    , const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

bool CUIButton::SetTextureFullPath(EButtonState state, const std::string& name
    , const std::vector<const TCHAR*>& vecFullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = m_Scene->GetResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
        {
            return false;
        }
        m_TextureInfo[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
    }
    return true;
}

void CUIButton::SetImageTint(EButtonState state, const Vector4& tint)
{
    m_TextureInfo[(int)state].tint = tint;
}

void CUIButton::SetImageTint(EButtonState state, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_TextureInfo[(int)state].tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUIButton::AddFrameData(EButtonState state, const Vector2& start, const Vector2& end)
{
    Animation2DFrameData    frame;
    frame.start = start;
    frame.end = end;
    m_TextureInfo[(int)state].vecFrameData.push_back(frame);
    m_TextureInfo[(int)state].frameTime = 
        m_TextureInfo[(int)state].vecFrameData.size() / m_TextureInfo[(int)state].playTime;
}

void CUIButton::SetPlayTime(EButtonState state, float playTime)
{
    m_TextureInfo[(int)state].playTime = playTime;
    m_TextureInfo[(int)state].frameTime = 
        m_TextureInfo[(int)state].vecFrameData.size() / m_TextureInfo[(int)state].playTime;
}

void CUIButton::SetPlayScale(EButtonState state, float playScale)
{
    m_TextureInfo[(int)state].playScale = playScale;
}

void CUIButton::SetSound(EButtonEventState state, CSound* sound)
{
    m_Sound[(int)state] = sound;
}

void CUIButton::SetSound(EButtonEventState state, const std::string& name)
{
    if (m_Scene)
    {
        m_Sound[(int)state] = m_Scene->GetResource()->FindSound(name);
    }
    else
    {
        m_Sound[(int)state] = CResourceManager::GetInst()->FindSound(name);
    }
}

bool CUIButton::SetSound(EButtonEventState state, const std::string& groupName, const std::string& name
    , bool loop, const char* fileName, const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadSound(groupName, name, loop, fileName, pathName))
        {
            return false;
        }
        m_Sound[(int)state] = m_Scene->GetResource()->FindSound(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadSound(groupName, name, loop, fileName, pathName))
        {
            return false;
        }
        m_Sound[(int)state] = CResourceManager::GetInst()->FindSound(name);
    }
    return true;
}

void CUIButton::Start()
{
	CUIWidget::Start();
}

bool CUIButton::Init()
{
	if (!CUIWidget::Init())
	{
		return false;
	}
	return true;
}

void CUIButton::Update(float deltaTime)
{
    CUIWidget::Update(deltaTime);
    if (m_State != EButtonState::Disable)
    {
        Vector2 mousePos = CInput::GetInst()->GetMouseUIPos();
        bool    mouseHovered = true;
        if (m_RenderPos.x > mousePos.x)
        {
            mouseHovered = false;
        }
        else if (m_RenderPos.y > mousePos.y)
        {
            mouseHovered = false;
        }
        else if (m_RenderPos.x + m_Size.x < mousePos.x)
        {
            mouseHovered = false;
        }
        else if (m_RenderPos.y + m_Size.y < mousePos.y)
        {
            mouseHovered = false;
        }
        if (mouseHovered)
        {
            if (CInput::GetInst()->GetMouseLDown())
            {
                if (m_State != EButtonState::Click)
                {
                    m_TextureInfo[(int)m_State].frame = 0;
                    m_TextureInfo[(int)m_State].time = 0.f;
                }
                m_State = EButtonState::Click;
            }
            else if (m_State == EButtonState::Click && CInput::GetInst()->GetMouseLUp())
            {
                if (m_Sound[(int)EButtonEventState::Click])
                {
                    m_Sound[(int)EButtonEventState::Click]->Play();
                }
                if (m_ClickCallback[(int)EButtonEventState::Click])
                {
                    m_ClickCallback[(int)EButtonEventState::Click]();
                }
                if (m_State != EButtonState::Hovered)
                {
                    m_TextureInfo[(int)m_State].frame = 0;
                    m_TextureInfo[(int)m_State].time = 0.f;
                }
                m_State = EButtonState::Hovered;
            }
            else if (m_State == EButtonState::Click && CInput::GetInst()->GetMouseLPush())
            {
                m_State = EButtonState::Click;
            }
            else
            {
                if (m_MouseHovered != mouseHovered)
                {
                    if (m_Sound[(int)EButtonEventState::Hovered])
                    {
                        m_Sound[(int)EButtonEventState::Hovered]->Play();
                    }
                }
                if (m_ClickCallback[(int)EButtonEventState::Hovered])
                {
                    m_ClickCallback[(int)EButtonEventState::Hovered]();
                }
                if (m_State != EButtonState::Hovered)
                {
                    m_TextureInfo[(int)m_State].frame = 0;
                    m_TextureInfo[(int)m_State].time = 0.f;
                }
                m_State = EButtonState::Hovered;
            }
            m_MouseHovered = true;
        }
        else
        {
            m_MouseHovered = false;
            if (m_State != EButtonState::Normal)
            {
                m_TextureInfo[(int)m_State].frame = 0;
                m_TextureInfo[(int)m_State].time = 0.f;
            }
            m_State = EButtonState::Normal;
        }
    }
    if (!m_TextureInfo[(int)m_State].vecFrameData.empty())
    {
        m_TextureInfo[(int)m_State].time += deltaTime;
        if (m_TextureInfo[(int)m_State].time >= m_TextureInfo[(int)m_State].frameTime)
        {
            m_TextureInfo[(int)m_State].time -= m_TextureInfo[(int)m_State].frameTime;
            ++m_TextureInfo[(int)m_State].frame;
            if (m_TextureInfo[(int)m_State].frame == m_TextureInfo[(int)m_State].vecFrameData.size())
            {
                m_TextureInfo[(int)m_State].frame = 0;
            }
        }
    }
}

void CUIButton::PostUpdate(float deltaTime)
{
	CUIWidget::PostUpdate(deltaTime);
}

void CUIButton::Render()
{
    bool textureEnable = m_TextureInfo[(int)m_State].texture ? true : false;
    m_CBuffer->SetTextureEnable(textureEnable);
    if (textureEnable)
    {
        if (!m_TextureInfo[(int)m_State].vecFrameData.empty())
        {
            int textureFrame = 0;
            if (m_TextureInfo[(int)m_State].texture->GetImageType() == EImageType::Frame)
            {
                textureFrame = m_TextureInfo[(int)m_State].frame;
            }
            m_TextureInfo[(int)m_State].texture->SetShader(0, (int)EShaderBufferType::Pixel, textureFrame);
            m_AnimCBuffer->SetAnim2DEnable(true);
            m_AnimCBuffer->SetFrame(m_TextureInfo[(int)m_State].frame);
            m_AnimCBuffer->SetImageFrame(m_TextureInfo[(int)m_State].vecFrameData[m_TextureInfo[(int)m_State].frame].start,
                m_TextureInfo[(int)m_State].vecFrameData[m_TextureInfo[(int)m_State].frame].end);
            m_AnimCBuffer->SetImageSize((float)m_TextureInfo[(int)m_State].texture->GetWidth(),
                (float)m_TextureInfo[(int)m_State].texture->GetHeight());
            m_AnimCBuffer->SetImageType((EAnimation2DType)m_TextureInfo[(int)m_State].texture->GetImageType());
        }
        else
        {
            m_TextureInfo[(int)m_State].texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
            m_AnimCBuffer->SetAnim2DEnable(false);
        }
    }
    m_Tint = m_TextureInfo[(int)m_State].tint;
    CUIWidget::Render();
}

CUIButton* CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::Save(FILE* file)
{
    CUIWidget::Save(file);
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        fwrite(&m_TextureInfo[i].tint, sizeof(Vector4), 1, file);
        fwrite(&m_TextureInfo[i].frameTime, sizeof(float), 1, file);
        fwrite(&m_TextureInfo[i].playTime, sizeof(float), 1, file);
        fwrite(&m_TextureInfo[i].playScale, sizeof(float), 1, file);
        int frameCount = (int)m_TextureInfo[i].vecFrameData.size();
        fwrite(&frameCount, sizeof(int), 1, file);
        if (frameCount > 0)
        {
            fwrite(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
        }
        bool    textureEnable = m_TextureInfo[i].texture ? true : false;
        fwrite(&textureEnable, sizeof(bool), 1, file);
        if (textureEnable)
        {
            std::string texName = m_TextureInfo[i].texture->GetName();
            int length = (int)texName.length();
            fwrite(&length, sizeof(int), 1, file);
            fwrite(texName.c_str(), 1, length, file);
            m_TextureInfo[i].texture->Save(file);
        }
    }
    for (int i = 0; i < (int)EButtonEventState::Max; ++i)
    {
        bool    soundEnable = m_Sound[i] ? true : false;
        fwrite(&soundEnable, sizeof(bool), 1, file);
        if (soundEnable)
        {
            std::string soundName = m_Sound[i]->GetName();
            int length = (int)soundName.length();
            fwrite(&length, sizeof(int), 1, file);
            fwrite(soundName.c_str(), 1, length, file);
            m_Sound[i]->Save(file);
        }
    }
}

void CUIButton::Load(FILE* file)
{
    CUIWidget::Load(file);
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        fread(&m_TextureInfo[i].tint, sizeof(Vector4), 1, file);
        fread(&m_TextureInfo[i].frameTime, sizeof(float), 1, file);
        fread(&m_TextureInfo[i].playTime, sizeof(float), 1, file);
        fread(&m_TextureInfo[i].playScale, sizeof(float), 1, file);
        int frameCount = 0;
        fread(&frameCount, sizeof(int), 1, file);
        if (frameCount > 0)
        {
            m_TextureInfo[i].vecFrameData.resize((size_t)frameCount);
            fread(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
        }
        bool    textureEnable = false;
        fread(&textureEnable, sizeof(bool), 1, file);
        if (textureEnable)
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
                    m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
                }
                else
                {
                    CResourceManager::GetInst()->LoadTexture(texName, fileName, pathName);
                    m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
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
                        m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
                    }
                    else
                    {
                        CResourceManager::GetInst()->LoadTexture(texName, vecFileName, resultPathName);
                        m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
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
                        m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
                    }
                    else
                    {
                        CResourceManager::GetInst()->LoadTextureArray(texName, vecFileName, resultPathName);
                        m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
                    }
                    for (int i = 0; i < textureSRVCount; ++i)
                    {
                        SAFE_DELETE_ARRAY(vecFileName[i]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < (int)EButtonEventState::Max; ++i)
    {
        bool    soundEnable = false;
        fread(&soundEnable, sizeof(bool), 1, file);
        if (soundEnable)
        {
            char    soundName[256] = {};
            int length = 0;
            fread(&length, sizeof(int), 1, file);
            assert(length >= 0);
            fread(soundName, 1, length, file);
            bool    loop = false;
            fread(&loop, sizeof(bool), 1, file);
            length = 0;
            char    groupName[256] = {};
            fread(&length, sizeof(int), 1, file);
            assert(length >= 0);
            fread(groupName, 1, length, file);
            char    fileName[MAX_PATH] = {};
            char    pathName[MAX_PATH] = {};
            fread(fileName, sizeof(char), MAX_PATH, file);
            fread(pathName, sizeof(char), MAX_PATH, file);
            if (m_Scene)
            {
                m_Scene->GetResource()->LoadSound(groupName, soundName, loop, fileName, pathName);
                m_Sound[i] = m_Scene->GetResource()->FindSound(soundName);
            }
            else
            {
                CResourceManager::GetInst()->LoadSound(groupName, soundName, loop, fileName, pathName);
                m_Sound[i] = CResourceManager::GetInst()->FindSound(soundName);
            }
        }
    }
}
