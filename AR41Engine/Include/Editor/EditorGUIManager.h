#pragma once
#include "EditorInfo.h"
#include "EditorWindow.h"

class CEditorGUIManager
{
	DECLARE_SINGLE(CEditorGUIManager)
private:
	std::unordered_map<std::string, CEditorWindow*>	m_mapWindow;
	ImGuiContext* m_Context;
	ImFont* m_Font;
public:
	bool IsWindowEmpty()
	{
		return m_mapWindow.empty();
	}
	bool Init(HWND hWnd);
	void Update(float deltaTime);
	void Render();
	void SetFont(const std::string& name, const char* fileName, float size, int overH = 2, int overV = 1, 
		float spacing = 1.f, const std::string& pathName = FONT_PATH);
	void EnableFont();
	void DisableFont();
	void OnOffEditorWindow(bool b);
	template <typename T>
	T* FindEditorWindow(const std::string& name)
	{
		auto	iter = m_mapWindow.find(name);
		if (iter == m_mapWindow.end())
		{
			return nullptr;
		}
		return (T*)iter->second;
	}
	template <typename T>
	T* CreateEditorWindow(const std::string& name)
	{
		T* window = FindEditorWindow<T>(name);
		if (window)
		{
			return window;
		}
		window = new T;
		window->SetName(name);
		if (!window->Init())
		{
			SAFE_DELETE(window);
			return nullptr;
		}
		m_mapWindow.insert(std::make_pair(name, window));
		return window;
	}
};

