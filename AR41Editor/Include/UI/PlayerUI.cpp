#include "PlayerUI.h"
#include "Device.h"
#include "Engine.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CPlayerUI::CPlayerUI() 
	: m_AllOpacity(3.f)
{
	m_WindowTypeName = "PlayerUI";
}

CPlayerUI::CPlayerUI(const CPlayerUI& Window)
	: CUIWindow(Window)
	, m_AllOpacity(3.f)
{
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = FindWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_CurHP[i] = FindWidget<CUIImage>("CurHP" + std::to_string(i));
	}
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");

	m_SocksText = FindWidget<CUIText>("SocksText");
	m_FritterText = FindWidget<CUIText>("FritterText");
	m_GlitterText = FindWidget<CUIText>("GlitterText");
}

CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::Start()
{
	CUIWindow::Start();
}

bool CPlayerUI::Init()
{
	CUIWindow::Init();
	Resolution	RS = CDevice::GetInst()->GetResolution();
	Vector2 ratio = Vector2((float)RS.width, (float)RS.height)*0.001f;

	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = CreateWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_MaxHP[i]->SetSize(60.f* ratio.x, 60.f* ratio.x);
		m_MaxHP[i]->SetPos(18.f * ratio.x + (m_MaxHP[i]->GetSize().x+5.f)*i, 866.f* ratio.y);
		m_MaxHP[i]->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

		m_CurHP[i] = CreateWidget<CUIImage>("CurHP" + std::to_string(i));
		m_CurHP[i]->SetSize(40.f * ratio.x, 40.f * ratio.x);
		m_CurHP[i]->SetPos(25.f * ratio.x + (m_MaxHP[i]->GetSize().x+5.f) * i , 877.f * ratio.y);
		m_CurHP[i]->SetTexture("CurHP", TEXT("UI/Underpants.png"));
	}

	m_Fritter = CreateWidget<CUIImage>("Fritter");
	m_Fritter->SetSize(110.f * ratio.x, 110.f * ratio.x);
	m_Fritter->SetPos(425.f * ratio.x, 810.f * ratio.y);
	m_Fritter->SetTexture("Fritter", TEXT("UI/Spatula.png"));

	m_Glittering = CreateWidget<CUIImage>("Glittering");
	m_Glittering->SetSize(60.f * ratio.x, 60.f * ratio.x);
	m_Glittering->SetPos(920.f * ratio.x, 860.f * ratio.y);
	m_Glittering->SetTexture("Glittering", TEXT("UI/Flower.png"));

	m_Socks = CreateWidget<CUIImage>("Socks");
	m_Socks->SetSize(90.f * ratio.x, 75.f * ratio.x);
	m_Socks->SetPos(890.f * ratio.x, 27.f * ratio.y);
	m_Socks->SetTexture("Socks", TEXT("UI/Sock.png"));

	m_SocksText = CreateWidget<CUIText>("SocksText");
	m_SocksText->SetSize(60.f * ratio.x, 50.f * ratio.x);
	m_SocksText->SetPos(805.f * ratio.x, 40.f * ratio.y);
	m_SocksText->SetPivot(0.5f, 0.5f);
	m_SocksText->SetFontSize(50.f * ratio.x);
	m_SocksText->SetText(TEXT("00"));
	m_SocksText->SetTransparency(true);
	m_SocksText->SetAlignH(Text_Align_H::Right);

	m_FritterText = CreateWidget<CUIText>("FritterText");
	m_FritterText->SetSize(60.f * ratio.x, 50.f * ratio.x);
	m_FritterText->SetPos(510.f * ratio.x, 867.f * ratio.y);
	m_FritterText->SetPivot(0.5f, 0.5f);
	m_FritterText->SetFontSize(50.f * ratio.x);
	m_FritterText->SetText(TEXT("00"));
	m_FritterText->SetTransparency(true);
	m_FritterText->SetAlignH(Text_Align_H::Right);

	m_GlitterText = CreateWidget<CUIText>("GlitterText");
	m_GlitterText->SetSize(200.f * ratio.x, 40.f * ratio.x);
	m_GlitterText->SetPos(710.f * ratio.x, 890.f * ratio.y);
	m_GlitterText->SetPivot(0.5f, 0.5f);
	m_GlitterText->SetFontSize(40.f * ratio.x);
	m_GlitterText->SetText(TEXT("0"));
	m_GlitterText->SetTransparency(true);
	m_GlitterText->SetAlignH(Text_Align_H::Right);
	return true;
}

void CPlayerUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	if(m_AllOpacity>0.f)
	{
		m_AllOpacity -= DeltaTime;
		SetPlayerUIOpacity(m_AllOpacity);
	}
}

void CPlayerUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CPlayerUI::Render()
{
	CUIWindow::Render();
}

CPlayerUI* CPlayerUI::Clone()
{
	return new CPlayerUI(*this);
}

void CPlayerUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CPlayerUI::Load(FILE* File)
{
	CUIWindow::Load(File);
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = FindWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_CurHP[i] = FindWidget<CUIImage>("CurHP" + std::to_string(i));
	}
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");

	m_SocksText = FindWidget<CUIText>("SocksText");
	m_FritterText = FindWidget<CUIText>("FritterText");
	m_GlitterText = FindWidget<CUIText>("GlitterText");
}

void CPlayerUI::SetPlayerUIOpacity(float opacity)
{
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i]->SetOpacity(opacity);
		m_CurHP[i]->SetOpacity(opacity);
	}
	m_Socks->SetOpacity(opacity);
	m_Fritter->SetOpacity(opacity);
	m_Glittering->SetOpacity(opacity);

	m_SocksText->SetOpacity(opacity);
	m_FritterText->SetOpacity(opacity);
	m_GlitterText->SetOpacity(opacity);
}

void CPlayerUI::SetHp(int hp)
{
	size_t size = std::size(m_CurHP);
	for (size_t i = 0; i < size; ++i)
	{
		if (hp > i)
		{
			m_CurHP[i]->SetEnable(true);
		}
		else
		{
			m_CurHP[i]->SetEnable(false);
		}
	}
}

void CPlayerUI::SetMaxHp(int hp)
{
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		if (hp > i)
		{
			m_MaxHP[i]->SetEnable(true);
		}
		else
		{
			m_MaxHP[i]->SetEnable(false);
		}
	}
}

void CPlayerUI::SetGlitter(int i)
{
	m_GlitterText->SetIntText(i);
}

void CPlayerUI::SetSocks(int i)
{
	m_SocksText->SetIntText(i,true);
}

void CPlayerUI::SetFritter(int i)
{
	m_FritterText->SetIntText(i, true);
}
