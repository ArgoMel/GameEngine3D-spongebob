#include "FontManager.h"
#include "../../Device.h"

CFontManager::CFontManager()    
    : m_WriteFactory(nullptr)
{
}

CFontManager::~CFontManager()
{
    {
        auto    iter = m_mapFontColor.begin();
        auto    iterEnd = m_mapFontColor.end();
        for (; iter != iterEnd; ++iter)
        {
            SAFE_RELEASE(iter->second);
        }
    }
    SAFE_RELEASE(m_WriteFactory);
}

bool CFontManager::Init()
{
    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory), (IUnknown**)&m_WriteFactory)))
    {
        return false;
    }
    CreateFontCollection("Default", TEXT("NotoSansKR-Regular.otf"));
    LoadFont("Default", GetFontFaceName("Default"), 600, 20.f, TEXT("ko"));
    return true;
}

bool CFontManager::CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    CFontCollection* fontCollection = FindFontCollection(name);
    if (fontCollection)
    {
        return true;
    }
    fontCollection = new CFontCollection;
    if (!fontCollection->CreateFontCollection(m_WriteFactory, name, fileName, pathName))
    {
        SAFE_DELETE(fontCollection);
        return false;
    }
    m_mapFontCollection.insert(std::make_pair(name, fontCollection));
    return true;
}

bool CFontManager::LoadFont(const std::string& name, const TCHAR* fontName, int weight,
    float fontSize, const TCHAR* localName, int stretch)
{
    CFont* font = FindFont(name);
    if (font)
    {
        return true;
    }
    font = new CFont;
    if (!font->LoadFont(m_WriteFactory, name, fontName, weight, fontSize, localName, stretch))
    {
        SAFE_DELETE(font);
        return false;
    }
    m_mapFont.insert(std::make_pair(name, font));
    return true;
}

const TCHAR* CFontManager::GetFontFaceName(const std::string& collectionName)
{
    CFontCollection* collection = FindFontCollection(collectionName);
    if (!collection)
    {
        return TEXT("");
    }
    return collection->GetFontFaceName();
}

const char* CFontManager::GetFontFaceNameMultibyte(const std::string& collectionName)
{
    CFontCollection* collection = FindFontCollection(collectionName);
    if (!collection)
    {
        return "";
    }
    return collection->GetFontFaceNameMultibyte();
}

bool CFontManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return CreateFontColor(CreateFontColorKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(float r, float g, float b, float a)
{
    return CreateFontColor(CreateFontColorKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(const Vector4& color)
{
    return CreateFontColor(CreateFontColorKey(color));
}

bool CFontManager::CreateFontColor(unsigned int color)
{
    ID2D1SolidColorBrush* brush = FindFontColor(color);
    if (brush)
    {
        return true;
    }
    float b =  (color & 0x000000ff) / 255.f;
    float g = ((color >> 8) & 0x000000ff) / 255.f;
    float r = ((color >> 16) & 0x000000ff) / 255.f;
    float a = ((color >> 24) & 0x000000ff) / 255.f;
    if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush)))
    {
        return false;
    }
    m_mapFontColor.insert(std::make_pair(color, brush));
    return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(float r, float g, float b, float a)
{
    return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const Vector4& color)
{
    return FindFontColor(CreateFontColorKey(color));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned int color)
{
    auto    iter = m_mapFontColor.find(color);
    if (iter == m_mapFontColor.end())
    {
        return nullptr;
    }
    return iter->second;
}

unsigned int CFontManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    unsigned int key = 0;
    key |= a;
    key <<= 8;
    key |= r;
    key <<= 8;
    key |= g;
    key <<= 8;
    key |= b;
    return key;
}

unsigned int CFontManager::CreateFontColorKey(float r, float g, float b, float a)
{
    unsigned char _r = (unsigned char)r * 255;
    unsigned char  _g = (unsigned char)g * 255;
    unsigned char _b = (unsigned char)b * 255;
    unsigned char _a = (unsigned char)a * 255;
    unsigned int key = 0;
    key |= _a;
    key <<= 8;
    key |= _r;
    key <<= 8;
    key |= _g;
    key <<= 8;
    key |= _b;
    return key;
}

unsigned int CFontManager::CreateFontColorKey(const Vector4& color)
{
    unsigned char _r = (unsigned char)(color.x * 255);
    unsigned char _g = (unsigned char)(color.y * 255);
    unsigned char _b = (unsigned char)(color.z * 255);
    unsigned char _a = (unsigned char)(color.w * 255);
    unsigned int key = 0;
    key |= _a;
    key <<= 8;
    key |= _r;
    key <<= 8;
    key |= _g;
    key <<= 8;
    key |= _b;
    return key;
}

CFont* CFontManager::FindFont(const std::string& name)
{
    auto    iter = m_mapFont.find(name);
    if (iter == m_mapFont.end())
    {
        return nullptr;
    }
    return iter->second;
}

CFontCollection* CFontManager::FindFontCollection(const std::string& name)
{
    auto    iter = m_mapFontCollection.find(name);
    if (iter == m_mapFontCollection.end())
    {
        return nullptr;
    }
    return iter->second;
}

void CFontManager::ReleaseFont(const std::string& name)
{
    auto	iter = m_mapFont.find(name);
    if (iter == m_mapFont.end())
    {
        return;
    }
    if (iter->second->GetRefCount() == 1)
    {
        m_mapFont.erase(iter);
    }
}

void CFontManager::ReleaseFontCollection(const std::string& name)
{
    auto	iter = m_mapFontCollection.find(name);
    if (iter == m_mapFontCollection.end())
    {
        return;
    }
    if (iter->second->GetRefCount() == 1)
    {
        m_mapFontCollection.erase(iter);
    }
}