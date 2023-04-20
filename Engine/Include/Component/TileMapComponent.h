#pragma once
#include "PrimitiveComponent.h"
#include "Tile.h"

class CTileMapComponent : public CPrimitiveComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    int GetTileRenderIndexX(const Vector3& pos);
    int GetTileRenderIndexY(const Vector3& pos);
protected:
    class CTileMapConstantBuffer* m_TileMapCBuffer;
    class CStructuredBuffer* m_TileInfoBuffer;
    class CTile* m_EditorMouseOnTile;
    CSharedPtr<class CMesh> m_TileMesh;
    CSharedPtr<class CMaterial>  m_TileMaterial;
    CSharedPtr<class CTexture>  m_TileBackTexture;
    Vector2 m_TileSize;
    Vector4 m_TileTypeColor[(int)ETileOption::End];
    Vector2 m_TileStartFrame;
    Vector2 m_TileEndFrame;
    ETileShape      m_Shape;
    int     m_CountX;
    int     m_CountY;
    int     m_Count;
    int     m_TileInfoCount;
    int     m_RenderCount;
    std::vector<CTile*> m_vecTile;
    std::vector<TileInfo>   m_vecTileInfo;
    std::vector<Animation2DFrameData>   m_vecTileFrame;
    CTileMapComponent();
    CTileMapComponent(const CTileMapComponent& component);
    virtual ~CTileMapComponent();
public:
    class CMaterial* GetTileMaterial()  const
    {
        return m_TileMaterial;
    }
    const Vector2& GetTileSize()    const
    {
        return m_TileSize;
    }
    ETileShape GetShape()   const
    {
        return m_Shape;
    }
    int GetCountX() const
    {
        return m_CountX;
    }
    int GetCountY() const
    {
        return m_CountY;
    }
    int GetCount() const
    {
        return m_Count;
    }
    int GetTileIndexX(const Vector2& pos);
    int GetTileIndexX(const Vector3& pos);
    int GetTileIndexY(const Vector2& pos);
    int GetTileIndexY(const Vector3& pos);
    int GetTileIndex(const Vector2& pos);
    int GetTileIndex(const Vector3& pos);
    CTile* GetTile(const Vector2& pos);
    CTile* GetTile(const Vector3& pos);
    CTile* GetTile(int x, int y);
    CTile* GetTile(int index);

    void SetEditorMouseOnTile(int index);
    void SetTileMaterial(const std::string& name);
    void SetTileMaterial(class CMaterial* material);
    void SetTileTexture(class CTexture* texture);
    void SetTileTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetTileTextureFullPath(const std::string& name, const TCHAR* fullPath);
    void SetTileTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    void SetTileTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    
    void SetTileBackTexture(class CTexture* texture);
    void SetTileBackTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetTileBackTextureFullPath(const std::string& name, const TCHAR* fullPath);
    void SetTileBackTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    void SetTileBackTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    
    void SetTileBaseColor(const Vector4& color);
    void CreateTile(ETileShape shape, int countX, int countY, const Vector2& tileSize);
    void ChangeTileFrame(const Vector2& pos, int frame);
    void ChangeTileOption(const Vector2& pos, ETileOption option);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CTileMapComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

