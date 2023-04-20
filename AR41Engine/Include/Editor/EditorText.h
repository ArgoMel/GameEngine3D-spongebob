#pragma once
#include "EditorWidget.h"
class CEditorText :public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	char		m_Text[1024];
	wchar_t		m_wText[2048];
	char		m_TextUTF8[1024];
	bool m_IsMultiLine;
	std::function<void()>	m_TextCallback;
protected:
	CEditorText();
	virtual ~CEditorText();
public:
	const char* GetText()	const
	{
		return m_Text;
	}
	const wchar_t* GetWText()	const
	{
		return m_wText;
	}
	const char* GetTextUTF8()	const
	{
		return m_TextUTF8;
	}
	bool GetMultiLine()	const
	{
		return m_IsMultiLine;
	}
	void SetMultiLine(bool b)
	{
		m_IsMultiLine = b;
	}
	void SetText(const char* text);
	void AddText(const char* text);
	bool Empty()	const;
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetTextCallback(T* obj, void(T::* func)())
	{
		m_TextCallback = std::bind(func, obj);
	}
};

