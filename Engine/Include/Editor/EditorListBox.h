#pragma once
#include "EditorWidget.h"

class CEditorListBox :public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::string m_NullName;
	bool						m_Sort;
	bool						m_Select;
	int							m_SelectIndex;
	int							m_PageItemCount;
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	std::function<void(int, const std::string&)>	m_SelectCallback;
protected:
	CEditorListBox();
	virtual ~CEditorListBox();
public:
	const std::string& GetItem(int index)	const
	{
		return m_vecItem[index];
	}
	const std::string& GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex];
	}
	size_t GetSize()	const
	{
		return m_vecItem.size();
	}
	int GetSelectIndex()	const
	{
		return m_SelectIndex;
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
	const std::string& CheckPartitionItem(size_t index,std::string name)	const
	{
		// m_vecItem[index]의 Index i에서 length개에 해당하는 문자열을 name와 비교
		size_t vecLength = m_vecItem[index].size();
		size_t length = name.size();
		for (size_t i = 0; i < vecLength;++i)
		{
			if (m_vecItem[index].compare(i, length, name) == 0)
			{
				return m_vecItem[index];
			}
		}
		return m_NullName;
	}
	void SetPageItemCount(int count)
	{
		m_PageItemCount = count;
	}
	void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
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

