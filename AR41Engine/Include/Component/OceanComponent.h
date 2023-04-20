#pragma once
#include "PrimitiveComponent.h"

class COceanComponent : public CPrimitiveComponent
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
    //ocean
    float m_Time;
    float m_MaxTime;
    float m_Dir;
    std::vector<Vertex3DStatic> m_vecVtx;
    std::vector<unsigned int> m_vecIndex;
    std::vector<Vector3> m_vecFaceNormal;
    void ComputeNormal();
    void ComputeTangent();
protected:
    COceanComponent();
    COceanComponent(const COceanComponent& component);
    virtual ~COceanComponent();
public:
    float GetDetailLevel() const;
    int GetSplatCount() const;
    float GetTime() const;
    float GetWaveSpeed() const;
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
    float GetMaxTime() const
    {
        return m_MaxTime;
    }
    float GetDir() const
    {
        return m_Dir;
    }
    void SetDetailLevel(float level);
    void SetSplatCount(int count);
    void SetHeight(float height)
    {
        m_Height = height;
    }
    void SetTime(float time);
    void SetWaveSpeed(float speed);
    void SetMaxTime(float time)
    {
        m_MaxTime = time;
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
    virtual COceanComponent* Clone() const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    void CreateOcean(int countX, int countY, float sizeX, float sizeY, const TCHAR* heightMapName = nullptr,
        const std::string& heightMapPath = ROOT_PATH);
    void DeleteOcean();
};

