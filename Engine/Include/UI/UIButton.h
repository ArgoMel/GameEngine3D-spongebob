#pragma once
#include "UIWidget.h"

class CUIButton : public CUIWidget
{
protected:
    friend class CUIWindow;
    friend class CScene;
    CSharedPtr<class CSound>    m_Sound[(int)EButtonEventState::Max];
    UIWidgetImageInfo   m_TextureInfo[(int)EButtonState::Max];
    EButtonState        m_State;
    std::function<void()>   m_ClickCallback[(int)EButtonEventState::Max];
    bool    m_MouseHovered;
    CUIButton();
    CUIButton(const CUIButton& button);
    virtual ~CUIButton();
public:
    UIWidgetImageInfo GetTextureInfo(int index)	const
    {
        return m_TextureInfo[index];
    }
    EButtonState GetState()	const
    {
        return m_State;
    }
    bool GetButtonEnable()	const
    {
        return m_State != EButtonState::Disable;
    }
    void ButtonEnable(bool enable)
    {
        m_State = enable ? EButtonState::Normal : EButtonState::Disable;
    }
    void SetTexture(EButtonState state, CTexture* texture);
    bool SetTexture(EButtonState state, const std::string& name, const TCHAR* fileName, 
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(EButtonState state, const std::string& name, const TCHAR* fullPath);
    bool SetTexture(EButtonState state, const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(EButtonState state, const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(EButtonState state, const Vector4& tint);
    void SetImageTint(EButtonState state, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(EButtonState state, const Vector2& start, const Vector2& end);
    void SetPlayTime(EButtonState state, float playTime);
    void SetPlayScale(EButtonState state, float playScale);
    void SetSound(EButtonEventState state, class CSound* sound);
    void SetSound(EButtonEventState state, const std::string& name);
    bool SetSound(EButtonEventState state, const std::string& groupName, const std::string& name, bool loop,
        const char* fileName, const std::string& pathName = SOUND_PATH);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUIButton* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    template <typename T>
    void SetCallback(EButtonEventState state, T* obj, void(T::* func)())
    {
        m_ClickCallback[(int)state] = std::bind(func, obj);
    }
};

