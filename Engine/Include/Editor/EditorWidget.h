#pragma once
#include "EditorInfo.h"

class CEditorWidget
{
protected:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	class CEditorWindow* m_Owner;
	std::string		m_Name;
	std::string		m_NameUTF8;
	ImVec2			m_Size;
	ImVec4			m_Color;
	bool			m_IsHide;
	CEditorWidget();
	virtual ~CEditorWidget();
public:
	const std::string& GetName()	const
	{
		return  m_Name;
	}
	bool GetIsHide()
	{
		return m_IsHide;
	}
	void SetName(const std::string& name)
	{
		TCHAR	wName[256] = {};
		int length = (int)MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, wName, length);
		char	nameUTF8[256] = {};
		length = WideCharToMultiByte(CP_UTF8, 0, wName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_UTF8, 0, wName, -1, nameUTF8, length, nullptr, nullptr);
		m_NameUTF8 = nameUTF8;
		m_Name = name;
	}
	void SetHideName(const std::string& name)
	{
		m_IsHide = true;
		m_Name = "##" + name;	//ÀÌ·¯¸é ¼û°ÜÁü
		TCHAR	wName[256] = {};
		int length = (int)MultiByteToWideChar(CP_ACP, 0, m_Name.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, m_Name.c_str(), -1, wName, length);
		char	nameUTF8[256] = {};
		length = WideCharToMultiByte(CP_UTF8, 0, wName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_UTF8, 0, wName, -1, nameUTF8, length, nullptr, nullptr);
		m_NameUTF8 = nameUTF8;
	}
	void SetSize(const ImVec2& size)
	{
		m_Size = size;
	}
	void SetSize(const Vector2& size)
	{
		m_Size.x = size.x;
		m_Size.y = size.y;
	}
	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}
	void SetSizeX(float x)
	{
		m_Size.x = x;
	}
	void SetSizeY(float y)
	{
		m_Size.y = y;
	}
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Color.x = r / 255.f;
		m_Color.y = g / 255.f;
		m_Color.z = b / 255.f;
		m_Color.w = a / 255.f;
	}
	virtual bool Init() = 0;
	virtual void Render() = 0;
};

