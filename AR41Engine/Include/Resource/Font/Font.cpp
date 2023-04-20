#include "Font.h"

CFont::CFont()  
    : m_Format(nullptr)
    , m_Factory(nullptr)
{
}

CFont::~CFont()
{
    SAFE_RELEASE(m_Format);
}

bool CFont::LoadFont(IDWriteFactory5* writeFactory, const std::string& name,
    const TCHAR* fontName, int weight, float fontSize, const TCHAR* localName, int stretch)
{
    m_Factory = writeFactory;
    SetName(name);
    if (FAILED(writeFactory->CreateTextFormat(fontName, nullptr, (DWRITE_FONT_WEIGHT)weight,
        DWRITE_FONT_STYLE_NORMAL, (DWRITE_FONT_STRETCH)stretch, fontSize, localName, &m_Format)))
    {
        return false;
    }
    return true;
}

IDWriteTextLayout* CFont::CreateLayout(const TCHAR* text, float width, float height)
{
    IDWriteTextLayout* layout = nullptr;
    int temp = lstrlen(text);
    if (FAILED(m_Factory->CreateTextLayout(text, lstrlen(text), m_Format, width, height, &layout)))
    {
        return nullptr;
    }
    return layout;
}
