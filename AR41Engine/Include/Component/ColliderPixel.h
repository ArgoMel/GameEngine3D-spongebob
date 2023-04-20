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
    //이미지 바꾸는 함수들
    void SetInfo(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoFullPath(const std::string& name, const TCHAR* fullPath);
    void SetInfoMultibyte(const std::string& name, const char* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const std::string& name, const char* fullPath);
    void SetPixelColorCollisionType(EPixelCollision_Type type); //색상에 반응할지 결정
    void SetPixelAlphaCollisionType(EPixelCollision_Type type); //알파값에 반응할지 결정
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);  //색상 설정
    void SetPixelAlpha(unsigned char a);    //알파값 설정
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

