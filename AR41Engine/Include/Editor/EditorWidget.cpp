#include "EditorWidget.h"

CEditorWidget::CEditorWidget()
	: m_Size{ 100.f, 100.f }
	, m_Color{ 1.f, 1.f, 1.f, 1.f }
	, m_Owner(nullptr)
	, m_IsHide(false)
{

}

CEditorWidget::~CEditorWidget()
{
}