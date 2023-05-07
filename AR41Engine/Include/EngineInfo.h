#pragma once
#define	DIRECTINPUT_VERSION	0x0800 //include전에 써야 dxinput이 작동한다 
#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <filesystem>
#include <dwrite_3.h>
#include <d2d1.h>
#include <TCHAR.H>
#include <process.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"
#include "DShow.h"
#include <time.h>

#pragma comment(lib, "strmiids.lib")	//dsshow
#pragma comment(lib, "dwrite.lib")		//text
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "../Bin/fmodL_vc.lib")	//fmod

extern float g_DeltaTime;	//extern을 쓰면 다른 외부파일에서 사용할수 있다

#define	GRAVITY	9.8f
#define	ROOT_PATH		"Root"
#define	SHADER_PATH		"Shader"
#define	TEXTURE_PATH	"Texture"
#define	SOUND_PATH		"Sound"
#define	FONT_PATH		"Font"
#define	ANIMATION_PATH	"Animation"
#define	ANIMATION3D_PATH	"Animation3D"
#define	SCENE_PATH		"Scene"
#define	SAVE_PATH		"Save"
#define	MESH_PATH		"Mesh"
#define	DIALOG_PATH		"Dialog"
#define	CHATGPT_PATH		"ChatGPT"
#define	CUTSCENE_PATH		"Cutscene"
#define	DEFINITION_SINGLE(type)	type* type::m_Inst = nullptr;
#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr;}

#define	DECLARE_SINGLE(type)	\
private:\
	static type*	m_Inst;\
public:\
	static type* GetInst()\
	{\
		if (!m_Inst)\
			m_Inst = new type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	type();\
	~type();

struct Resolution
{
	unsigned int	width;
	unsigned int	height;
};

struct VertexColor	// 위치, 색상 정보를 가지고 있는 정점.
{
	Vector3	pos;
	Vector4	color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _pos, const Vector4& _color)
		: pos(_pos)
		, color(_color)
	{
	}
};

struct VertexBuffer
{
	ID3D11Buffer* buffer;
	int		size;	// 정점 1개의 크기
	int		count;	// 정점 개수
	void* data;		// 정점 정보

	VertexBuffer() 
		: buffer(nullptr)
		, size(0)
		, count(0)
		, data(nullptr)
	{
	}

	~VertexBuffer()
	{
		SAFE_DELETE_ARRAY(data);
		SAFE_RELEASE(buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* buffer;
	DXGI_FORMAT	fmt;	// 인덱스 포멧
	int		size;		// 인덱스 1개의 크기
	int		count;		// 인덱스 개수
	void* data;			// 인덱스 정보

	IndexBuffer() 
		: buffer(nullptr)
		, size(0)
		, count(0)
		, fmt(DXGI_FORMAT_UNKNOWN)
		, data(nullptr)
	{
	}

	~IndexBuffer()
	{
		SAFE_DELETE_ARRAY(data);
		SAFE_RELEASE(buffer);
	}
};
//struct가 matrix크기에 맞춰서 생성되서 Vector3 pivot을 생성할때 남는 메모리때문에 pivot이 이상해짐 
// 그래서 float 1개 넣어서 크기 맞춰줌
struct TransformCBuffer
{
	Matrix  matWorld;		//월드
	Matrix  matView;		//뷰
	Matrix  matProj;		//투영
	Matrix  matWVP;
	Matrix	matWV;
	Matrix	matInvWorld;
	Matrix	matInvView;
	Matrix	matInvProj;
	Matrix	matInvWVP;
	Matrix	matInvVP;
	Vector3 pivot;
	float	proj11;
	Vector3 meshSize;
	float	proj22;
};

struct GlobalCBuffer
{
	Vector2	resolution;
	Vector2	noiseResolution;
	Vector3	cameraAxisX;
	float	deltaTime;
	Vector3	cameraAxisY;
	float	accTime;
	Vector2 shadowMapResolution;
	Vector2 empty;
};

struct VertexUV
{
	Vector3	pos;
	Vector2	UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) 
		: pos(_Pos)
		, UV(_UV)
	{
	}
};

struct Vertex3D
{
	Vector3	pos;
	Vector3	normal;
	Vector2	UV;
	Vector3	tangent;
	Vector3	binormal;
	Vector4	blendWeight;
	Vector4	blendIndex;
	//Vector4	color;
};

struct Vertex3DStatic	//지형지물 만들때 사용
{
	Vector3	pos;
	Vector3	normal;
	Vector2	UV;
	Vector3	tangent;
	Vector3	binormal;
};

struct MaterialCBuffer
{
	Vector4 baseColor;
	Vector4 ambientColor;	//w는 범프에 사용
	Vector4 specularColor;
	Vector4 emissiveColor;	//w는 스펙큘러에 사용
	float opacity;
	int	textureType;
	float textureWidth;
	float textureHeight;
	int animation3DEnable;
	int	receiveDecal;
	Vector2	empty;
};

struct HierarchyName
{
	class CComponent* component;
	class CComponent* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyName()
		: component(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyObjectName
{
	class CGameObject* obj;
	class CGameObject* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyObjectName()
		: obj(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyWindowName
{
	class CUIWindow* window;
	class CUIWindow* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWindowName()
		: window(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyWidgetName
{
	class CUIWidget* widget;
	class CUIWidget* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWidgetName()
		: widget(nullptr)
		, parent(nullptr)
	{
	}
};

struct Animation2DFrameData
{
	Vector2	start;
	Vector2	end;
};

struct Animation2DCBuffer
{
	float anim2DImageWidth;
	float anim2DImageHeight;
	Vector2 anim2DFrameStart;
	Vector2 anim2DFrameEnd;
	int anim2DType;
	int anim2DEnable;
	int	anim2DFrame;
	Vector3 anim2DEmpty;
};

struct CollisionChannel
{
	std::string			name;
	ECollision_Channel	channel;
	ECollision_Interaction	interaction;
};

struct CollisionProfile
{
	std::string							name;
	CollisionChannel* channel;
	bool								enable;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;
	CollisionProfile() 
		: enable(true)
		, channel(nullptr)
	{
	}
};

struct CollisionResult
{
	class CCollider* src;
	class CCollider* dest;
	Vector3			hitPoint;
	CollisionResult() 
		: src(nullptr)
		, dest(nullptr)
	{
	}
};

struct ColliderCBuffer
{
	Vector4	color;
	Matrix	matWVP;
};

struct Box2DInfo
{
	float	left;
	float	bottom;
	float	right;
	float	top;
};

struct CubeInfo
{
	float left;
	float bottom;
	float right;
	float top;
	float front;
	float back;
};

struct Sphere2DInfo
{
	Vector2	center;
	float	radius;
};

struct Sphere3DInfo
{
	Vector3	center;
	float	radius;
};

struct OBB2DInfo
{
	Vector2	center;
	Vector2	axis[(int)AXIS2D::AXIS2D_MAX];
	float	length[(int)AXIS2D::AXIS2D_MAX];
};

struct OBB3DInfo
{
	Vector3 center;
	Vector3	axis[(int)AXIS::AXIS_MAX];
	float	length[(int)AXIS::AXIS_MAX];
};

struct PixelInfo
{
	ID3D11ShaderResourceView* SRV;
	Box2DInfo	box2D;
	EPixelCollision_Type	pixelColorCollisionType;
	EPixelCollision_Type	pixelAlphaCollisionType;
	std::string	name;
	TCHAR		fileName[MAX_PATH];
	char		pathName[MAX_PATH];
	unsigned char* pixel;
	unsigned int	width;
	unsigned int	height;
	unsigned char	typeColor[4];
	int			refCount;
	PixelInfo() 
		: fileName{}
		, pathName{}
		, refCount(0)
		, pixel(nullptr)
		, box2D{}
		, width(0)
		, height(0)
		, pixelColorCollisionType(EPixelCollision_Type::Alpha_Confirm)
		, pixelAlphaCollisionType(EPixelCollision_Type::None)
		, typeColor{}
	{
	}
};

struct UICBuffer
{
	Vector4 UITint;
	Matrix	UIWVP;
	Vector2 UIMeshSize;
	Vector2 UIPivot;
	int		UITextureEnable;
	float	UIOpacity;
	Vector2	UIEmpty;
	UICBuffer()
		: UITextureEnable(0)
		, UIOpacity(0)
	{
	}
};

struct UIProgressBarCBuffer
{
	int		barDir;
	float	percent;
	Vector2	empty;
};

struct ThreadSyncData
{
	int	header;
	int	size;
	unsigned char data[1024];
};

struct TileMapCBuffer
{
	Vector2	imageSize;
	Vector2	start;
	Vector2	end;
	Vector2	tileSize;
	Matrix	matWVP;
	int		frame;
	Vector3	empty;
};

struct TileInfo
{
	Matrix	matWVP;
	Vector2	start;
	Vector2	end;
	Vector4	typeColor;
	float	opacity;
	int		animationType;
	int		frame;
	float	empty;
	TileInfo()
		: opacity(0.f)
		, animationType(0)
		, frame(0)
		, empty(0.f)
	{
	}
};

struct LightCBuffer
{
	Vector4	Color;
	int		LightType;
	Vector3	Pos;
	Vector3	Dir;
	float	Distance;
	float	AngleIn;
	float	AngleOut;
	float	Att1;
	float	Att2;
	float	Att3;
	float	Intensity;
	Vector2	Empty;
	LightCBuffer() 
		: Intensity(1.f)
		, Att1(0.f)
		, Att2(0.f)
		, Att3(1.f)
		, AngleIn(20.f)
		, AngleOut(35.f)
		, Distance(1000.f)
		, LightType(0)
	{
	}
};

struct AnimationCBuffer
{
	int		boneCount;
	int		currentFrame;
	int		nextFrame;
	float	ratio;
	int		frameCount;
	int		rowIndex;	//인스턴싱때문에 필요 타일때처럼 본의 위치 파악하려고 쓰는거임
	int		changeAnimation;
	float	changeRatio;
	int		changeFrameCount;
	Vector3	Empty;
};

struct InstancingBufferInfo
{
	Matrix  matWVP;
	Matrix	matWV;
	Vector4 baseColor;
	Vector4 ambientColor;
	Vector4 specularColor;
	Vector4 emissiveColor;
	float opacity;
	int animation3D;
	int	receiveDecal;
	float	empty;
};

struct InstancingCBuffer
{
	int	boneCount;
	Vector3	empty;
};

struct OutputBoneInfo
{
	Matrix	matBone;
	Vector3	pos;
	float	empty1;
	Vector3	scale;
	float	empty2;
	Vector4 rot;
	OutputBoneInfo()
		: empty1(0.f)
		, empty2(0.f)
	{
	}
};

struct TerrainCBuffer
{
	float	detailLevel;
	int		splatCount;
	float	time;
	float	waveSpeed;
};

struct Ray
{
	Vector3	pos;
	Vector3	dir;
public:
	void ConvertSpace(const Matrix& mat)
	{
		pos = pos.TransformCoord(mat);
		dir = dir.TransformNormal(mat);
		dir.Normalize();
	}
};

struct ParticleCBuffer
{
	unsigned int    particleSpawnEnable;  // 파티클 생성 여부
	Vector3			particleStartMin;     // 파티클이 생성될 영역의 Min
	Vector3			particleStartMax;     // 파티클이 생성될 영역의 Max
	unsigned int    particleSpawnCountMax;// 생성될 파티클의 최대 개수
	Vector3			particleScaleMin;     // 생성될 파티클 크기의 Min
	float			particleLifeTimeMin;  // 파티클 생명의 최소 시간
	Vector3			particleScaleMax;     // 생성될 파티클 크기의 Max
	float			particleLifeTimeMax;  // 파티클 생명의 최대 시간
	Vector4			particleColorMin;     // 생성될 파티클의 색상 Min
	Vector4			particleColorMax;     // 생성될 파티클의 색상 Max
	float			particleSpeedMin;     // 파티클 이동 속도 Min
	float			particleSpeedMax;     // 파티클 이동 속도 Max
	unsigned int    particleMoveEnable;   // 이동 할지 말지.
	unsigned int    particleGravity;      // 중력 적용 할지 말지.
	Vector3			particleMoveDir;      // 이동을 할 경우 기준이 될 방향.
	unsigned int    particleMoveDirEnable;
	Vector3			particleMoveAngle;    // 기준이 될 방향으로부터 회전할 최대 각도.
	unsigned int	particleCamDir;		  // 쳐다보는 방향
	ParticleCBuffer() 
		: particleSpawnCountMax(100)
		, particleSpawnEnable(true)
		, particleScaleMin(1.f, 1.f, 1.f)
		, particleScaleMax(10.f, 10.f, 1.f)
		, particleLifeTimeMin(2.f)
		, particleLifeTimeMax(5.f)
		, particleColorMin(1.f, 1.f, 1.f, 1.f)
		, particleColorMax(1.f, 1.f, 1.f, 1.f)
		, particleMoveEnable(1)
		, particleGravity(1)
		, particleMoveDir(0.f, 1.f, 0.f)
		, particleMoveDirEnable(1)
		, particleSpeedMin(10.f)
		, particleSpeedMax(30.f)
		, particleCamDir(0)
	{
	}
};

struct ParticleInfo
{
	unsigned int     enable;
	Vector3 worldPos;
	Vector3 dir;
	float   speed;
	float   lifeTime;
	float   lifeTimeMax;
	float   fallTime;
	float   fallStartY;
};

struct ParticleInfoShare
{
	unsigned int    spawnEnable;
	Vector3  scaleMin;
	Vector3  scaleMax;
	Vector4  colorMin;
	Vector4  colorMax;
	unsigned int    gravityEnable;
	float   gravity;
	unsigned int    camDir;		// 파티클이 쳐다보는 방향
	Vector2  empty2;
};

struct PickingResult
{
	class CSceneComponent* pickComponent;
	class CGameObject* pickObject;
	Vector3	hitPoint;
	float	distance;
	PickingResult() 
		: pickComponent(nullptr)
		, pickObject(nullptr)
		, distance(0.f)
	{
	}
};

struct ShadowCBuffer
{
	Matrix  matShadowVP;
	Matrix  matShadowInvVP;
	Vector2 resolution;
	float   bias;
	float   empty;
};

struct FXAACBuffer
{
	Vector4 rcpFrame;
};

struct DialogInfo
{
	std::vector<std::wstring> vecTalker;
	std::vector<std::wstring> vecText;
	std::vector<std::wstring> vecAnim;
	int TextIdx;
	int TextMaxIdx;

	DialogInfo()
		: TextIdx(0)
		, TextMaxIdx(0)
	{}
};