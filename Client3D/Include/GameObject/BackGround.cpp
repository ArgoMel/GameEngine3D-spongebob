#include "BackGround.h"
#include "Input.h"
#include "Device.h"
#include "PathManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Scene/SceneManager.h"
#include "Resource/Material/Material.h"
#include "../Scene/LoadingSceneInfo.h"

CBackGround::CBackGround()
{
	SetTypeID<CBackGround>();
	m_ObjectTypeName = "BackGround";
}

CBackGround::CBackGround(const CBackGround& obj)
	: CGameObject(obj)
{
	m_BG = (CSpriteComponent*)FindComponent("Background");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Body = (CColliderPixel*)FindComponent("Body");
}

CBackGround::~CBackGround()
{
}

void CBackGround::Destroy()
{
	CGameObject::Destroy();
	CInput::GetInst()->ClearCallback();
}

void CBackGround::Start()
{
	CGameObject::Start();
	if (m_Scene)
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
		std::string imgPath = "PixelCollider\\" + m_Scene->GetName() + ".png";
		TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
		t_filename[imgPath.size()] = 0;
		std::copy(imgPath.begin(), imgPath.end(), t_filename);
		m_Body->SetInfo(m_Scene->GetName(), t_filename);
		SAFE_DELETE_ARRAY(t_filename);
	}
	if(!&m_Body->GetInfo())
	{
		m_Body->SetInfo("PixelCollision", TEXT("PixelCollider\\sample.png"));
		m_Body->SetEnable(false);
	}
	m_Body->SetPixelColorCollisionType(EPixelCollision_Type::Color_Confirm);
	m_Body->SetPixelColor(255, 0, 255);

	CInput::GetInst()->AddBindFunction<CBackGround>("Touch", Input_Type::Down, this, &CBackGround::TouchScreen, m_Scene);
}

bool CBackGround::Init()
{
	CGameObject::Init();
	m_Body = CreateComponent<CColliderPixel>("Body");
	m_BG = CreateComponent<CSpriteComponent>("Background");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");

	m_Body->AddChild(m_BG);
	m_BG->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);
	Resolution RS = CDevice::GetInst()->GetResolution();
	float width = (float)RS.width / 2.f;
	float height = (float)RS.height / 2.f;
	m_Arm->SetTargetOffset(Vector3(-width, -height, 0.f));

	m_Body->SetWorldPosition(width, height);
	m_BG->SetRelativeScale(850.f, 1191.f);
	m_BG->SetPivot(0.5f, 0.5f);
	m_BG->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, "TitleBG", TEXT("BG//Title_height.jpg"));
	m_BG->SetRenderLayerName("Back");
	return true;
}

void CBackGround::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CBackGround::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBackGround* CBackGround::Clone() const
{
	return new CBackGround(*this);
}

void CBackGround::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CBackGround::Load(FILE* file)
{
	CGameObject::Load(file);

}

void CBackGround::TouchScreen()
{
	if (m_Scene->GetName()=="Title")
	{
		m_Scene->GetResource()->SoundPlay("ButtonClick1");
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false,"Lobby.scn","Title.scn");
	}
}

