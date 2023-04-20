#pragma once
#include "PrimitiveComponent.h"

class CTerrainComponent : public CPrimitiveComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    class CTerrainConstantBuffer* m_CBuffer;
    Vector2 m_CellSize;
    Vector2 m_Size;
    std::string m_FileName;
    int m_CountX;
    int m_CountY;
    int m_HeightMapX;
    int m_HeightMapY;
    float m_Height;
    bool m_Grid;
    std::vector<Vertex3DStatic> m_vecVtx;
    std::vector<unsigned int> m_vecIndex;
    std::vector<Vector3> m_vecFaceNormal;
    void ComputeNormal();
    void ComputeTangent();
protected:
    CTerrainComponent();
    CTerrainComponent(const CTerrainComponent& component);
    virtual ~CTerrainComponent();
public:
    float GetDetailLevel() const;
    int GetSplatCount() const;
    Vector2 GetCellSize()   const
    {
        return m_CellSize;
    }
    Vector2 GetSize()   const
    {
        return m_Size;
    }
    int GetCountX() const
    {
        return m_CountX;
    }
    int GetCountY() const
    {
        return m_CountY;
    }
    int GetHeightMapX() const
    {
        return m_HeightMapX;
    }
    int GetHeightMapY() const
    {
        return m_HeightMapY;
    }
    float GetHeight() const
    {
        return m_Height;
    }
    bool GetGrid() const
    {
        return m_Grid;
    }
    int GetFaceCount() const
    {
        return (int)m_vecIndex.size() / 3;
    }
    void GetFaceIndex(unsigned int index[3], int faceIndex)
    {
        index[0] = m_vecIndex[faceIndex * 3];
        index[1] = m_vecIndex[faceIndex * 3 + 1];
        index[2] = m_vecIndex[faceIndex * 3 + 2];
    }
    const Vector3& GetVertexPos(int index) const
    {
        return m_vecVtx[index].pos;
    }
    void SetDetailLevel(float Level);
    void SetSplatCount(int Count);
    void SetCountX(int count)
    {
        m_CountX = count;
    }
    void SetCountY(int count)
    {
        m_CountY = count;
    }
    void SetHeight(float f)
    {
        m_Height = f;
    }
    virtual bool SetMesh(const std::string& name);
    virtual bool SetMesh(class CMesh* mesh);
    virtual bool SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& name, const TCHAR* fullPath);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void RenderShadowMap();
    virtual void Render();
    virtual CTerrainComponent* Clone() const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    void CreateTerrain(int countX, int countY, float sizeX, float sizeY, const TCHAR* heightMapName = nullptr, 
        const std::string& heightMapPath = ROOT_PATH);
    void DeleteTerrain();
};

