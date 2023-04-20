#pragma once
#include "../../Ref.h"

struct KeyFrame
{
    double  time;
    Vector3 pos;
    Vector3 scale;
    Vector4 rot;
    KeyFrame()
        : time(0.0)
    {
    }
};

struct BoneKeyFrame
{
    int     boneIndex;
    std::vector<KeyFrame*>  vecKeyFrame;
    int     refCount;
    BoneKeyFrame() 
        : refCount(1)
        , boneIndex(0)
    {
    }
    ~BoneKeyFrame()
    {
        size_t  size = vecKeyFrame.size();
        for (size_t i = 0; i < size; ++i)
        {
            SAFE_DELETE(vecKeyFrame[i]);
        }
        vecKeyFrame.clear();
    }
};

struct AnimationFrameTransform
{
    Vector4 translate;
    Vector4 scale;
    Vector4 rot;
};

class CAnimationSequence : public CRef
{
private:
    friend class CAnimationManager;
    friend class CResourceManager;
    friend class CSceneResource;
    friend class CAnimation;
    class CScene* m_Scene;
    class CStructuredBuffer* m_KeyFrameBuffer;
    float						m_StartTime;
    float						m_EndTime;
    float						m_TimeLength;
    float						m_FrameTime;
    float						m_PlayTime;
    float						m_PlayScale;
    int							m_StartFrame;
    int							m_EndFrame;
    int							m_FrameLength;
    int							m_FrameMode;
    int							m_ChangeFrame;
    bool						m_Loop;
    bool						m_End;//에니메이션이 끝났는지 체크하는 변수(마지막 프레임인지 체크)
    char						m_FullPath[MAX_PATH];
    std::vector<BoneKeyFrame*>	m_vecKeyFrame;
    CAnimationSequence();
    CAnimationSequence(const CAnimationSequence& anim);
    ~CAnimationSequence();
public:
    float GetSequenceStartTime()	const
    {
        return m_StartTime;
    }
    float GetSequenceEndTime()	const
    {
        return m_EndTime;
    }
    float GetSequenceTimeLength()	const
    {
        return m_TimeLength;
    }
    float GetSequenceFrameTime()	const
    {
        return m_FrameTime;
    }
    float GetSequencePlayTime()	const
    {
        return m_PlayTime;
    }
    float GetSequencePlayScale()	const
    {
        return m_PlayScale;
    }
    int GetSequenceStartFrame()	const
    {
        return m_StartFrame;
    }
    int GetSequenceEndFrame()	const
    {
        return m_EndFrame;
    }
    int GetKeyFrameCount()	const
    {
        return m_FrameLength;
    }
    int GetSequenceFrameMode()	const
    {
        return m_FrameMode;
    }
    int GetSequenceChangeFrame()	const
    {
        return m_ChangeFrame;
    }
    bool GetSequenceLoop()	const
    {
        return m_Loop;
    }
    bool GetSequenceEnd()	const
    {
        return m_End;
    }
    void SetScene(class CScene* scene)
    {
        m_Scene = scene;
    }
    void SetEnd(bool b)
    {
        m_End = b;
    }
public:
    bool IsSequenceEnd()	const;
    BoneKeyFrame* GetKeyFrame(int index);
    void SetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame);
    bool LoadFbxAnimation(const char* fullPath, bool loop = false);
    bool Save(const TCHAR* fileName, const std::string& pathName = MESH_PATH);
    bool SaveMultibyte(const char* fileName, const std::string& pathName = MESH_PATH);
    bool SaveFullPath(const TCHAR* fullPath);
    bool SaveFullPathMultibyte(const char* fullPath);
    bool Load(const TCHAR* fileName, const std::string& pathName = MESH_PATH);
    bool LoadMultibyte(const char* fileName, const std::string& pathName = MESH_PATH);
    bool LoadFullPath(const TCHAR* fullPath);
    bool LoadFullPathMultibyte(const char* fullPath);
    void SetPlayScale(float scale);
    void SetPlayTime(float time);
    bool CreateSequence(bool loop, const struct FbxAnimationClip* clip);
    bool CreateSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime,
        const std::vector<BoneKeyFrame*>& vecFrame);
    bool CreateSequence(const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
    bool CreateSequenceFullPath(const std::string& name, const TCHAR* fullPath);
    bool CreateSequenceMultibyte(const std::string& name, const char* fileName, const std::string& pathName = MESH_PATH);
    bool CreateSequenceFullPathMultibyte(const std::string& name, const char* fullPath);
    void SetShader();
    void ResetShader();
    void SetChangeShader();
    void ResetChangeShader();
    void Clear();
    void AddNotify(const std::string& name, float time);
    CAnimationSequence* Clone();
};

