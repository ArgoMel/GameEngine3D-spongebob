#pragma once
#include "EngineInfo.h"
#include "EngineSetting.h"

class CEngine
{
	DECLARE_SINGLE(CEngine)
private:
	static bool		m_Loop;
	static bool		m_EditorMode;
	static std::function<bool(HWND, UINT, WPARAM, LPARAM)>	m_WndProcFunc;
	class CTimer* m_Timer;
	class CServer* m_Server;
	class CGlobalConstantBuffer* m_GlobalCBuffer;
	class CStructuredBuffer* m_RandomBuffer;
	CEngineSetting* m_Setting;
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_WindowRS;
	float		m_ClearColor[4];
	float		m_TimeScale;	// 글로벌 타임 재생 배율(게임 전체의 시간을 조절할때 씀)
	float		m_AccTime;
	bool		m_Render2D;
	void Logic();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);
	bool Collision(float deltaTime);
	void Render(float deltaTime);
	void Register(const TCHAR* className, int iconID, int smallIconID, int menuID);
	bool Create(const TCHAR* title, const TCHAR* className);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	static bool GetEditorMode()
	{
		return m_EditorMode;
	}
	static void EnableEditor()
	{
		m_EditorMode = true;
	}
	class CGlobalConstantBuffer* GetGlobalCBuffer()	const
	{
		return m_GlobalCBuffer;
	}
	HINSTANCE GetWindowInst()	const
	{
		return m_hInst;
	}
	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}
	Resolution GetWindowResolution()	const
	{
		return m_WindowRS;
	}
	float GetTimeScale()	const
	{
		return m_TimeScale;
	}
	void SetTimeScale(float scale)
	{
		m_TimeScale = scale;
	}
	float GetFPS()	const;
	void SetServerText(const char* name, const char* text);
	bool GetRender2D()	const
	{
		return m_Render2D;
	}
	void EnableRender2D()
	{
		m_Render2D = true;
	}
	void SetCameraAxisX(const Vector3& axis);
	void SetCameraAxisY(const Vector3& axis);
	bool Init(HINSTANCE hInst, const TCHAR* title,const TCHAR* className, int iconID, int smallIconID,
		unsigned int windowWidth,unsigned int windowHeight,unsigned int deviceWidth, unsigned int deviceHeight,
		bool windowMode = true, int menuID = 0);
	int Run();
	void Exit();
	template <typename T>
	static void SetWndProcCallback(T* obj, bool(T::* func)(HWND, UINT, WPARAM, LPARAM))
	{
		m_WndProcFunc = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3, std::placeholders::_4);
	}
	template <typename T>
	void SetEngineSetting()
	{
		SAFE_DELETE(m_Setting);
		m_Setting = new T;
		m_Setting->Init();
	}
};

