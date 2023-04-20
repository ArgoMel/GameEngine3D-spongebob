#pragma once
#include "../../Ref.h"
class CAnimationSequence2D : public CRef
{
private:
    friend class CAnimationManager;
    class CScene* m_Scene;
    CSharedPtr<class CTexture>  m_Texture;
    std::vector<Animation2DFrameData>   m_vecFrameData;
    EAnimation2DType    m_Anim2DType;
    CAnimationSequence2D();
    CAnimationSequence2D(const CAnimationSequence2D& anim);
    ~CAnimationSequence2D();
public:
    class CTexture* GetTexture()    const
    {
        return m_Texture;
    }
    const Animation2DFrameData& GetFrameData(int index) const
    {
        return m_vecFrameData[index];
    }
    int GetFrameCount() const
    {
        return (int)m_vecFrameData.size();
    }
    EAnimation2DType GetAnim2DType()    const
    {
        return m_Anim2DType;
    }
    void SetScene(class CScene* scene)
    {
        m_Scene = scene;
    }
    bool Init(class CTexture* texture);
    bool Init(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& name, const TCHAR* fullPath);
    bool Init(const std::string& name, const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void AddFrame(const Vector2& start, const Vector2& end);
    void AddFrame(float startX, float startY, float endX, float endY);
    void AddFrameAll(int count, const Vector2& start, const Vector2& end);
    void AddFrameAll(int count, float startX, float startY, float endX, float endY);
    void DeleteFrame(int index);
    void ClearFrame();
    CAnimationSequence2D* Clone();
    bool Save(const char* fullPath);
    bool Load(const char* fullPath);
    bool Save(const char* fileName, const std::string& pathName);
    bool Load(const char* fileName, const std::string& pathName);
};

