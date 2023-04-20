#include "FontCollection.h"
#include "../../PathManager.h"

CFontCollection::CFontCollection()	
	: m_Collection(nullptr)
	, m_FontFaceName{}
	, m_FontFaceNameMultibyte{}
{
}

CFontCollection::~CFontCollection()
{
	SAFE_RELEASE(m_Collection);
}

bool CFontCollection::CreateFontCollection(IDWriteFactory5* writeFactory, const std::string& name, const TCHAR* fileName,
	const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	TCHAR	fullPath[MAX_PATH] = {};
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	IDWriteFontFile* fontFile = nullptr;
	writeFactory->CreateFontFileReference(fullPath, nullptr, &fontFile);
	IDWriteFontSetBuilder1* fontBuilder = nullptr;
	if (FAILED(writeFactory->CreateFontSetBuilder(&fontBuilder)))
	{
		return false;
	}
	assert(fontFile!=0);
	fontBuilder->AddFontFile(fontFile);
	IDWriteFontSet* fontSet = nullptr;
	if (FAILED(fontBuilder->CreateFontSet(&fontSet)))
	{
		return false;
	}
	if (FAILED(writeFactory->CreateFontCollectionFromFontSet(fontSet, &m_Collection)))
	{
		return false;
	}
	SAFE_RELEASE(fontSet);
	SAFE_RELEASE(fontBuilder);
	SAFE_RELEASE(fontFile);
	IDWriteFontFamily* family = nullptr;
	if (FAILED(m_Collection->GetFontFamily(0, &family)))
	{
		return false;
	}
	IDWriteLocalizedStrings* localizedName = nullptr;
	if (FAILED(family->GetFamilyNames(&localizedName)))
	{
		return false;
	}
	if (FAILED(localizedName->GetString(0, m_FontFaceName, 64)))
	{
		return false;
	}
	int	length = WideCharToMultiByte(CP_ACP, 0, m_FontFaceName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, m_FontFaceName, -1, m_FontFaceNameMultibyte, length, 0, 0);
	return true;
}

const TCHAR* CFontCollection::GetFontFaceName() const
{
	return m_FontFaceName;
}

const char* CFontCollection::GetFontFaceNameMultibyte() const
{
	return m_FontFaceNameMultibyte;
}
