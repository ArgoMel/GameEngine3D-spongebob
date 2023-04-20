#pragma once
#include "UIWidget.h"

class CUIImage : public CUIWidget
{
protected:
    friend class CUIWindow;
    friend class CScene;
    UIWidgetImageInfo   m_TextureInfo;
    CUIImage();
    CUIImage(const CUIImage& image);
    virtual ~CUIImage();
public:
    UIWidgetImageInfo GetTextureInfo()	const
    {
        return m_TextureInfo;
    }
    void SetTexture(CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName,
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(const Vector4& tint);
    void SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& start, const Vector2& end);
    void DeleteFrame(int index);
    void ClearFrame();
    void SetPlayTime(float playTime);
    void SetPlayScale(float playScale);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUIImage* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

