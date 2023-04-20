#pragma once
#include "EditorWidget.h"

class CEditorSameLine : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	float	m_OffsetX;	// Offset은 현재 라인의 가장 왼쪽으로부터 어느정도의 위치에 표현할 것인지를 나타낸다.
	float	m_Spacing;	// 왼쪽의 위젯으로부터 얼마나 떨어질지를 결정한다.
protected:
	CEditorSameLine();
	virtual ~CEditorSameLine();
public:
	//절대 위치
	void SetOffsetX(float x)
	{
		m_OffsetX = x;
	}
	//상대 위치
	void SetSpacing(float spacing)
	{
		m_Spacing = spacing;
	}
	virtual bool Init();
	virtual void Render();
};

