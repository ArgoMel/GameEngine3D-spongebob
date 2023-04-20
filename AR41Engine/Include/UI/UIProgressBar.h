#pragma once
#include "UIWidget.h"
#include "../Resource/Shader/UIProgressBarConstantBuffer.h"

class CUIProgressBar : public CUIWidget
{
protected:
    friend class CUIWindow;
    friend class CResourceManager;
    friend class CScene;
    static CUIProgressBarConstantBuffer* m_ProgressBarCBuffer;
    CSharedPtr<class CShader>   m_BarShader;
    UIWidgetImageInfo   m_TextureInfo[(int)EProgressBarTextureType::Max];
    EProgressBarDir     m_BarDir;
    float               m_Min;
    float               m_Max;
    float               m_Value;
    CUIProgressBar();
    CUIProgressBar(const CUIProgressBar& progressBar);
    virtual ~CUIProgressBar();
public:
    UIWidgetImageInfo GetTextureInfo(int index)    const
    {
        return m_TextureInfo[index];
    }
    EProgressBarDir GetBarDir()    const
    {
        return m_BarDir;
    }
    float GetMinValue()    const
    {
        return m_Min;
    }
    float GetMaxValue()    const
    {
        return m_Max;
    }
    float GetValue()    const
    {
        return m_Value;
    }
    void SetBarDir(EProgressBarDir dir)
    {
        m_BarDir = dir;
    }
    void SetTexture(EProgressBarTextureType type, CTexture* texture);
    bool SetTexture(EProgressBarTextureType type, const std::string& name, const TCHAR* fileName, 
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(EProgressBarTextureType type, const std::string& name, const TCHAR* fullPath);
    bool SetTexture(EProgressBarTextureType type, const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(EProgressBarTextureType type, const std::string& name, 
        const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(EProgressBarTextureType type, const Vector4& tint);
    void SetImageTint(EProgressBarTextureType type, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(EProgressBarTextureType type, const Vector2& start, const Vector2& end);
    void SetPlayTime(EProgressBarTextureType type, float playTime);
    void SetPlayScale(EProgressBarTextureType type, float playScale);
    void SetProgressBarMin(float min);
    void SetProgressBarMax(float max);
    void SetValue(float value);
    void AddValue(float value);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUIProgressBar* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

