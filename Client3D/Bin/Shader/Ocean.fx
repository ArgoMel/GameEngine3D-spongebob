#include "Share.fx"

float3 ComputeBumpNormalOcean(float3 Tangent, float3 Binormal, float3 Normal, float2 UV, float2 OriginUV)
{
    float3  Result = Normal;
    if (g_MtrlAmbientColor.w == 1.f)
    {
        float4 NormalColor = g_NormalTexture.Sample(g_LinearSmp, UV);
        // Alpha Texture의 색상을 얻어온다.
        for (int i = 0; i < g_TerrainSplatCount; ++i)
        {
            float4 Alpha = g_SplatAlphaTextureArray.Sample(g_LinearSmp, float3(OriginUV, i));
            float4 SplatNrm = g_SplatNormalTextureArray.Sample(g_LinearSmp, float3(UV, i));
            NormalColor.rgb = NormalColor.rgb * (1.f - Alpha.r) + SplatNrm.rgb * Alpha.r;
        }
        // 색상은 0 ~ 1 사이로 나오므로 -1 ~ 1 사이로 변환한다.
        float3  ConvertNormal = NormalColor.rgb * 2.f - 1.f;
        // z는 무조건 + 방향으로 만들어준다.;
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

VS_OUTPUT_STATIC OceanVS(VS_INPUT_STATIC input)
{
    VS_OUTPUT_STATIC output = (VS_OUTPUT_STATIC)0;

    float2  texcoord = (input.UV * 8) + g_Time * 0.009 * g_WaveSpeed;
    float2  texcoord2 = (float2(-input.UV.x, input.UV.y) * 8) + g_Time * 0.01* g_WaveSpeed;
    float2 height= lerp(texcoord, texcoord2,0.5);
    float height2= lerp(height.x, height.y,0.5);
    //input.Pos.x += height.x;
    input.Pos.y += height2;

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
    output.ViewPos = mul(float4(input.Pos, 1.f), g_matWV).xyz;
    output.Normal = normalize(mul(float4(input.Normal, 0.f), g_matWV).xyz);
    output.Tangent = normalize(mul(float4(input.Tangent, 0.f), g_matWV).xyz);
    output.Binormal = normalize(mul(float4(input.Binormal, 0.f), g_matWV).xyz);
    output.Pos = output.ProjPos;
    output.UV = input.UV;
    return output;
}

PS_OUTPUT_GBUFFER OceanPS(VS_OUTPUT_STATIC input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;
    float2  DetailUV = (input.UV * 8) + g_GlobalAccTime * 0.009* g_TerrainDetailLevel;
    float2  DetailUV2 = (float2(-input.UV.x, input.UV.y) * 8) + g_GlobalAccTime * 0.01* g_TerrainDetailLevel;
    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, DetailUV);
    if (TextureColor.a == 0.f || g_MtrlOpacity == 0.f)
    {
        clip(-1);
    }
    float4  SpecularColor = g_MtrlSpecularColor;
    if (g_MtrlBaseColor.a == 1.f)
    {
        SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, DetailUV).rrr;
    }
    // Alpha Texture의 색상을 얻어온다.
    for (int i = 0; i < g_TerrainSplatCount; ++i)
    {
        float4 Alpha = g_SplatAlphaTextureArray.Sample(g_LinearSmp, float3(input.UV, i));
        float4 SplatDif = g_SplatDifTextureArray.Sample(g_LinearSmp, float3(DetailUV, i));
        TextureColor.rgb = TextureColor.rgb * (1.f - Alpha.r) + SplatDif.rgb * Alpha.r;
        float4 SplatSpc = g_SplatSpcTextureArray.Sample(g_LinearSmp, float3(DetailUV, i));
        SpecularColor.rgb = SpecularColor.rgb * (1.f - Alpha.r) + SplatSpc.rgb * Alpha.r;
    }
    output.GBuffer1.rgb = TextureColor.rgb;
    output.GBuffer1.a = TextureColor.a * g_MtrlOpacity;

    float3 normal1 = ComputeBumpNormalOcean(input.Tangent, input.Binormal, input.Normal, DetailUV, input.UV);
    float3 normal2 = ComputeBumpNormalOcean(input.Tangent, input.Binormal, input.Normal, DetailUV2, input.UV);
    output.GBuffer2.rgb = normalize(lerp(normal1, normal2,0.5));
    //output.GBuffer2.rgb = normalize(mul(output.GBuffer2.rgb,input.ProjPos.xyz));

    output.GBuffer2.a = 1.f;
    // 원근투영을 이용하여 변환된 투영공간의 위치에서 w는 뷰공간에서의 z값이 그대로 들어간다.
    output.GBuffer3.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer3.g = input.ProjPos.w;
    output.GBuffer3.b = g_MtrlSpecularColor.w;
    output.GBuffer3.a = g_MtrlReceiveDecal;
    output.GBuffer5.a = 1.f;
    output.GBuffer5.rgb = input.Tangent;
    output.GBuffer6.a = 1.f;
    output.GBuffer6.rgb = input.Normal;
    output.GBuffer4.r = ConvertColor(g_MtrlBaseColor);
    output.GBuffer4.g = ConvertColor(g_MtrlAmbientColor);
    output.GBuffer4.b = ConvertColor(SpecularColor);
    float4  EmissiveColor = g_MtrlEmissiveColor;
    if (g_MtrlEmissiveColor.a == 1.f)
    {
        EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, DetailUV).rgb;
    }
    output.GBuffer4.a = ConvertColor(EmissiveColor);
    return output;
}