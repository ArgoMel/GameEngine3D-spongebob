#pragma once
#include "Editor.h"

class CEditorManager
{
private:
	bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SaveScene();
	void OpenScene();
	void CreateEmptyObject();
	void CreateObject();
	void ChatGPT();
	void ChatWindow();
	void WindowOnOff();
public:
	CEditorManager();
	~CEditorManager();
	bool Init(HINSTANCE hInst);
	int Run();
};

