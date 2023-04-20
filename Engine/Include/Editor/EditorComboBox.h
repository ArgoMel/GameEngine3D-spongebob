#pragma once
#include "EditorWidget.h"

class CEditorComboBox :public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	std::string					m_PrevViewName;
	std::string					m_PrevViewNameUTF8;
	ImGuiComboFlags				m_Flag;
	std::function<void(int, const std::string&)>	m_SelectCallback;
	bool						m_Sort;
	bool						m_Select;
	bool						m_SelectPrevViewName;
	int							m_SelectIndex;
	int							m_PageItemCount;
protected:
	CEditorComboBox();
	virtual ~CEditorComboBox();
public:
	const std::string& GetItem(int index)	const
	{
		return m_vecItem[index];
	}
	const std::string& GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex];
	}
	int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}
	void SetPrevViewName(const std::string& name)
	{
		TCHAR	wText[256] = {};
		int length = (int)MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, wText, length);
		char	textUTF8[256] = {};
		length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
		m_PrevViewName = name;
		m_PrevViewNameUTF8 = textUTF8;
	}
	void SetSelectPrevViewName(bool enable)
	{
		m_SelectPrevViewName = enable;
	}
	void SetSelectIndex(int index)
	{
		m_SelectIndex = index;
		SetPrevViewName(GetItem(m_SelectIndex));
	}
	void SetPageItemCount(int count)
	{
		m_PageItemCount = count;
	}
	bool CheckItem(const std::string& item)	const
	{
		size_t	size = m_vecItem.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_vecItem[i] == item)
			{
				return true;
			}
		}
		return false;
	}
	void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
	}
	void AddFlag(ImGuiComboFlags_ flag)
	{
		m_Flag |= flag;
	}
	void AddItem(const std::string& item);
	void DeleteItem(const std::string& item);
	void DeleteItem(int index);
	void Sort(bool sort);
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetSelectCallback(T* obj, void(T::* func)(int, const std::string&))
	{
		m_SelectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
};

