#pragma once
#include "Editor\EditorWindow.h"

class COceanWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Grid;
	class CEditorInputText* m_MeshName;
	class CEditorInputText* m_CountX;
	class CEditorInputText* m_CountY;
	class CEditorInputText* m_HeightMapX;	//�б� ����
	class CEditorInputText* m_HeightMapY;	//�б� ����
	class CEditorInputText* m_CellSizeX;
	class CEditorInputText* m_CellSizeY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorInputText* m_Height;
	class CEditorInputText* m_Dir;
	class CEditorImage* m_Image;
	//���۵�����
	class CEditorInputText* m_Time;
	class CEditorInputText* m_MaxTime;
	class CEditorInputText* m_DetailLevel;
	class CEditorInputText* m_SplatCount;
	class CEditorInputText* m_WaveSpeed;
	CSharedPtr<class COceanComponent>	m_SelectComponent;
	void ChangeOcean();
	void ResetOcean();
protected:
	COceanWindow();
	virtual ~COceanWindow();
public:
	void SetSelectComponent(class COceanComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	void SetImage(const std::string& name, const TCHAR* path);
};

