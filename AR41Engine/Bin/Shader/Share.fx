/*
    새 항목-유틸리티-텍스트파일-
	ar41engine -bin-shader 에 생성
	생성후 속성-빌드에서 제외
*/

static const float2 g_NullPos[4] =
{
    float2(-1.f, 1.f),
    float2(1.f, 1.f),
    float2(-1.f, -1.f),
    float2(1.f, -1.f)
};

static const float2 g_NullUV[4] =
{
    float2(0.f, 0.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 1.f)
};
// DirectX11은 RenderTarget을 최대 8개까지 한꺼번에 사용할 수 있다.
// SV_TARGET0 은 0번 렌더타겟에 출력한다.
struct PS_OUTPUT_SINGLE
{
    float4 Color : SV_TARGET;
};

struct PS_OUTPUT_GBUFFER
{
    float4 GBuffer1 : SV_TARGET;
    float4 GBuffer2 : SV_TARGET1;
    float4 GBuffer3 : SV_TARGET2;
    float4 GBuffer4 : SV_TARGET3;
    float4 GBuffer5 : SV_TARGET4;
    float4 GBuffer6 : SV_TARGET5;
};

struct InstancingBuffer
{
    matrix  matWVP;
    matrix	matWV;
    float4 BaseColor;
    float4 AmbientColor;
    float4 SpecularColor;
    float4 EmissiveColor;
    float Opacity;
    int Animation3D;
    int ReceiveDecal;
    float	Empty;
};
// terrain, ocean
struct VS_INPUT_STATIC
{
    float3	Pos : POSITION;
    float3	Normal : NORMAL;
    float2	UV : TEXCOORD;
    float3	Tangent : TANGENT;
    float3	Binormal : BINORMAL;
};

struct VS_OUTPUT_STATIC
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
    float3 ViewPos : POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};
//
//particle
struct ParticleInfoShare
{
    uint    SpawnEnable;
    float3  ScaleMin;
    float3  ScaleMax;
    float4  ColorMin;
    float4  ColorMax;
    uint    GravityEnable;
    float   Gravity;
    uint    CamDir;
    float2  Empty2;
};
//
SamplerState g_PointSmp : register(s0);
SamplerState g_LinearSmp : register(s1);
SamplerState g_AnisotropicSmp : register(s2);
SamplerState g_ClampSmp : register(s3); // FXAA 전용
SamplerState g_PointClampSmp : register(s4); // 카툰렌더링용

Texture2D g_BaseTexture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_SpecularTexture : register(t2);
Texture2D g_EmissiveTexture : register(t3);
Texture2D g_RoughnessTexture : register(t4);
Texture2D g_AOTexture : register(t5);
Texture2D g_GlobalNoiseTexture : register(t6);
Texture2D g_LUTTexture : register(t7);
Texture2DMS<float4> g_InputMSTex : register(t8);
//debug.fx에서 9번 사용중
Texture2DArray g_SplatDifTextureArray: register(t10);
Texture2DArray g_SplatNormalTextureArray: register(t11);
Texture2DArray g_SplatSpcTextureArray: register(t12);
Texture2DArray g_SplatAlphaTextureArray: register(t13);
//14~22 라이트 사용중, 파티클 14번
Texture2DMS<float4> g_ScreenTex : register(t21);

StructuredBuffer<InstancingBuffer> g_InstancingInfoArray : register(t50);
StructuredBuffer<int>   g_RandomArray : register(t51);

// 상수버퍼는 16바이트 단위로 맞춰야 한다.
cbuffer Transform : register(b0)
{
    matrix  g_matWorld; //월드
    matrix  g_matView;  //뷰
    matrix  g_matProj;  //투영
    matrix  g_matWVP;
    matrix  g_matWV;
    matrix	g_matInvWorld;
    matrix	g_matInvView;
    matrix	g_matInvProj;
    matrix	g_matInvWVP;
    matrix	g_matInvVP;
    float3  g_Pivot;
    float g_Proj11;
    float3  g_MeshSize;
    float  g_Proj22;
};

cbuffer Material : register(b1)
{
    float4 g_MtrlBaseColor;
    float4 g_MtrlAmbientColor;
    float4 g_MtrlSpecularColor;
    float4 g_MtrlEmissiveColor;
    float g_MtrlOpacity;
    int g_MtrlTextureType;
    float g_MtrlTextureWidth;
    float g_MtrlTextureHeight;
    int   g_MtrlAnimation3D;
    int   g_MtrlReceiveDecal;
    float2  g_MtrlEmpty;
};

cbuffer Animation2D : register(b2)
{
    float g_Anim2DImageWidth;
    float g_Anim2DImageHeight;
    float2 g_Anim2DFrameStart;
    float2 g_Anim2DFrameEnd;
    int g_Anim2DType;
    int g_Anim2DEnable;
    int g_Anim2DFrame;
    float3 g_Anim2DEmpty;
};

cbuffer GlobalCBuffer : register(b3)
{
    float2  g_Resolution;
    float2  g_GlobalNoiseResolution;
    float3  g_CameraAxisX;
    float   g_GlobalDeltaTime;
    float3  g_CameraAxisY;
    float   g_GlobalAccTime;
    float2  g_ShadowMapResolution;
    float2  g_GlobalEmpty;
};

cbuffer LightCBuffer : register(b4)
{
    float4	g_LightColor;
    int		g_LightLightType;
    float3	g_LightPos;
    float3	g_LightDir;
    float	g_LightDistance;
    float	g_LightAngleIn;
    float	g_LightAngleOut;
    float	g_LightAtt1;
    float	g_LightAtt2;
    float	g_LightAtt3;
    float   g_LightIntensity;
    float2	g_LightEmpty;
};

cbuffer InstancingCBuffer : register(b5)
{
    int g_InstancingBoneCount;
    float3	g_InstancingEmpty;
};

cbuffer TerrainCBuffer : register(b10)
{
    float       g_TerrainDetailLevel;   //ocean에서는 물결속도를 의미
    int         g_TerrainSplatCount;
    float       g_Time;
    float       g_WaveSpeed;              //파도 높낮이
};

#define Anim2D_None -1
#define Anim2D_Atlas 0
#define Anim2D_Frame 1
#define Anim2D_Array 2
#define Image_Atlas 0
#define Image_Frame 1
#define Image_Array 2

float2 UpdateAnimation2D(float2 UV)
{
    if (g_Anim2DEnable == 0)
    {
        return UV;
    }
    float2 result = (float2) 0;
    if (g_Anim2DType == Anim2D_Atlas)
    {
        if (UV.x == 0.f)
        {
            result.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
        }
        else
        {
            result.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
        }
        if (UV.y == 0.f)
        {
            result.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
        }
        else
        {
            result.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;
        }
    }
    else
    {
        result = UV;
    }
    return result;
}
//외부텍스쳐 사용시 설정 바꿔야 한다
float3 ComputeBumpNormal(float3 Tangent, float3 Binormal, float3 Normal, float2 UV)
{
    float3  Result = Normal;
    if (g_MtrlAmbientColor.w == 1.f)
    {
        float4 NormalColor = g_NormalTexture.Sample(g_LinearSmp, UV);
        // 색상은 0 ~ 1 사이로 나오므로 -1 ~ 1 사이로 변환한다.
        float3  ConvertNormal = NormalColor.rgb * 2.f - 1.f;
        ConvertNormal = normalize(ConvertNormal);
        // Tangent(X축), Binormal(Y축), Normal(Z축)을 이용해서 법선벡터를 뷰공간으로 변환해준다.
        float3x3 matConvert =
        {
            Tangent,
            Binormal,
            Normal
        };
        Result = normalize(mul(ConvertNormal, matConvert));
    }
    return Result;
}

float3 ComputeBumpNormalInstancing(float3 Tangent, float3 Binormal, float3 Normal, float2 UV, uint InstanceID)
{
    float3  Result = Normal;
    if (g_InstancingInfoArray[InstanceID].AmbientColor.w == 1.f)
    {
        float4 NormalColor = g_NormalTexture.Sample(g_LinearSmp, UV);
        float3  ConvertNormal = NormalColor.rgb * 2.f - 1.f;
        ConvertNormal = normalize(ConvertNormal);
        float3x3 matConvert =
        {
            Tangent,
            Binormal,
            Normal
        };
        Result = normalize(mul(ConvertNormal, matConvert));
    }
    return Result;
}

float ConvertColor(float4 Color)
{
    uint4 Convert = (float4)0;
    Convert.r = uint(Color.r * 255);
    Convert.g = uint(Color.g * 255);
    Convert.b = uint(Color.b * 255);
    Convert.a = uint(Color.a * 255);
    uint Result = 0;
    Result = Convert.a;
    Result = (Result << 8) | Convert.r;
    Result = (Result << 8) | Convert.g;
    Result = (Result << 8) | Convert.b;
    return asfloat(Result);
}

float4 ConvertColor(float Color)
{
    uint Convert = asuint(Color);
    float4 Result;
    Result.b = (Convert & 0x000000ff) / 255.f;
    Result.g = ((Convert >> 8) & 0x000000ff) / 255.f;
    Result.r = ((Convert >> 16) & 0x000000ff) / 255.f;
    Result.a = ((Convert >> 24) & 0x000000ff) / 255.f;
    return Result;
}

static float g_Gaussian5x5[25] =
{
    0.003765f, 0.015019f, 0.023792f, 0.015019f, 0.003765f,
    0.015019f, 0.059912f, 0.094907f, 0.059912f, 0.015019f,
    0.023792f, 0.094907f, 0.150342f, 0.094907f, 0.023792f,
    0.015019f, 0.059912f, 0.094907f, 0.059912f, 0.015019f,
    0.003765f, 0.015019f, 0.023792f, 0.015019f, 0.003765f
};

float GaussianSample(int2 NoiseUV)
{
    float   Output = 0.f;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2    UV = NoiseUV + int2(j - 2, i - 2);
            if (UV.x < 0)
            {
                UV.x = g_GlobalNoiseResolution.x - UV.x;
            }
            else if (UV.x > g_GlobalNoiseResolution.x)
            {
                UV.x = UV.x - g_GlobalNoiseResolution.x;
            }
            if (UV.y < 0)
            {
                UV.y = g_GlobalNoiseResolution.y - UV.y;
            }
            else if (UV.y > g_GlobalNoiseResolution.y)
            {
                UV.y = UV.y - g_GlobalNoiseResolution.y;
            }
            Output += g_GlobalNoiseTexture[UV].r;
        }
    }
    Output /= 25.f;
    return Output;
}

float Rand(float Key)
{
    // NoiseTexture에서 사용할 UV를 만든다.
    float2  UV = float2(cos(Key + g_GlobalAccTime), sin(g_GlobalAccTime));
    // 0 ~ 1 사이로 만들어준다.
    UV = UV * 0.5f + 0.5f;
    int Index = 100;
    return g_RandomArray[Index] % 100001 / 100000.f;
}

float RandIndex(inout int Index)
{
    float Number = g_RandomArray[Index] % 100001 / 100000.f;

    Index = (Index + 1) % (1024 * 1024);

    return Number;
}

float DegreeToRadian(float Angle)
{
    return Angle / 180.f * 3.14159f;
}
