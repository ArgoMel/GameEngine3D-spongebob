#include "UIText.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Device.h"

CUIText::CUIText()
    : m_Layout(nullptr)
    , m_Brush(nullptr)
    , m_RenderTarget(nullptr)
    , m_FontSize(10.f)
    , m_Transparency(false)
    , m_Opacity(1.f)
    , m_Color(Vector4::white)
    , m_ShadowEnable(false)
    , m_ShadowBrush(nullptr)
    , m_ShadowTransparency(false)
    , m_ShadowOpacity(1.f)
    , m_AlignH(Text_Align_H::Left)
    , m_AlignV(Text_Align_V::Top)
    , m_ShadowOffset(1.f, 1.f)
{
    m_Color.w = m_Opacity;
    m_ShadowColor = m_ShadowOpacity;
    m_TextCapacity = 32;
    m_TextCount = 0;
    m_Text = new TCHAR[m_TextCapacity];
    _tcscpy_s(m_Text, m_TextCapacity, TEXT("\n"));
    m_WidgetTypeName = "UIText";
}

CUIText::CUIText(const CUIText& text)
    : CUIWidget(text)
{
    m_Font = text.m_Font;
    m_Layout = nullptr;
    m_Brush = text.m_Brush;
    m_RenderTarget = text.m_RenderTarget;
    m_FontSize = text.m_FontSize;
    m_Transparency = text.m_Transparency;
    m_Opacity = text.m_Opacity;
    m_Color = text.m_Color;
    m_ShadowEnable = text.m_ShadowEnable;
    m_ShadowBrush = text.m_ShadowBrush;
    m_ShadowTransparency = text.m_ShadowTransparency;
    m_ShadowOpacity = text.m_ShadowOpacity;
    m_ShadowColor = text.m_ShadowColor;
    m_ShadowOffset = text.m_ShadowOffset;
    m_AlignH = text.m_AlignH;
    m_AlignV = text.m_AlignV;
    m_TextCapacity = text.m_TextCapacity;
    m_TextCount = text.m_TextCount;
    m_Text = new TCHAR[m_TextCapacity];
    lstrcpy(m_Text, text.m_Text);
    CreateLayout();
}

CUIText::~CUIText()
{
    SAFE_RELEASE(m_Layout);
    SAFE_DELETE_ARRAY(m_Text);
}

void CUIText::SetFont(const std::string& name)
{
    if (m_Scene)
    {
        m_Font = m_Scene->GetResource()->FindFont(name);
    }
    else
    {
        m_Font = CResourceManager::GetInst()->FindFont(name);
    }
    CreateLayout();
}

void CUIText::SetFont(CFont* font)
{
    m_Font = font;
    CreateLayout();
}

void CUIText::SetAlignH(Text_Align_H align)
{
    m_AlignH = align;
    CreateLayout();
}

void CUIText::SetAlignV(Text_Align_V align)
{
    m_AlignV = align;
    CreateLayout();
}

void CUIText::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
    m_Color.x = r / 255.f;
    m_Color.y = g / 255.f;
    m_Color.z = b / 255.f;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_Color);
        m_Brush = m_Scene->GetResource()->FindFontColor(m_Color);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_Color);
        m_Brush = CResourceManager::GetInst()->FindFontColor(m_Color);
    }
}

void CUIText::SetColor(const Vector4& color)
{
    m_Color.x = color.x;
    m_Color.y = color.y;
    m_Color.z = color.z;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_Color);
        m_Brush = m_Scene->GetResource()->FindFontColor(m_Color);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_Color);
        m_Brush = CResourceManager::GetInst()->FindFontColor(m_Color);
    }
}

void CUIText::SetColor(unsigned int color)
{
    m_Color.x = ((color >> 16) & 0x000000ff) / 255.f;
    m_Color.y = ((color >> 8) & 0x000000ff) / 255.f;
    m_Color.z = (color & 0x000000ff) / 255.f;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_Color);
        m_Brush = m_Scene->GetResource()->FindFontColor(m_Color);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_Color);
        m_Brush = CResourceManager::GetInst()->FindFontColor(m_Color);
    }
}

void CUIText::SetFontSize(float size)
{
    m_FontSize = size;
    CreateLayout();
}

void CUIText::SetOpacity(float opacity)
{
    m_Opacity = opacity;
}

void CUIText::SetTransparency(bool transparency)
{
    m_Transparency = transparency;
}

void CUIText::SetShadowColor(unsigned char r, unsigned char g, unsigned char b)
{
    m_ShadowColor.x = r / 255.f;
    m_ShadowColor.y = g / 255.f;
    m_ShadowColor.z = b / 255.f;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = m_Scene->GetResource()->FindFontColor(m_ShadowColor);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
    }
}

void CUIText::SetShadowColor(const Vector4& color)
{
    m_ShadowColor.x = color.x;
    m_ShadowColor.y = color.y;
    m_ShadowColor.z = color.z;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = m_Scene->GetResource()->FindFontColor(m_ShadowColor);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
    }
}

void CUIText::SetShadowColor(unsigned int color)
{
    m_ShadowColor.x = ((color >> 16) & 0x000000ff) / 255.f;
    m_ShadowColor.y = ((color >> 8) & 0x000000ff) / 255.f;
    m_ShadowColor.z = (color & 0x000000ff) / 255.f;
    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = m_Scene->GetResource()->FindFontColor(m_ShadowColor);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
        m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
    }
}

void CUIText::SetShadowOffset(const Vector2& offset)
{
    m_ShadowOffset = offset;
}

void CUIText::SetShadowOffset(float x, float y)
{
    m_ShadowOffset = Vector2(x, y);
}

void CUIText::SetShadowOpacity(float opacity)
{
    m_ShadowOpacity = opacity;
}

void CUIText::SetShadowEnable(bool enable)
{
    m_ShadowEnable = enable;
}

void CUIText::SetShadowTransparency(bool transparency)
{
    m_ShadowTransparency = transparency;
}

void CUIText::Start()
{
    CUIWidget::Start();
}

bool CUIText::Init()
{
    if (!CUIWidget::Init())
    {
        return false;
    }
    m_RenderTarget = CDevice::GetInst()->Get2DTarget();
    SetFont("Default");
    SetColor(m_Color);
    SetShadowColor(m_ShadowColor);
    CreateLayout();
    return true;
}

void CUIText::Update(float deltaTime)
{
    CUIWidget::Update(deltaTime);
}

void CUIText::PostUpdate(float deltaTime)
{
    CUIWidget::PostUpdate(deltaTime);
}

void CUIText::Render()
{
    m_RenderTarget->BeginDraw();
    Resolution RS = CDevice::GetInst()->GetResolution();
    D2D1_POINT_2F   point;
    point.x = m_RenderPos.x;
    point.y = RS.height - m_RenderPos.y - m_Size.y;
    if (m_ShadowEnable)
    {
        D2D1_POINT_2F   shadowPoint = point;
        shadowPoint.x += m_ShadowOffset.x;
        shadowPoint.y += m_ShadowOffset.y;
        if (m_ShadowTransparency)
        {
            m_ShadowBrush->SetOpacity(m_ShadowOpacity);
        }
        else
        {
            m_ShadowBrush->SetOpacity(1.f);
        }
        m_RenderTarget->DrawTextLayout(shadowPoint, m_Layout, m_ShadowBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }
    if (m_Transparency)
    {
        m_Brush->SetOpacity(m_Opacity);
    }
    else
    {
        m_Brush->SetOpacity(1.f);
    }
    m_RenderTarget->DrawTextLayout(point, m_Layout, m_Brush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    m_RenderTarget->EndDraw();
}

CUIText* CUIText::Clone()
{
    return new CUIText(*this);
}

void CUIText::Save(FILE* file)
{
    CUIWidget::Save(file);
    fwrite(&m_TextCapacity, sizeof(int), 1, file);
    fwrite(&m_TextCount, sizeof(int), 1, file);
    fwrite(m_Text, sizeof(TCHAR), m_TextCount, file);
    int length = (int)m_Font->GetName().length();
    fwrite(&length, sizeof(int), 1, file);
    fwrite(m_Font->GetName().c_str(), 1, length, file);
    fwrite(&m_FontSize, sizeof(float), 1, file);
    fwrite(&m_Transparency, sizeof(bool), 1, file);
    fwrite(&m_Opacity, sizeof(float), 1, file);
    fwrite(&m_Color, sizeof(Vector4), 1, file);
    fwrite(&m_ShadowEnable, sizeof(bool), 1, file);
    fwrite(&m_ShadowTransparency, sizeof(bool), 1, file);
    fwrite(&m_ShadowOpacity, sizeof(float), 1, file);
    fwrite(&m_ShadowColor, sizeof(Vector4), 1, file);
    fwrite(&m_ShadowOffset, sizeof(Vector2), 1, file);
    fwrite(&m_AlignH, sizeof(Text_Align_H), 1, file);
    fwrite(&m_AlignV, sizeof(Text_Align_V), 1, file);
}

void CUIText::Load(FILE* file)
{
    CUIWidget::Load(file);
    SAFE_DELETE_ARRAY(m_Text);
    fread(&m_TextCapacity, sizeof(int), 1, file);
    fread(&m_TextCount, sizeof(int), 1, file);
    m_Text = new TCHAR[m_TextCapacity];
    memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
    assert(m_TextCount >=0);
    fread(m_Text, sizeof(TCHAR), m_TextCount, file);
    int length = 0;
    char    fontName[256] = {};
    fread(&length, sizeof(int), 1, file);
    assert(length >= 0);
    fread(fontName, 1, length, file);
    SetFont(fontName);
    fread(&m_FontSize, sizeof(float), 1, file);
    fread(&m_Transparency, sizeof(bool), 1, file);
    fread(&m_Opacity, sizeof(float), 1, file);
    fread(&m_Color, sizeof(Vector4), 1, file);
    fread(&m_ShadowEnable, sizeof(bool), 1, file);
    fread(&m_ShadowTransparency, sizeof(bool), 1, file);
    fread(&m_ShadowOpacity, sizeof(float), 1, file);
    fread(&m_ShadowColor, sizeof(Vector4), 1, file);
    fread(&m_ShadowOffset, sizeof(Vector2), 1, file);
    fread(&m_AlignH, sizeof(Text_Align_H), 1, file);
    fread(&m_AlignV, sizeof(Text_Align_V), 1, file);
    SetColor(m_Color);
    SetShadowColor(m_ShadowColor);
    m_RenderTarget = CDevice::GetInst()->Get2DTarget();
    CreateLayout();
}

void CUIText::CreateLayout()
{
    SAFE_RELEASE(m_Layout);
    m_Layout = m_Font->CreateLayout(m_Text, m_Size.x, m_Size.y);
    DWRITE_TEXT_RANGE   range = {};
    range.startPosition = 0;
    range.length = lstrlen(m_Text);
    m_Layout->SetFontSize(m_FontSize, range);
    switch (m_AlignH)
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
    switch (m_AlignV)
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