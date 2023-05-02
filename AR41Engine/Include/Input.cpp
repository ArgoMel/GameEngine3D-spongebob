#include "Input.h"
#include "Device.h"
//#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneCollision.h"
#include "Component/CameraComponent.h"
#include "UI/UIImage.h"
#pragma comment(lib, "dinput8.lib")
DEFINITION_SINGLE(CInput)

CInput::CInput() 
	: m_MouseLDown(false)
	, m_MouseLStay(false)
	, m_MouseLUp(false)
	, m_InputSystem(InputSystem_Type::DInput)
	, m_Input(nullptr)
	, m_Keyboard(nullptr)
	, m_Mouse(nullptr)
	, m_KeyArray{}
	, m_MouseState{}
	, m_CollisionWidget(false)
	, m_IsMouseImg(false)
	, m_ShowCursor(false)
	, m_Wheel(0)
{
}

CInput::~CInput()
{
	{
		auto	iter = m_mapKeyState.begin();
		auto	iterEnd = m_mapKeyState.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_mapKeyState.clear();
	}
	{
		auto	iter = m_mapBindKey.begin();
		auto	iterEnd = m_mapBindKey.end();
		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (int)Input_Type::End; ++i)
			{
				size_t	size = iter->second->vecFunction[i].size();
				for (size_t j = 0; j < size; ++j)
				{
					SAFE_DELETE(iter->second->vecFunction[i][j]);
				}
			}
			SAFE_DELETE(iter->second);
		}
		m_mapBindKey.clear();
	}
	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);
	SAFE_RELEASE(m_Input);
}

bool CInput::InitDirectInput()
{
	if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr)))
	{
		return false;
	}
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}
	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr)))
	{
		return false;
	}
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}
	m_Keyboard->Acquire();
	m_Mouse->Acquire();
	return true;
}

void CInput::ReadDirectInputKeyboard()
{
	HRESULT	result = m_Keyboard->GetDeviceState(256, m_KeyArray);
	//if (FAILED(result))
	//{
	//	if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	//	{
	//		m_Keyboard->Acquire();
	//	}
	//}
}

void CInput::ReadDirectInputMouse()
{
	HRESULT	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	//if (FAILED(result))
	//{
	//	if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	//	{
	//		m_Mouse->Acquire();
	//	}
	//}
}

void CInput::UpdateMouse(float deltaTime)
{
	POINT	ptMouse;
	GetCursorPos(&ptMouse);	// 스크린 좌표로 나온다.
	ScreenToClient(m_hWnd, &ptMouse);	// 스크린 좌표를 윈도우 좌표로 변경한다.
	RECT	windowRC;
	GetClientRect(m_hWnd, &windowRC);
	Vector2	resolutionRatio = CDevice::GetInst()->GetResolutionRatio();
	Resolution RS = CDevice::GetInst()->GetResolution();
	Vector2	mousePos;
	mousePos.x = (float)ptMouse.x * resolutionRatio.x;
	mousePos.y = (float)ptMouse.y * resolutionRatio.y;
	m_MouseUIPos = mousePos;
	m_MouseUIPos.y = RS.height - m_MouseUIPos.y;
	Vector2	mouse2DPos = m_MousePos;
	mouse2DPos.y = RS.height - mouse2DPos.y;
	m_MouseMove2D = m_MouseUIPos - mouse2DPos;
	m_MouseMove = mousePos - m_MousePos;
	m_MousePos = mousePos;
	if (!m_IsMouseImg)
	{
		InitMouseImg();
		m_IsMouseImg = true;
	}
	m_mouseImg->SetPos(m_MouseUIPos);
	m_mouseWindow->Update(deltaTime);

	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	ComputeWorldMousePos(camera);

	m_Ray.dir.x = m_MousePos.x / RS.width;
	m_Ray.dir.y = m_MousePos.y / RS.height;
	m_Ray.dir.x = m_Ray.dir.x * 2.f - 1.f;
	m_Ray.dir.y = m_Ray.dir.y * -2.f + 1.f;
	Matrix	matProj = camera->GetProjMatrix();
	m_Ray.dir.x /= matProj._11;
	m_Ray.dir.y /= matProj._22;
	m_Ray.dir.z = 1.f;	//뷰공간에서의 반직선은 +z방향이다
	m_Ray.dir.Normalize();
	m_Ray.pos = Vector3(0.f, 0.f, 0.f);
}

void CInput::UpdateKeyState(float deltaTime)
{
	switch (m_InputSystem)
	{
	case InputSystem_Type::DInput:
		if (m_KeyArray[DIK_LCONTROL] & 0x80)
		{
			m_Ctrl = true;
		}
		else
		{
			m_Ctrl = false;
		}
		if (m_KeyArray[DIK_LALT] & 0x80)
		{
			m_Alt = true;
		}
		else
		{
			m_Alt = false;
		}
		if (m_KeyArray[DIK_LSHIFT] & 0x80)
		{
			m_Shift = true;
		}
		else
		{
			m_Shift = false;
		}
		if (m_MouseState.rgbButtons[0] & 0x80)
		{
			if (!m_MouseLDown && !m_MouseLStay)
			{
				m_MouseLDown = true;
				m_MouseLStay = true;
			}
			else
			{
				m_MouseLDown = false;
			}
		}
		else if (m_MouseLStay)
		{
			m_MouseLDown = false;
			m_MouseLStay = false;
			m_MouseLUp = true;
		}
		else if (m_MouseLUp)
		{
			m_MouseLUp = false;
		}
		break;
	case InputSystem_Type::Window:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			m_Ctrl = true;
		}
		else
		{
			m_Ctrl = false;
		}
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			m_Alt = true;
		}
		else
		{
			m_Alt = false;
		}
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			m_Shift = true;
		}
		else
		{
			m_Shift = false;
		}
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!m_MouseLDown && !m_MouseLStay)
			{
				m_MouseLDown = true;
				m_MouseLStay = true;
			}
			else
			{
				m_MouseLDown = false;
			}
		}
		else if (m_MouseLStay)
		{
			m_MouseLDown = false;
			m_MouseLStay = false;
			m_MouseLUp = true;
		}
		else if (m_MouseLUp)
		{
			m_MouseLUp = false;
		}
		break;
	}
	auto	iter = m_mapKeyState.begin();
	auto	iterEnd = m_mapKeyState.end();
	for (; iter != iterEnd; ++iter)
	{
		bool	keyStay = false;
		switch (m_InputSystem)
		{
		case InputSystem_Type::DInput:
			switch (iter->second->key)
			{
			case DIK_MOUSELBUTTON:
				if (m_MouseState.rgbButtons[0] & 0x80)
				{
					keyStay = true;
				}
				break;
			case DIK_MOUSERBUTTON:
				if (m_MouseState.rgbButtons[1] & 0x80)
				{
					keyStay = true;
				}
				break;
			case DIK_MOUSEWBUTTON:
				if (m_MouseState.rgbButtons[2] & 0x80)
				{
					keyStay = true;
				}
				break;
			default:	// 일반 키보드 키일때
				if (m_KeyArray[iter->second->key] & 0x80)
				{
					keyStay = true;
				}
				break;
			}
			break;
		case InputSystem_Type::Window:
			if (GetAsyncKeyState(iter->second->key) & 0x8000)
			{
				keyStay = true;
			}
			break;
		}
		if (keyStay)			// 키를 눌렀을 경우
		{
			// Down과 stay 모두 false라면 이 키를 지금 누른 것이다.
			// 그러므로 둘다 true로 변경한다.
			if (!iter->second->down && !iter->second->stay)
			{
				iter->second->down = true;
				iter->second->stay = true;
			}
			// Down과 Push 둘중 하나라도 true라면 Down은 false가
			// 되어야 한다. stay는 이미 위에서 true로 변경되었다.
			else
			{
				iter->second->down = false;
			}
		}
		// 키가 안눌러졌을 경우 stay가 true라면
		// 이전 프레임에 키를 누르고 있다가 지금 떨어졌다는 것이다.
		else if (iter->second->stay)
		{
			iter->second->up = true;
			iter->second->stay = false;
			iter->second->down = false;
		}
		else if (iter->second->up)
		{
			iter->second->up = false;
		}
	}
}

void CInput::UpdateBindKey(float deltaTime)
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->key->down &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			size_t	size = iter->second->vecFunction[(int)Input_Type::Down].size();
			for (size_t i = 0; i < size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Down][i]->func();
			}
		}
		if (iter->second->key->stay &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			size_t	size = iter->second->vecFunction[(int)Input_Type::Stay].size();
			for (size_t i = 0; i < size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Stay][i]->func();
			}
		}
		if (iter->second->key->up &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			size_t	size = iter->second->vecFunction[(int)Input_Type::Up].size();
			for (size_t i = 0; i < size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Up][i]->func();
			}
		}
	}
}

void CInput::SetKeyCtrl(const std::string& name, bool ctrl)
{
	BindKey* key = FindBindKey(name);
	if (!key)
	{
		return;
	}
	key->ctrl = ctrl;
}

void CInput::SetKeyAlt(const std::string& name, bool alt)
{
	BindKey* key = FindBindKey(name);
	if (!key)
	{
		return;
	}
	key->alt = alt;
}

void CInput::SetKeyShift(const std::string& name, bool shift)
{
	BindKey* key = FindBindKey(name);
	if (!key)
	{
		return;
	}
	key->shift = shift;
}

KeyState* CInput::FindKeyState(unsigned char key)
{
	auto	iter = m_mapKeyState.find(key);
	if (iter == m_mapKeyState.end())
	{
		return nullptr;
	}
	return iter->second;
}

BindKey* CInput::FindBindKey(const std::string& name)
{
	auto	iter = m_mapBindKey.find(name);
	if (iter == m_mapBindKey.end())
	{
		return nullptr;
	}
	return iter->second;
}

unsigned char CInput::ConvertKey(unsigned char key)
{
	if (m_InputSystem == InputSystem_Type::DInput)
	{
		switch (key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWBUTTON;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
		//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}
		return 0xff;
	}
	return key;
}

void CInput::ComputeWorldMousePos(class CCameraComponent* camera)
{
	Vector3 offset;
	if(camera->GetParent())
	{
		offset = camera->GetParent()->GetRelativePos();
	}
	Resolution RS = CDevice::GetInst()->GetResolution();
	Vector3 cameraPos=camera->GetWorldPos();
	m_MouseWorldPos.x = m_MousePos.x + cameraPos.x - offset.x- RS.width*0.5f;
	m_MouseWorldPos.y = RS.height- m_MousePos.y + cameraPos.y - offset.y - RS.height * 0.5f;
	m_MouseWorldPos.z = 1.f;//cameraPos.z- offset.z;
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hWnd = hWnd;
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, nullptr)))
	{
		m_InputSystem = InputSystem_Type::Window;
	}
	if (m_InputSystem == InputSystem_Type::DInput)
	{
		if (!InitDirectInput())
		{
			return false;
		}
	}
	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;
	ShowCursor(FALSE);
	AddBindKey("F9", VK_F10);
	AddBindKey("F11", VK_F11);
	AddBindFunction<CInput>("F9", Input_Type::Down, this, &CInput::SetMouseImgVisible,
		CSceneManager::GetInst()->GetScene());
	AddBindFunction<CInput>("F11", Input_Type::Down, this, &CInput::SetMouseVisible,
		CSceneManager::GetInst()->GetScene());
	return true;
}

void CInput::Update(float deltaTime)
{
	if (m_InputSystem == InputSystem_Type::DInput)
	{
		ReadDirectInputKeyboard();
		ReadDirectInputMouse();
	}
	UpdateMouse(deltaTime);
	m_CollisionWidget = CSceneManager::GetInst()->GetScene()->GetCollisionManager()->CollisionWidget();
	UpdateKeyState(deltaTime);
	UpdateBindKey(deltaTime);
}

void CInput::PostUpdate(float deltaTime)
{
	m_mouseWindow->PostUpdate(deltaTime);
}

bool CInput::AddBindKey(const std::string& name, unsigned char key)
{
	if (FindBindKey(name))
	{
		return false;
	}
	BindKey* newKey = new BindKey;
	KeyState* state = FindKeyState(key);
	if (!state)
	{
		state = new KeyState;
		state->key = ConvertKey(key);
		m_mapKeyState.insert(std::make_pair(key, state));
	}
	newKey->key = state;
	newKey->name = name;
	m_mapBindKey.insert(std::make_pair(name, newKey));
	return true;
}

void CInput::ClearCallback()
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();
	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)Input_Type::End; ++i)
		{
			size_t	size = iter->second->vecFunction[i].size();
			for (size_t j = 0; j < size; ++j)
			{
				SAFE_DELETE(iter->second->vecFunction[i][j]);
			}
			iter->second->vecFunction[i].clear();
		}
	}
}

void CInput::ClearCallback(CScene* scene)
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();
	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)Input_Type::End; ++i)
		{
			auto	iter1 = iter->second->vecFunction[i].begin();
			auto	iter1End = iter->second->vecFunction[i].end();
			for (; iter1 != iter1End;)
			{
				if ((*iter1)->scene == scene)
				{
					SAFE_DELETE((*iter1));
					iter1 = iter->second->vecFunction[i].erase(iter1);
					iter1End = iter->second->vecFunction[i].end();
					continue;
				}
				++iter1;
			}
		}
	}
	m_mouseImg=nullptr;
	m_mouseWindow =nullptr;
	m_IsMouseImg = false;
}

void CInput::InitMouseImg()
{
	if (!m_mouseWindow)
	{
		m_mouseWindow = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateUIWindow<CUIWindow>("Mouse");
	}
	if (!m_mouseImg)
	{
		m_mouseImg = m_mouseWindow->CreateWidget<CUIImage>("Mouse");
	}
	m_mouseImg->SetSize(32.f, 31.f);
	m_mouseImg->SetPivot(0.f, 1.f);
	m_mouseImg->SetTexture("Mouse", TEXT("Mouse\\Default\\0.png"));
}

void CInput::SetMouseImgVisible()
{
	if(m_mouseImg->GetEnable()) 
	{
		m_mouseImg->SetEnable(false);
		m_ShowCursor = false;
		ShowCursor(FALSE);
	}
	else
	{
		m_mouseImg->SetEnable(true);
		m_ShowCursor = true;
		ShowCursor(TRUE);
	}
}

void CInput::SetMouseVisible()
{
	if(m_ShowCursor)
	{
		m_ShowCursor = false;
		ShowCursor(FALSE);
	}
	else
	{
		m_ShowCursor = true;
		ShowCursor(TRUE);
	}
}
