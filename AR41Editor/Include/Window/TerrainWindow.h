#pragma once
#include "Editor\EditorWindow.h"

class CTerrainWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Grid;
	class CEditorInputText* m_MeshName;
	class CEditorInputText* m_CountX;
	class CEditorInputText* m_CountY;
	class CEditorInputText* m_HeightMapX;	//읽기 전용
	class CEditorInputText* m_HeightMapY;	//읽기 전용
	class CEditorInputText* m_Height;
	class CEditorInputText* m_CellSizeX;
	class CEditorInputText* m_CellSizeY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorImage* m_Image;
	//버퍼데이터
	class CEditorInputText* m_DetailLevel;
	class CEditorInputText* m_SplatCount;
	CSharedPtr<class CTerrainComponent>	m_SelectComponent;
	void ChangeTerrain();
	void ResetTerrain();
protected:
	CTerrainWindow();
	virtual ~CTerrainWindow();
public:
	void SetSelectComponent(class CTerrainComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	void SetImage(const std::string& name, const TCHAR* path);
};