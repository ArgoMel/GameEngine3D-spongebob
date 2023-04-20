#pragma once
#include "../../Ref.h"

class CFont : public CRef
{
private:
    friend class CFontManager;
    IDWriteFactory5* m_Factory;
    IDWriteTextFormat* m_Format;
    CFont();
    ~CFont();
public:
    bool LoadFont(IDWriteFactory5* writeFactory, const std::string& name, const TCHAR* fontName,
        int weight, float fontSize, const TCHAR* localName, int stretch=5);
    IDWriteTextLayout* CreateLayout(const TCHAR* text, float width, float height);
};

