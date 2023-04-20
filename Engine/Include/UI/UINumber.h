#pragma once
#include "UIWidget.h"

class CUINumber : public CUIWidget
{
protected:
    friend class CUIWindow;
    friend class CScene;
    UIWidgetImageInfo           m_TextureInfo;
    unsigned int                m_Number;
    float                       m_Space;
    std::vector<unsigned int>   m_vecNumber;
    CUINumber();
    CUINumber(const CUINumber& number);
    virtual ~CUINumber();
public:
    UIWidgetImageInfo GetTextureInfo()	const
    {
        return m_TextureInfo;
    }
    unsigned int GetNumber()	const
    {
        return m_Number;
    }
    void SetNumber(unsigned int number)
    {
        m_Number = number;
    }
    void AddNumber(unsigned int number)
    {
        m_Number += number;
    }
    float GetSpace()	const
    {
        return m_Space;
    }
    void SetSpace(float space)
    {
        m_Space = space;
    }
    void SetTexture(CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(const Vector4& tint);
    void SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& start, const Vector2& end);
    void SetPlayTime(float playTime);
    void SetPlayScale(float playScale);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUINumber* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

