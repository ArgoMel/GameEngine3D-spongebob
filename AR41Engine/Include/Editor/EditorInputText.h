#pragma once
#include "EditorWidget.h"

enum class EImGuiInputType
{
	String,
	Int,
	Float
};

class CEditorInputText : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	ImGuiInputTextFlags	m_Flag;
	EImGuiInputType	m_InputType;
	std::function<void()>	m_InputCallback;
	char		m_Text[1024];
	wchar_t		m_wText[2048];
	TCHAR		m_TText[2048];
	char		m_TextUTF8[1024];
	char		m_HintText[1024];
	wchar_t		m_HintwText[2048];
	TCHAR		m_HintTText[2048];
	char		m_HintTextUTF8[1024];
	bool			m_MultiLine;
	int				m_ValueInt;
	float			m_ValueFloat;
protected:
	CEditorInputText();
	virtual ~CEditorInputText();
public:
	EImGuiInputType GetInputType()	const
	{
		return m_InputType;
	}
	const char* GetText()	const
	{
		return m_Text;
	}
	const wchar_t* GetWText()	const
	{
		return m_wText;
	}
	const TCHAR* GetTText()	const
	{
		return m_TText;
	}
	const char* GetTextUTF8()	const
	{
		return m_TextUTF8;
	}
	bool GetMultiLine()	const
	{
		return m_MultiLine;
	}
	int GetInt()	const
	{
		return m_ValueInt;
	}
	float GetFloat()	const
	{
		return m_ValueFloat;
	}
	void SetText(const char* text);
	void SetHintText(const char* text);
	void SetInt(int value);
	void SetFloat(float value);
	void SetMultiLine(bool multiLine);
	void AddFlag(ImGuiInputTextFlags_ flag);
	void ReadOnly(bool enable);
	void PasswordMode(bool enable);
	void SetInputType(EImGuiInputType type);
	bool Empty()	const;
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetInputCallback(T* obj, void(T::* func)())
	{
		m_InputCallback = std::bind(func, obj);
	}
};

