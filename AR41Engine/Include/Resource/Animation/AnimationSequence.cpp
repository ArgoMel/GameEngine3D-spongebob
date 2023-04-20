#include "AnimationSequence.h"
#include "../Mesh/FBXLoader.h"
#include "../Shader/StructuredBuffer.h"
#include "../../PathManager.h"

CAnimationSequence::CAnimationSequence()
	: m_Scene(nullptr)
	, m_Loop(false)
	, m_StartTime(0.f)
	, m_EndTime(0.f)
	, m_TimeLength(0.f)
	, m_FrameTime(0.f)
	, m_PlayTime(1.f)
	, m_PlayScale(1.f)
	, m_StartFrame(0)
	, m_EndFrame(0)
	, m_FrameLength(0)
	, m_FrameMode(0)
	, m_ChangeFrame(0)
	, m_End(false)
	, m_FullPath{}
	, m_KeyFrameBuffer(nullptr)
{
	SetTypeID<CAnimationSequence>();
}

CAnimationSequence::CAnimationSequence(const CAnimationSequence& anim)
	: CRef(anim)
{
	*this = anim;
	m_KeyFrameBuffer = nullptr;
	size_t  Size = m_vecKeyFrame.size();
	for (size_t i = 0; i < Size; ++i)
	{
		++m_vecKeyFrame[i]->refCount;
	}
}

CAnimationSequence::~CAnimationSequence()
{
	SAFE_DELETE(m_KeyFrameBuffer);
	size_t  Size = m_vecKeyFrame.size();
	for (size_t i = 0; i < Size; ++i)
	{
		--m_vecKeyFrame[i]->refCount;
		if (m_vecKeyFrame[i]->refCount == 0)
		{
			delete m_vecKeyFrame[i];
		}
	}
	m_vecKeyFrame.clear();
}

bool CAnimationSequence::IsSequenceEnd() const
{
	return m_End;
}

BoneKeyFrame* CAnimationSequence::GetKeyFrame(int index)
{
	if(index >=m_vecKeyFrame.size())
	{
		return nullptr;
	}
	return m_vecKeyFrame[index];
}

void CAnimationSequence::SetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame)
{
	m_vecKeyFrame = vecFrame;
}

bool CAnimationSequence::LoadFbxAnimation(const char* fullPath, bool loop)
{
	CFBXLoader  loader;
	loader.LoadFBX(fullPath, false);
	// 애니메이션 클립을 추가한다.
	const std::vector<FbxAnimationClip*>* vecClip = loader.GetClips();
	memset(m_FullPath, 0, MAX_PATH);
	strcpy_s(m_FullPath, fullPath);
	return CreateSequence(loop, vecClip->front());
}

bool CAnimationSequence::Save(const TCHAR* fileName, const std::string& pathName)
{
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		lstrcpy(fullPath, path->path);
	}
	lstrcat(fullPath, fileName);
	return SaveFullPath(fullPath);
}

bool CAnimationSequence::SaveMultibyte(const char* fileName, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return SaveFullPathMultibyte(fullPath);
}

bool CAnimationSequence::SaveFullPath(const TCHAR* fullPath)
{
	char    fullPathMultibyte[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, length, 0, 0);
	return SaveFullPathMultibyte(fullPathMultibyte);
}

bool CAnimationSequence::SaveFullPathMultibyte(const char* fullPath)
{
	FILE* file = NULL;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	CRef::Save(file);
	fwrite(&m_Loop, sizeof(bool), 1, file);
	fwrite(&m_StartTime, sizeof(float), 1, file);
	fwrite(&m_EndTime, sizeof(float), 1, file);
	fwrite(&m_TimeLength, sizeof(float), 1, file);
	fwrite(&m_FrameTime, sizeof(float), 1, file);
	fwrite(&m_PlayTime, sizeof(float), 1, file);
	fwrite(&m_PlayScale, sizeof(float), 1, file);
	fwrite(&m_StartFrame, sizeof(int), 1, file);
	fwrite(&m_EndFrame, sizeof(int), 1, file);
	fwrite(&m_FrameLength, sizeof(int), 1, file);
	fwrite(&m_FrameMode, sizeof(int), 1, file);
	fwrite(&m_ChangeFrame, sizeof(int), 1, file);
	size_t	count = m_vecKeyFrame.size();
	fwrite(&count, sizeof(size_t), 1, file);
	for (size_t i = 0; i < count; ++i)
	{
		fwrite(&m_vecKeyFrame[i]->boneIndex, sizeof(int), 1, file);
		size_t	frameCount = m_vecKeyFrame[i]->vecKeyFrame.size();
		fwrite(&frameCount, sizeof(size_t), 1, file);
		for (size_t j = 0; j < frameCount; ++j)
		{
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->time, sizeof(double), 1, file);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->pos, sizeof(Vector3), 1, file);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->scale, sizeof(Vector3), 1, file);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->rot, sizeof(Vector4), 1, file);
		}
	}
	fclose(file);
	return true;
}

bool CAnimationSequence::Load(const TCHAR* fileName, const std::string& pathName)
{
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		lstrcpy(fullPath, path->path);
	}
	lstrcat(fullPath, fileName);
	return LoadFullPath(fullPath);
}

bool CAnimationSequence::LoadMultibyte(const char* fileName, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return LoadFullPathMultibyte(fullPath);
}

bool CAnimationSequence::LoadFullPath(const TCHAR* fullPath)
{
	char    fullPathMultibyte[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, length, 0, 0);
	return LoadFullPathMultibyte(fullPathMultibyte);
}

bool CAnimationSequence::LoadFullPathMultibyte(const char* fullPath)
{
	FILE* file = NULL;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	CRef::Load(file);
	fread(&m_Loop, sizeof(bool), 1, file);
	fread(&m_StartTime, sizeof(float), 1, file);
	fread(&m_EndTime, sizeof(float), 1, file);
	fread(&m_TimeLength, sizeof(float), 1, file);
	fread(&m_FrameTime, sizeof(float), 1, file);
	fread(&m_PlayTime, sizeof(float), 1, file);
	fread(&m_PlayScale, sizeof(float), 1, file);
	fread(&m_StartFrame, sizeof(int), 1, file);
	fread(&m_EndFrame, sizeof(int), 1, file);
	fread(&m_FrameLength, sizeof(int), 1, file);
	fread(&m_FrameMode, sizeof(int), 1, file);
	fread(&m_ChangeFrame, sizeof(int), 1, file);
	size_t	count = 0;
	fread(&count, sizeof(size_t), 1, file);
	std::vector<AnimationFrameTransform>	vecFrameTrans;
	vecFrameTrans.resize(count * m_FrameLength);
	for (size_t i = 0; i < count; ++i)
	{
		BoneKeyFrame* boneKeyFrame = new BoneKeyFrame;
		m_vecKeyFrame.push_back(boneKeyFrame);
		fread(&boneKeyFrame->boneIndex, sizeof(int), 1, file);
		size_t	boneFrameCount = 0;
		fread(&boneFrameCount, sizeof(size_t), 1, file);
		for (size_t j = 0; j < boneFrameCount; ++j)
		{
			KeyFrame* keyFrame = new KeyFrame;
			boneKeyFrame->vecKeyFrame.push_back(keyFrame);
			fread(&keyFrame->time, sizeof(double), 1, file);
			fread(&keyFrame->pos, sizeof(Vector3), 1, file);
			fread(&keyFrame->scale, sizeof(Vector3), 1, file);
			fread(&keyFrame->rot, sizeof(Vector4), 1, file);
			if (j < m_FrameLength)
			{
				AnimationFrameTransform	frameTr = {};
				frameTr.scale = Vector4(keyFrame->scale.x, keyFrame->scale.y, keyFrame->scale.z, 1.f);
				frameTr.translate = Vector4(keyFrame->pos.x, keyFrame->pos.y, keyFrame->pos.z, 1.f);
				frameTr.rot = keyFrame->rot;
				vecFrameTrans[i * m_FrameLength + j] = frameTr;
			}
		}
	}
	m_KeyFrameBuffer = new CStructuredBuffer;
	m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
		(unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);
	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0], (unsigned int)vecFrameTrans.size());
	fclose(file);
	return true;
}

void CAnimationSequence::SetPlayScale(float scale)
{
	m_PlayScale = scale;
}

void CAnimationSequence::SetPlayTime(float time)
{
	m_PlayTime = time;
	m_EndTime = m_PlayTime;
	m_TimeLength = m_PlayTime;
	m_FrameTime = m_PlayTime / m_FrameLength;
	size_t  size = m_vecKeyFrame.size();
	for (size_t i = 0; i < size; ++i)
	{
		BoneKeyFrame* frame = m_vecKeyFrame[i];
		size_t  frameCount = frame->vecKeyFrame.size();
		for (size_t j = 0; j < frameCount; ++j)
		{
			KeyFrame* keyFrame = frame->vecKeyFrame[j];
			keyFrame->time = j * m_FrameTime;
		}
	}
}

bool CAnimationSequence::CreateSequence(bool loop, const FbxAnimationClip* clip)
{
	m_Loop = loop;	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	SetName(clip->Name);
	m_ChangeFrame = 0;
	switch (clip->TimeMode)
	{
	case FbxTime::eFrames24:
		m_FrameMode = 24;
		break;
	case FbxTime::eFrames30:
		m_FrameMode = 30;
		break;
	case FbxTime::eFrames60:
		m_FrameMode = 60;
		break;
	}
	// FbxAnimationClip에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_StartFrame = (int)clip->StartTime.GetFrameCount(clip->TimeMode);
	m_EndFrame = (int)clip->EndTime.GetFrameCount(clip->TimeMode);
	m_FrameLength = (int)(m_EndFrame - m_StartFrame + 1);
	// 시간 정보를 저장해준다.
	m_StartTime = 0.f;
	m_EndTime = m_PlayTime;
	m_TimeLength = m_PlayTime;
	m_FrameTime = m_PlayTime / m_FrameLength;
	std::vector<AnimationFrameTransform>	vecFrameTrans;
	vecFrameTrans.resize(clip->vecBoneKeyFrame.size() * m_FrameLength);
	// 본 3개 키프레임 5개일 경우
	// 본0키0|본0키1|본0키2|본0키3|본0키4|본1키0|본1키1|본1키2|본1키3|본1키4|본2키0|본2키1|본2키2|본2키3|본2키4

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로 뽑아온다.
	for (size_t i = 0; i < clip->vecBoneKeyFrame.size(); ++i)
	{
		BoneKeyFrame* boneKeyFrame = new BoneKeyFrame;
		boneKeyFrame->boneIndex = clip->vecBoneKeyFrame[i].BoneIndex;
		m_vecKeyFrame.push_back(boneKeyFrame);
		// 아래부터 키프레임 정보를 저장한다.
		boneKeyFrame->vecKeyFrame.reserve(clip->vecBoneKeyFrame[i].vecKeyFrame.size());
		for (size_t j = 0; j < clip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			KeyFrame* keyFrame = new KeyFrame;
			keyFrame->time = j * m_FrameTime;
			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix		mat = clip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;
			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			FbxVector4		pos = mat.GetT();
			FbxVector4	scale = mat.GetS();
			FbxQuaternion	rot = mat.GetQ();
			keyFrame->scale = Vector3((float)scale.mData[0], (float)scale.mData[1],	(float)scale.mData[2]);
			keyFrame->pos = Vector3((float)pos.mData[0], (float)pos.mData[1], (float)pos.mData[2]);
			keyFrame->rot = Vector4((float)rot.mData[0], (float)rot.mData[1], (float)rot.mData[2], (float)rot.mData[3]);
			boneKeyFrame->vecKeyFrame.push_back(keyFrame);
			if (j < m_FrameLength)
			{
				AnimationFrameTransform	frameTr = {};
				frameTr.scale = Vector4(keyFrame->scale.x, keyFrame->scale.y, keyFrame->scale.z, 1.f);
				frameTr.translate = Vector4(keyFrame->pos.x, keyFrame->pos.y, keyFrame->pos.z, 1.f);
				frameTr.rot = keyFrame->rot;
				vecFrameTrans[i * m_FrameLength + j] = frameTr;
			}
		}
	}
	m_KeyFrameBuffer = new CStructuredBuffer;
	m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
		(unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);
	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0], (unsigned int)vecFrameTrans.size());
	if (strlen(m_FullPath) != 0)
	{
		char	animName[MAX_PATH] = {};
		_splitpath_s(m_FullPath, 0, 0, 0, 0, animName, MAX_PATH, 0, 0);
		char	animPath[MAX_PATH] = {};
		strcpy_s(animPath, m_FullPath);
		int	iLength = (int)strlen(animPath);
		for (int i = iLength - 1; i >= 0; --i)
		{
			if (animPath[i] == '/' || animPath[i] == '\\')
			{
				memset(&animPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
				break;
			}
		}
		char	animFullPath[MAX_PATH] = {};
		strcpy_s(animFullPath, animPath);
		strcat_s(animFullPath, animName);
		strcat_s(animFullPath, ".sqc");
		SaveFullPathMultibyte(animFullPath);
	}
	return true;
}

bool CAnimationSequence::CreateSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime, 
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	m_Loop = loop;
	SetName(name);
	m_ChangeFrame = 0;
	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_StartFrame = 0;
	m_EndFrame = endFrame - startFrame;
	m_FrameLength = m_EndFrame - m_StartFrame + 1;
	m_PlayTime = playTime;
	m_FrameTime = playTime / m_FrameLength;
	// 시간 정보를 저장해준다.
	m_StartTime = 0.f;
	m_EndTime = playTime;
	m_TimeLength = playTime;
	std::vector<AnimationFrameTransform>	vecFrameTrans;
	vecFrameTrans.resize(vecFrame.size() * m_FrameLength);
	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		BoneKeyFrame* boneKeyFrame = new BoneKeyFrame;
		boneKeyFrame->boneIndex = vecFrame[i]->boneIndex;
		m_vecKeyFrame.push_back(boneKeyFrame);
		// 아래부터 키프레임 정보를 저장한다.
		boneKeyFrame->vecKeyFrame.reserve(m_FrameLength);
		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = 0; j < m_FrameLength; ++j)
			{
				KeyFrame* keyFrame = new KeyFrame;
				keyFrame->time = (j - startFrame) * m_FrameTime;
				keyFrame->scale = vecFrame[i]->vecKeyFrame[j]->scale;
				keyFrame->rot = vecFrame[i]->vecKeyFrame[j]->rot;
				keyFrame->pos = vecFrame[i]->vecKeyFrame[j]->pos;
				boneKeyFrame->vecKeyFrame.push_back(keyFrame);
				if (j < m_FrameLength)
				{
					AnimationFrameTransform	frameTr = {};
					frameTr.scale = Vector4(keyFrame->scale.x, keyFrame->scale.y, keyFrame->scale.z, 1.f);
					frameTr.translate = Vector4(keyFrame->pos.x, keyFrame->pos.y, keyFrame->pos.z, 1.f);
					frameTr.rot = keyFrame->rot;
					vecFrameTrans[i * m_FrameLength + j] = frameTr;
				}
			}
		}
	}
	m_KeyFrameBuffer = new CStructuredBuffer;
	m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
		(unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);
	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0], (unsigned int)vecFrameTrans.size());
	return true;
}

bool CAnimationSequence::CreateSequence(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		lstrcpy(fullPath, path->path);
	}
	lstrcat(fullPath, fileName);
	return CreateSequenceFullPath(name, fullPath);
}

bool CAnimationSequence::CreateSequenceFullPath(const std::string& name, const TCHAR* fullPath)
{
	char    fullPathMultibyte[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, length, 0, 0);
	return CreateSequenceFullPathMultibyte(name, fullPathMultibyte);
}

bool CAnimationSequence::CreateSequenceMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return CreateSequenceFullPathMultibyte(name, fullPath);
}

bool CAnimationSequence::CreateSequenceFullPathMultibyte(const std::string& name, const char* fullPath)
{
	SetName(name);
	char	ext[_MAX_EXT] = {};
	_splitpath_s(fullPath, 0, 0, 0, 0, 0, 0, ext, _MAX_EXT);
	_strupr_s(ext);
	if (strcmp(ext, ".FBX") == 0)
	{
		return LoadFbxAnimation(fullPath, m_Loop);
	}
	return LoadFullPathMultibyte(fullPath);
}

void CAnimationSequence::SetShader()
{
	m_KeyFrameBuffer->SetShader();
}

void CAnimationSequence::ResetShader()
{
	m_KeyFrameBuffer->ResetShader();
}

void CAnimationSequence::SetChangeShader()
{
	m_KeyFrameBuffer->SetShader(17, (int)EShaderBufferType::Compute);
}

void CAnimationSequence::ResetChangeShader()
{
	m_KeyFrameBuffer->ResetShader(17, (int)EShaderBufferType::Compute);
}

void CAnimationSequence::Clear()
{
	m_vecKeyFrame.clear();
}

void CAnimationSequence::AddNotify(const std::string& name, float time)
{
}

CAnimationSequence* CAnimationSequence::Clone()
{
	return new CAnimationSequence(*this);
}