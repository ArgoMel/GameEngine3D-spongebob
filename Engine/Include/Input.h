#pragma once
#include "EngineInfo.h"
#define	DIK_MOUSELBUTTON	0xfc
#define	DIK_MOUSERBUTTON	0xfd
#define	DIK_MOUSEWBUTTON	0xfe

enum class InputSystem_Type
{
	DInput,	//�Ϲ����� �׷���ī�忡�� ���. ����
	Window	//�󵥿¿��� ���
};

enum class Input_Type
{
	Down,
	Stay,
	Up,
	End
};

struct KeyState
{
	unsigned char	key;
	bool	down;	// ������ �����Ҷ�
	bool	stay;	// ������ ������
	bool	up;		// ������ �ִ� Ű�� �ö�ö�

	KeyState() 
		: key(0)
		, down(false)
		, stay(false)
		, up(false)
	{
	}
};

struct BindFunction
{
	void* obj;
	class CScene* scene;
	std::function<void()>	func;

	BindFunction() 
		: obj(nullptr)
		, scene(nullptr)
	{
	}
};

struct BindKey
{
	std::string	name; // �̸�
	KeyState* key;		// � Ű�� ����ϴ���.
	std::vector<BindFunction*>	vecFunction[(int)Input_Type::End];
	bool	ctrl;
	bool	alt;
	bool	shift;

	BindKey() 
		: key(nullptr)
		, ctrl(false)
		, alt(false)
		, shift(false)
	{
	}
};

class CInput
{
	DECLARE_SINGLE(CInput)
private:
	friend class CDefaultSetting;
	IDirectInput8* m_Input;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	CSharedPtr<class CUIImage> m_mouseImg;
	CSharedPtr<class CUIWindow> m_mouseWindow;
	HWND	m_hWnd;
	DIMOUSESTATE	m_MouseState;
	InputSystem_Type	m_InputSystem;
	Ray		m_Ray;				//����Ƽ�� raycasthit������
	Vector2	m_MousePos;			// ������ â������ ��ġ
	Vector3	m_MouseWorldPos;	// ������������� ���콺 ��ġ
	Vector2	m_MouseUIPos;
	Vector2	m_MouseMove;
	Vector2	m_MouseMove2D;
	unsigned char	m_KeyArray[256];
	bool	m_Ctrl;
	bool	m_Alt;
	bool	m_Shift;
	bool	m_MouseLDown;
	bool	m_MouseLStay;
	bool	m_MouseLUp;
	bool	m_ShowCursor;
	bool	m_IsMouseImg;
	bool	m_CollisionWidget;
	short	m_Wheel;
	std::unordered_map<unsigned char, KeyState*>	m_mapKeyState;
	std::unordered_map<std::string, BindKey*>		m_mapBindKey;
	bool InitDirectInput();
	void ReadDirectInputKeyboard();
	void ReadDirectInputMouse();
	void UpdateMouse(float deltaTime);
	void UpdateKeyState(float deltaTime);
	void UpdateBindKey(float deltaTime);
	void SetKeyCtrl(const std::string& name, bool ctrl = true);
	void SetKeyAlt(const std::string& name, bool alt = true);
	void SetKeyShift(const std::string& name, bool shift = true);
	KeyState* FindKeyState(unsigned char key);
	BindKey* FindBindKey(const std::string& name);
	unsigned char ConvertKey(unsigned char key);
public:
	CUIImage* GetMouseImg() const
	{
		return m_mouseImg;
	}
	const Ray& GetRay()	const
	{
		return m_Ray;
	}
	const Vector2& GetMousePos()	const
	{
		return m_MousePos;
	}
	const Vector3& GetMouseWorldPos()	const
	{
		return m_MouseWorldPos;
	}
	const Vector2& GetMouseUIPos()	const
	{
		return m_MouseUIPos;
	}
	const Vector2& GetMouseMove()	const
	{
		return m_MouseMove;
	}
	const Vector2& GetMouseMove2D()	const
	{
		return m_MouseMove2D;
	}
	bool GetMouseLDown()	const
	{
		return m_MouseLDown;
	}
	bool GetMouseLPush()	const
	{
		return m_MouseLStay;
	}
	bool GetMouseLUp()	const
	{
		return m_MouseLUp;
	}
	short GetMouseWheel()	const
	{
		return m_Wheel;
	}
	void SetWheel(short wheel)
	{
		m_Wheel = wheel;
	}
	void ComputeWorldMousePos(class CCameraComponent* camera);
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	bool AddBindKey(const std::string& name, unsigned char key);
	void ClearCallback();
	void ClearCallback(class CScene* scene);
	void InitMouseImg();
	void SetMouseImgVisible();
	void SetMouseVisible();
	template <typename T>
	void AddBindFunction(const std::string& keyName, Input_Type type, T* object, void (T::* func)(), class CScene* scene)
	{
		BindKey* key = FindBindKey(keyName);
		if (!key)
		{
			return;
		}
		BindFunction* function = new BindFunction;
		function->obj = object;
		function->func = std::bind(func, object);			// ����Լ��� ����Ҷ� �Լ��ּ�, ��ü�ּҸ� ����ؾ� �Ѵ�.
		function->scene = scene;
		key->vecFunction[(int)type].push_back(function);
	}
	template <typename T>
	bool DeleteBindFunction(const std::string& name, Input_Type type, T* obj)
	{
		BindKey* key = FindBindKey(name);
		if (!key)
		{
			return false;
		}
		auto	iter = key->vecFunction[(int)type].begin();
		auto	iterEnd = key->vecFunction[(int)type].end();
		for (; iter != iterEnd;)
		{
			if ((*iter)->obj == obj)
			{
				SAFE_DELETE((*iter));
				iter = key->vecFunction[(int)type].erase(iter);
				iterEnd = key->vecFunction[(int)type].end();
				continue;
			}
			++iter;
		}
		return true;
	}
};

