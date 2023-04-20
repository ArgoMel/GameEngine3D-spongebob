#include "Marker.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/Material/Material.h"

CMarker::CMarker()
{
	SetTypeID<CMarker>();
	m_ObjectTypeName = "Marker";
}

CMarker::CMarker(const CMarker& obj)
	: CGameObject(obj)
{
	m_Body = (CColliderSphere2D*)FindComponent("Body");
	m_Sprite = (CSpriteComponent*)FindComponent("Marker");
}

CMarker::~CMarker()
{
}

void CMarker::Destroy()
{
	CGameObject::Destroy();
}

void CMarker::Start()
{
	CGameObject::Start();
	m_Body->SetCollisionProfile("Marker");

	std::string imgPath = "Character\\marker\\pixelart\\sprite_sheet\\000a.png";
	TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
	t_filename[imgPath.size()] = 0;
	std::copy(imgPath.begin(), imgPath.end(), t_filename);
	m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, "marker", t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	m_Sprite->SetRelativeScale((float)m_Sprite->GetMaterial(0)->GetTexture()->GetWidth() *2.f,
		(float)m_Sprite->GetMaterial(0)->GetTexture()->GetHeight() * 2.f);
//애니메이션
	m_Anim = m_Sprite->SetAnimation<CAnimation2D>("MarkerAnim");
	m_Anim->AddAnimation("Break", "MarkerBreak", 1.f, 2.f);
	m_Anim->GetCurrentAnimationSequence()->Load("marker_break", ANIMATION_PATH);
	m_Anim->SetCurrentEndFunction("Break", this, &CMarker::Destroy);
	m_Anim->AddAnimation("Idle", "MarkerIdle", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Idle");
	m_Anim->GetCurrentAnimationSequence()->Load("marker_idle", ANIMATION_PATH);
}

bool CMarker::Init()
{
	CGameObject::Init();
	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>("Marker");

	m_Body->AddChild(m_Sprite);

	m_Body->SetRadius(25.f);
	m_Sprite->SetRenderLayerName("Effect");
	m_Sprite->SetPivot(0.5f, 0.5f);
	return true;
}

void CMarker::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CMarker::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}	

CMarker* CMarker::Clone() const
{
	return new CMarker(*this);
}

void CMarker::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CMarker::Load(FILE* file)
{
	CGameObject::Load(file);
}

int CMarker::InflictDamage(float damage)
{
	CGameObject::InflictDamage(damage);
	if (m_Anim)
	{
		m_Anim->ChangeAnimation("Break");
	}
	return 0;
}