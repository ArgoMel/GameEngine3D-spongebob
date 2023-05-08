#include "ChatWindow.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorGroup.h"

CChatWindow::CChatWindow()
	: m_Chat(nullptr)
	, m_ChatRange(nullptr)
	, m_TextTree(nullptr)
	, m_ClickCount(0)
{
}

CChatWindow::~CChatWindow()
{
}

bool CChatWindow::Init()
{
	m_TextTree = CreateWidget<CEditorTree<class CEditorText*>>("TextTree");
	m_TextTree->SetHideName("TextTree");
	m_TextTree->SetSize(400.f, 300.f);
	m_TextTree->AddItem(nullptr, "채팅창");
	m_TextTree->SetSelectCallback<CChatWindow>(this, &CChatWindow::TreeCallback);

	m_ChatRange = CreateWidget<CEditorComboBox>("ChatRange", 80.f, 30.f);
	m_ChatRange->SetHideName("ChatRange");
	m_ChatRange->AddItem("전체");
	m_ChatRange->AddItem("친구");
	m_ChatRange->AddItem("길드");
	m_ChatRange->AddItem("개인");
	m_ChatRange->SetPrevViewName("전체");
	m_ChatRange->SetSelectIndex(0);
	m_ChatRange->SetSelectPrevViewName(true);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_Chat = CreateWidget<CEditorInputText>("Chat", 256.f, 100.f);
	m_Chat->SetHideName("Chat");
	m_Chat->SetMultiLine(true);
	m_Chat->SetInputType(EImGuiInputType::String);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("Send", 50.f, 26.f);
	button->SetClickCallback<CChatWindow>(this, &CChatWindow::SendBtnCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("초기화", 50.f, 26.f);
	button->SetClickCallback<CChatWindow>(this, &CChatWindow::Clear);
	return true;
}

void CChatWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CChatWindow::Clear()
{
	m_TextTree->Clear();
	m_TextTree->AddItem(nullptr, "채팅창");
	m_ClickCount = 0;
}

void CChatWindow::ClearSelect()
{
	m_SelectTextItem = "";
}

void CChatWindow::Delete()
{
	if(m_SelectTextItem=="")
	{
		return;
	}
	m_TextTree->DeleteItem(m_SelectTextItem);
}

bool CChatWindow::AddItem(class CEditorText* text, const std::string& name, const std::string& parentName)
{
	return m_TextTree->AddItem(text, name, parentName);
}

void CChatWindow::SendBtnCallback()
{
	struct tm t = {};
	time_t timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	localtime_s(&t,&timer); // 포맷팅을 위해 구조체에 넣기
	CEditorText* txt = m_TextTree->CreateWidget<CEditorText>("채팅창", "txt"+ std::to_string(m_ClickCount++));
	std::string timeTxt = "[" + std::to_string(t.tm_hour) +" : "+ std::to_string(t.tm_min) + " : " + std::to_string(t.tm_sec) + "] ";
	timeTxt += "[" + m_ChatRange->GetSelectItem() + "] 플레이어 : ";

	char inputText[256];
	WideCharToMultiByte(CP_ACP, 0, m_Chat->GetTText(), _MAX_EXT, inputText, _MAX_EXT, NULL, NULL);
	std::string s = inputText;
	timeTxt += s;
	txt->SetText(timeTxt.c_str());
	txt->SetMultiLine(true);
	m_Chat->SetText("");
}

void CChatWindow::TreeCallback(CEditorTreeItem<class CEditorText*>* node, const std::string& item)
{
	//m_SelectText = node->GetCustomData();
	m_SelectTextItem = item;
}