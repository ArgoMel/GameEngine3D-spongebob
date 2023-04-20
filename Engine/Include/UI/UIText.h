#pragma once
#include "UIWidget.h"
#include "../Resource/Font/Font.h"

class CUIText : public CUIWidget
{
private:
    friend class CUIWindow;
    friend class CScene;
    void CreateLayout();
protected:
    IDWriteTextLayout* m_Layout;
    ID2D1SolidColorBrush* m_Brush;
    ID2D1RenderTarget* m_RenderTarget;
    CSharedPtr<CFont> m_Font;
    TCHAR* m_Text;
    Text_Align_H    m_AlignH;
    Text_Align_V    m_AlignV;
    Vector4     m_Color;
    int m_TextCount;
    int m_TextCapacity;
    float       m_FontSize;
    float       m_Opacity;
    bool        m_Transparency;
    // Shadow
    ID2D1SolidColorBrush* m_ShadowBrush;
    Vector4     m_ShadowColor;
    Vector2     m_ShadowOffset;
    float       m_ShadowOpacity;
    bool        m_ShadowEnable;
    bool        m_ShadowTransparency;
    CUIText();
    CUIText(const CUIText& text);
    virtual ~CUIText();
public:
    const TCHAR* GetText()  const
    {
        return m_Text;
    }
    Text_Align_H GetAlignH()  const
    {
        return m_AlignH;
    }
    Text_Align_V GetAlignV()  const
    {
        return m_AlignV;
    }
    Vector4 GetColor()	const
    {
        return m_Color;
    }
    int GetTextCount()  const
    {
        return m_TextCount;
    }
    float GetFontSize()  const
    {
        return m_FontSize;
    }
    bool GetTransparency()  const
    {
        return m_Transparency;
    }
    Vector4 GetShadowColor()	const
    {
        return m_ShadowColor;
    }
    float GetShadowOpacity()  const
    {
        return m_ShadowOpacity;
    }
    Vector2 GetShadowOffset()	const
    {
        return m_ShadowOffset;
    }
    bool GetShadowEnable()  const
    {
        return m_ShadowEnable;
    }
    bool GetShadowTransparency()  const
    {
        return m_ShadowTransparency;
    }
    void SetFont(const std::string& name);
    void SetFont(CFont* font);
    void SetText(const TCHAR* text)
    {
        int length = lstrlen(text);
        if (length + 1 > m_TextCapacity)
        {
            m_TextCapacity = length + 1;
            SAFE_DELETE_ARRAY(m_Text);
            m_Text = new TCHAR[m_TextCapacity];
            memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
        }
        lstrcpy(m_Text, text);
        m_TextCount = length;
        CreateLayout();
    }
    void SetText(const char* text)
    {
        TCHAR tChar[MAX_PATH] = {};
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, (int)strlen(text), tChar, 256);
        SetText(tChar);
    }
    void SetIntText(int text,bool needZero=false)
    {
        TCHAR number[MAX_PATH] = {};
        if (text < 10&& needZero)
        {
            swprintf_s(number, TEXT("0%d"), text);
        }
        else
        {
            swprintf_s(number, TEXT("%d"), text);
        }
        SetText(number);
    }
    void SetFloatText(float text)
    {
        TCHAR number[MAX_PATH] = {};
        swprintf_s(number, TEXT("%f"), text);
        SetText(number);
    }
    virtual void SetSize(const Vector2& size)
    {
        CUIWidget::SetSize(size);
        CreateLayout();
    }
    virtual void SetSize(float x, float y)
    {
        CUIWidget::SetSize(x, y);
        CreateLayout();
    }
    void SetAlignH(Text_Align_H align);
    void SetAlignV(Text_Align_V align);
    void SetColor(unsigned char r, unsigned char g, unsigned char b);
    void SetColor(const Vector4& color);
    void SetColor(unsigned int color);
    void SetFontSize(float size);
    void SetOpacity(float opacity);
    void SetTransparency(bool transparency);

    void SetShadowColor(unsigned char r, unsigned char g, unsigned char b);
    void SetShadowColor(const Vector4& color);
    void SetShadowColor(unsigned int color);
    void SetShadowOffset(const Vector2& offset);
    void SetShadowOffset(float x, float y);
    void SetShadowOpacity(float opacity);
    void SetShadowEnable(bool enable);
    void SetShadowTransparency(bool transparency);
    void Append(const TCHAR* text)
    {
        int length = lstrlen(text) + m_TextCount;
        if (length + 1 > m_TextCapacity)
        {
            m_TextCapacity = length + 1;
            TCHAR* newText = new TCHAR[m_TextCapacity];
            memset(newText, 0, sizeof(TCHAR) * m_TextCapacity);
            lstrcpy(newText, m_Text);
            SAFE_DELETE_ARRAY(m_Text);
            m_Text = newText;
        }
        lstrcat(m_Text, text);
        m_TextCount += lstrlen(text);
        CreateLayout();
    }
    void pop_back()
    {
        if (m_TextCount > 0)
        {
            --m_TextCount;
            m_Text[m_TextCount] = 0;
            CreateLayout();
        }
    }
    void Clear()
    {
        if (m_TextCount > 0)
        {
            m_TextCount = 0;
            memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
            CreateLayout();
        }
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUIText* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

