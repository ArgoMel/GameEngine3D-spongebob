#include "Share.fx"

struct VS_INPUT_COLOR
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT_COLOR
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_COLOR SpriteColorVS(VS_INPUT_COLOR input)
{
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;
    float3  pos = input.Pos - g_Pivot * g_MeshSize;
    output.Pos = mul(float4(pos, 1.f), g_matWVP);
    output.Color = input.Color;
    return output;
}

PS_OUTPUT_SINGLE SpriteColorPS(VS_OUTPUT_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
    output.Color = input.Color;
    return output;
}

VS_OUTPUT_UV SpriteVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;
    float3 pos = input.Pos - g_Pivot * g_MeshSize;
    output.Pos = mul(float4(pos, 1.f), g_matWVP);
    output.UV = UpdateAnimation2D(input.UV);
    return output;
}

PS_OUTPUT_SINGLE SpritePS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
    float4  textureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    output.Color.rgb = textureColor.rgb * g_MtrlBaseColor.rgb;
    output.Color.a = textureColor.a * g_MtrlOpacity;
    return output;
}