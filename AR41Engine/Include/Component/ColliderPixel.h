#pragma once
#include "Collider2D.h"
class CColliderPixel : public CCollider2D
{
protected:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<class CMesh> m_PixelMesh;
    CSharedPtr<class CShader> m_PixelShader;
    PixelInfo* m_Info;
    CColliderPixel();
    CColliderPixel(const CColliderPixel& component);
    virtual ~CColliderPixel();
public:
    const PixelInfo& GetInfo()  const
    {
        return *m_Info;
    }
    //�̹��� �ٲٴ� �Լ���
    void SetInfo(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoFullPath(const std::string& name, const TCHAR* fullPath);
    void SetInfoMultibyte(const std::string& name, const char* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const std::string& name, const char* fullPath);
    void SetPixelColorCollisionType(EPixelCollision_Type type); //���� �������� ����
    void SetPixelAlphaCollisionType(EPixelCollision_Type type); //���İ��� �������� ����
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);  //���� ����
    void SetPixelAlpha(unsigned char a);    //���İ� ����
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CColliderPixel* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
    virtual bool CollisionMouse(const Vector2& mouseWorldPos);
};

