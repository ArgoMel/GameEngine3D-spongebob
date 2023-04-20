#pragma once
#include "../../Ref.h"

class CFontCollection : public CRef
{
private:
    friend class CFontManager;
    IDWriteFontCollection1* m_Collection;
    TCHAR               m_FontFaceName[64];
    char                m_FontFaceNameMultibyte[64];
    CFontCollection();
    ~CFontCollection();
public:
    bool CreateFontCollection(IDWriteFactory5* writeFactory, const std::string& name, const TCHAR* fileName,
        const std::string& pathName = FONT_PATH);
    const TCHAR* GetFontFaceName()  const;
    const char* GetFontFaceNameMultibyte()  const;
};

