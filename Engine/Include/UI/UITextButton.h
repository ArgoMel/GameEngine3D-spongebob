#pragma once
#include "UIWidget.h"
#include "../Resource/Font/Font.h"

struct UIWidgetTextInfo
{
	ID2D1SolidColorBrush* brush;
	TCHAR*		text;
	Text_Align_H    alignH;
	Text_Align_V    alignV;
	Vector4     color;
	int			textCount;
	int			textCapacity;
	float       fontSize;
	float       opacity;
	// Shadow
	ID2D1SolidColorBrush* shadowBrush;
	Vector4     shadowColor;
	Vector2     shadowOffset;
	UIWidgetTextInfo()
		: fontSize(10.f)
		, opacity(1.f)
		, color(Vector4::black)
		, brush(nullptr)
		, alignH(Text_Align_H::Center)
		, alignV(Text_Align_V::Middle)
		, shadowBrush(nullptr)
		, shadowColor(Vector4::black)
		, shadowOffset(1.1f, 1.1f)
	{
		color.w = opacity;
		textCapacity = 32;
		textCount = 0;
		text = new TCHAR[textCapacity];
		_tcscpy_s(text, textCapacity, TEXT("\n"));
	}
	~UIWidgetTextInfo()
	{
		SAFE_DELETE_ARRAY(text);
	}
};

class CUITextButton : public CUIWidget
{
private:
	friend class CUIWindow;
	friend class CScene;
	void CreateLayout(EButtonState state);
protected:
	CSharedPtr<CFont> m_Font;
	IDWriteTextLayout* m_Layout;
	ID2D1RenderTarget* m_RenderTarget;
	UIWidgetTextInfo            m_TextInfo[(int)EButtonState::Max];
	CSharedPtr<class CSound>    m_Sound[(int)EButtonEventState::Max];
	EButtonState                m_State;
	std::function<void()>       m_ClickCallback[(int)EButtonEventState::Max];
	bool						m_MouseHovered;
	CUITextButton();
	CUITextButton(const CUITextButton& button);
	virtual ~CUITextButton();
public:
	void ButtonEnable(bool enable)
	{
		m_State = enable ? EButtonState::Normal : EButtonState::Disable;
	}
	void SetFont(const std::string& name);
	void SetText(EButtonState state, const TCHAR* text, float size, const Vector4& color);
	void SetSound(EButtonEventState state, class CSound* sound);
	void SetSound(EButtonEventState state, const std::string& name);
	bool SetSound(EButtonEventState state, const std::string& groupName, const std::string& name, 
		bool loop, const char* fileName, const std::string& pathName = SOUND_PATH);
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CUITextButton* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	template <typename T>
	void SetCallback(EButtonEventState state, T* obj, void(T::* func)())
	{
		m_ClickCallback[(int)state] = std::bind(func, obj);
	}
};

