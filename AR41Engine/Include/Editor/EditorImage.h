#pragma once
#include "EditorWidget.h"

class CEditorImage : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	CSharedPtr<class CTexture>	m_Texture;
	ImVec2		m_ImageStart;
	ImVec2		m_ImageEnd;
	ImVec4		m_BorderColor;
	UINT	m_Width;
	UINT	m_Height;
	int		m_ImageIndex;
	bool	m_IsFixed;
protected:
	CEditorImage();
	virtual ~CEditorImage();
public:
	class CTexture* GetTexture()    const
	{
		return m_Texture;
	}
	const UINT GetImgWidth()	const
	{
		return m_Width;
	}
	const UINT GetImgHeight()	const
	{
		return m_Height;
	}
	void SetImageIndex(int index)
	{
		m_ImageIndex = index;
	}
	void SetImageStart(float x, float y)
	{
		m_ImageStart.x = x;
		m_ImageStart.y = y;
	}
	void SetImageEnd(float x, float y)
	{
		m_ImageEnd.x = x;
		m_ImageEnd.y = y;
	}
	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_BorderColor.x = r / 255.f;
		m_BorderColor.y = g / 255.f;
		m_BorderColor.z = b / 255.f;
		m_BorderColor.w = a / 255.f;
	}
	void SetIsFixed(bool b)
	{
		m_IsFixed = b;
	}
	void SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTexture(const std::string& name);
	void SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
	void SetTexture(class CTexture* texture);
	virtual bool Init();
	virtual void Render();
};

