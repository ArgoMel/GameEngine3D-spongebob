#pragma once
#include "EditorWidget.h"

class CEditorSameLine : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	float	m_OffsetX;	// Offset�� ���� ������ ���� �������κ��� ��������� ��ġ�� ǥ���� �������� ��Ÿ����.
	float	m_Spacing;	// ������ �������κ��� �󸶳� ���������� �����Ѵ�.
protected:
	CEditorSameLine();
	virtual ~CEditorSameLine();
public:
	//���� ��ġ
	void SetOffsetX(float x)
	{
		m_OffsetX = x;
	}
	//��� ��ġ
	void SetSpacing(float spacing)
	{
		m_Spacing = spacing;
	}
	virtual bool Init();
	virtual void Render();
};

