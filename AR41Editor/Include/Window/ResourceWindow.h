#pragma once
#include "Editor\EditorWindow.h"

class CResourceWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_TextureList;
	class CEditorListBox* m_SoundList;
	class CEditorListBox* m_FontList;
	class CEditorImage* m_TextureImageBox;
	class CEditorInputText* m_FileName;
	class CEditorInputText* m_SoundName;
	class CEditorInputText* m_FontName;
	CSharedPtr<class CTexture>	m_SelectTexture;
	CSharedPtr<class CFont>	m_SelectFont;
	void LoadTextureList();
	void LoadSoundList();
	void LoadFontList();
	void TextureClickCallback(int index, const std::string& item);
	void SoundClickCallback(int index, const std::string& item);
	void FontClickCallback(int index, const std::string& item);
	void TextureSearch();
	void SoundSearch();
	void FontSearch();
protected:
	CResourceWindow();
	virtual ~CResourceWindow();
public:
	class CTexture* GetSelectTexture()	const
	{
		return m_SelectTexture;
	}
	class CFont* GetSelectFont()	const
	{
		return m_SelectFont;
	}
	virtual bool Init();
	virtual void Update(float deltaTime);
};

