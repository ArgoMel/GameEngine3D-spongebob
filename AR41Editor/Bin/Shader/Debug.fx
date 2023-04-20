#include "Share.fx"

struct VS_INPUT_DEBUG
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_DEBUG
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

Texture2DMS<float4> g_TargetTex : register(t9);

VS_OUTPUT_DEBUG DebugVS(VS_INPUT_DEBUG input)
{
    VS_OUTPUT_DEBUG output = (VS_OUTPUT_DEBUG)0;
    output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);
    output.UV = input.UV;
    return output;
}

PS_OUTPUT_SINGLE DebugPS(VS_OUTPUT_DEBUG input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
    int2 Pos = (int2)0;
    Pos.x = (int)(input.UV.x * g_ShadowMapResolution.x);
    Pos.y = (int)(input.UV.y * g_ShadowMapResolution.y);
    float4 Color = g_TargetTex.Load(Pos, 0);
    if (Color.a == 0.f)
    {
        clip(-1);
    }
    output.Color = Color;
    return output;
}
