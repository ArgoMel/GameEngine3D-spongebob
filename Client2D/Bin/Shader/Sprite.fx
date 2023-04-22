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

// ���ڷ� VS_INPUT_COLOR�� ���� �� ����ü�� ������ �Է� �������Ϳ��� ����
// �����ͼ� ����ϹǷ� ���ڸ� ���� �־��� �ʿ䰡 ����.
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