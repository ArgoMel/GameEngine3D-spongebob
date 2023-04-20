#include "UITextButton.h"
#include "../Input.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUITextButton::CUITextButton() 
    : m_Layout(nullptr)
    , m_RenderTarget(nullptr)
    , m_State(EButtonState::Normal)
    , m_MouseHovered(false)
{
    m_WidgetTypeName = "UITextButton";
}

CUITextButton::CUITextButton(const CUITextButton& button)
    : CUIWidget(button)
{
    m_Font = button.m_Font;
    m_Layout = nullptr;
    m_RenderTarget = button.m_RenderTarget;
    m_State = button.m_State;
    m_MouseHovered = false;
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        m_TextInfo[i] = button.m_TextInfo[i];
        CreateLayout((EButtonState)i);
    }
    for (int i = 0; i < (int)EButtonEventState::Max; ++i)
    {
        m_Sound[i] = button.m_Sound[i];
    }
}

CUITextButton::~CUITextButton()
{
    SAFE_RELEASE(m_Layout);

    //for (UIWidgetTextInfo iter : m_TextInfo)
    //{
    //    SAFE_DELETE_ARRAY(iter.text);
    //}
}

void CUITextButton::SetFont(const std::string& name)
{
    if (m_Scene)
    {
        m_Font = m_Scene->GetResource()->FindFont(name);
    }
    else
    {
        m_Font = CResourceManager::GetInst()->FindFont(name);
    }
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        CreateLayout((EButtonState)i);
    }
}

void CUITextButton::SetText(EButtonState state, const TCHAR* text, float size, const Vector4& color)
{
    //m_TextInfo[(int)state];
    int length = lstrlen(text);
    if (length + 1 > m_TextInfo[(int)state].textCapacity)
    {
        m_TextInfo[(int)state].textCapacity = length + 1;
        SAFE_DELETE_ARRAY(m_TextInfo[(int)state].text);
        m_TextInfo[(int)state].text = new TCHAR[m_TextInfo[(int)state].textCapacity];
        memset(m_TextInfo[(int)state].text, 0, sizeof(TCHAR) * m_TextInfo[(int)state].textCapacity);
    }
    lstrcpy(m_TextInfo[(int)state].text, text);
    m_TextInfo[(int)state].textCount = length;
    m_TextInfo[(int)state].fontSize = size;
    m_TextInfo[(int)state].color = color;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(color);
        m_TextInfo[(int)state].brush = m_Scene->GetResource()->FindFontColor(color);
        m_TextInfo[(int)state].shadowBrush = m_Scene->GetResource()->FindFontColor(m_TextInfo[(int)state].shadowColor);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(color);
        m_TextInfo[(int)state].brush = CResourceManager::GetInst()->FindFontColor(color);
        m_TextInfo[(int)state].shadowBrush = CResourceManager::GetInst()->FindFontColor(m_TextInfo[(int)state].shadowColor);
    }
    CreateLayout(state);
}

void CUITextButton::CreateLayout(EButtonState state)
{
    SAFE_RELEASE(m_Layout);
    m_Layout = m_Font->CreateLayout(m_TextInfo[(int)state].text, m_Size.x, m_Size.y);
    DWRITE_TEXT_RANGE   range = {};
    range.startPosition = 0;
    range.length = lstrlen(m_TextInfo[(int)state].text);
    m_Layout->SetFontSize(m_TextInfo[(int)state].fontSize, range);
    switch (m_TextInfo[(int)state].alignH)
    {
    case Text_Align_H::Left:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        break;
    case Text_Align_H::Center:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        break;
    case Text_Align_H::Right:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
        break;
    }
    switch (m_TextInfo[(int)state].alignV)
    {
    case Text_Align_V::Top:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        break;
    case Text_Align_V::Middle:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        break;
    case Text_Align_V::Bottom:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
        break;
    }
}

void CUITextButton::SetSound(EButtonEventState state, CSound* sound)
{
    m_Sound[(int)state] = sound;
}

void CUITextButton::SetSound(EButtonEventState state, const std::string& name)
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

bool CUITextButton::SetSound(EButtonEventState state, const std::string& groupName, const std::string& name, 
    bool loop, const char* fileName, const std::string& pathName)
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

void CUITextButton::Start()
{
    CUIWidget::Start();
}

bool CUITextButton::Init()
{
    if (!CUIWidget::Init())
    {
        return false;
    }
    m_RenderTarget = CDevice::GetInst()->Get2DTarget();
    SetFont("Default");
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(Vector4::black);
        m_TextInfo[(int)m_State].brush = m_Scene->GetResource()->FindFontColor(Vector4::black);
        m_TextInfo[(int)m_State].shadowBrush = m_Scene->GetResource()->FindFontColor(Vector4::black);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(Vector4::black);
        m_TextInfo[(int)m_State].brush = CResourceManager::GetInst()->FindFontColor(Vector4::black);
        m_TextInfo[(int)m_State].shadowBrush = CResourceManager::GetInst()->FindFontColor(Vector4::black);
    }
    CreateLayout(m_State);
    return true;
}

void CUITextButton::Update(float deltaTime)
{
    CUIWidget::Update(deltaTime);
    if (m_State != EButtonState::Disable)
    {
        Vector2 mousePos = CInput::GetInst()->GetMousePos();
        Resolution	RS = CDevice::GetInst()->GetResolution();
        mousePos.y = RS.height - mousePos.y;
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
                m_State = EButtonState::Click;
                CreateLayout(m_State);
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
                m_State = EButtonState::Hovered;
                CreateLayout(m_State);
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
                m_State = EButtonState::Hovered;
                CreateLayout(m_State);
            }
            m_MouseHovered = true;
        }
        else
        {
            m_MouseHovered = false;
            m_State = EButtonState::Normal;
            CreateLayout(m_State);
        }
    }
}

void CUITextButton::PostUpdate(float deltaTime)
{
    CUIWidget::PostUpdate(deltaTime);
}

void CUITextButton::Render()
{
    m_RenderTarget->BeginDraw();
    Resolution RS = CDevice::GetInst()->GetResolution();
    // Text
    D2D1_POINT_2F   point;
    point.x = m_RenderPos.x;
    point.y = RS.height - m_RenderPos.y - m_Size.y;
    // Shadow
    {
        D2D1_POINT_2F   shadowPoint = point;
        shadowPoint.x += m_TextInfo[(int)m_State].shadowOffset.x;
        shadowPoint.y += m_TextInfo[(int)m_State].shadowOffset.y;
        m_TextInfo[(int)m_State].shadowBrush->SetOpacity(1.f);
        m_RenderTarget->DrawTextLayout(shadowPoint, m_Layout, m_TextInfo[(int)m_State].shadowBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }
    m_RenderTarget->DrawTextLayout(point, m_Layout, m_TextInfo[(int)m_State].brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    m_RenderTarget->EndDraw();
}

CUITextButton* CUITextButton::Clone()
{
    return new CUITextButton(*this);
}

void CUITextButton::Save(FILE* file)
{
    CUIWidget::Save(file);
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        fwrite(&m_TextInfo[i].textCapacity, sizeof(int), 1, file);
        fwrite(&m_TextInfo[i].textCount, sizeof(int), 1, file);
        fwrite(m_TextInfo[i].text, sizeof(TCHAR), m_TextInfo[i].textCount, file);
        int length = (int)m_Font->GetName().length();
        fwrite(&length, sizeof(int), 1, file);
        fwrite(m_Font->GetName().c_str(), 1, length, file);
        fwrite(&m_TextInfo[i].fontSize, sizeof(float), 1, file);
        fwrite(&m_TextInfo[i].opacity, sizeof(float), 1, file);
        fwrite(&m_TextInfo[i].color, sizeof(Vector4), 1, file);
        fwrite(&m_TextInfo[i].shadowColor, sizeof(Vector4), 1, file);
        fwrite(&m_TextInfo[i].shadowOffset, sizeof(Vector2), 1, file);
        fwrite(&m_TextInfo[i].alignH, sizeof(Text_Align_H), 1, file);
        fwrite(&m_TextInfo[i].alignV, sizeof(Text_Align_V), 1, file);
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

void CUITextButton::Load(FILE* file)
{
    CUIWidget::Load(file);
    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {

        SAFE_DELETE_ARRAY(m_TextInfo[i].text);
        fread(&m_TextInfo[i].textCapacity, sizeof(int), 1, file);
        fread(&m_TextInfo[i].textCount, sizeof(int), 1, file);
        m_TextInfo[i].text = new TCHAR[m_TextInfo[i].textCapacity];
        memset(m_TextInfo[i].text, 0, sizeof(TCHAR) * m_TextInfo[i].textCapacity);
        fread(m_TextInfo[i].text, sizeof(TCHAR), m_TextInfo[i].textCount, file);
        int     length = 0;
        char    fontName[256] = {};
        fread(&length, sizeof(int), 1, file);
        assert(length >= 0);
        fread(fontName, 1, length, file);
        SetFont(fontName);
        fread(&m_TextInfo[i].fontSize, sizeof(float), 1, file);
        fread(&m_TextInfo[i].opacity, sizeof(float), 1, file);
        fread(&m_TextInfo[i].color, sizeof(Vector4), 1, file);
        fread(&m_TextInfo[i].shadowColor, sizeof(Vector4), 1, file);
        fread(&m_TextInfo[i].shadowOffset, sizeof(Vector2), 1, file);
        fread(&m_TextInfo[i].alignH, sizeof(Text_Align_H), 1, file);
        fread(&m_TextInfo[i].alignV, sizeof(Text_Align_V), 1, file);
        m_RenderTarget = CDevice::GetInst()->Get2DTarget();
        SetText((EButtonState)i, m_TextInfo[i].text, m_TextInfo[i].fontSize, m_TextInfo[i].color);
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
            // Group 이름과 Loop 저장해야한다
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
