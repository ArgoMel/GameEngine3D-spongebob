#pragma once
#include "FontCollection.h"
#include "Font.h"

class CFontManager
{
private:
	friend class CResourceManager;
	IDWriteFactory5* m_WriteFactory;
	std::unordered_map<std::string, CSharedPtr<CFontCollection>>	m_mapFontCollection;
	std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>	m_mapFontColor;
	CFontManager();
	~CFontManager();
public:
	bool Init();
	bool CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName = FONT_PATH);
	bool LoadFont(const std::string& name, const TCHAR* fontName,
		int weight, float fontSize, const TCHAR* localName, int stretch=5);
	const TCHAR* GetFontFaceName(const std::string& collectionName);
	const char* GetFontFaceNameMultibyte(const std::string& collectionName);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(unsigned int color);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int color);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& color);
	CFont* FindFont(const std::string& name);
	CFontCollection* FindFontCollection(const std::string& name);
	void ReleaseFont(const std::string& name);
	void ReleaseFontCollection(const std::string& name);
};

