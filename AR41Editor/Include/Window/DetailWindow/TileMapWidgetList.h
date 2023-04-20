#pragma once
#include "ComponentWidgetList.h"

class CTileMapWidgetList : public CComponentWidgetList
{
private:
	friend class CEditorWindow;
	class CEditorComboBox* m_ShapeCombo;
	class CEditorInputText* m_CountX;
	class CEditorInputText* m_CountY;
	class CEditorInputText* m_TileSizeX;
	class CEditorInputText* m_TileSizeY;
	class CEditorComboBox* m_TileEditCombo;
	class CEditorComboBox* m_TileOptionCombo;
	class CEditorListBox* m_TileFrameList;
	class CEditorImage* m_TileFrameImage;
	class CTileMapComponent* m_TileMap;
	void CreateTileMapClick();
	void SetShapeCallback(int index, const std::string& item);
	void SetEditModeCallback(int index, const std::string& item);
	void SetFrameListCallback(int index, const std::string& item);
protected:
	CTileMapWidgetList();
	virtual ~CTileMapWidgetList();
public:
	void SetTileMapComponent(class CTileMapComponent* tileMap);
	virtual bool Init();
	virtual void Render();
};

