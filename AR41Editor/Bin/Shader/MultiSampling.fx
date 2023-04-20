#include "Share.fx"

struct VS_OUTPUT_NULL
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD;
};

struct VS_OUTPUT_MSPS
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

VS_OUTPUT_NULL NullVS(uint vertexID : SV_VertexID)
{
    // VertexID : �� ���۸� ����� �� ���. �� ��° ������ Ȯ���ϴ� �뵵
    VS_OUTPUT_NULL output = (VS_OUTPUT_NULL)0;
    output.Pos = float4(g_NullPos[vertexID], 0.f, 1.f);
    output.UV = g_NullUV[vertexID];
    return output;
}

// ��Ƽ���ø� �ؽ�ó�� �Ϲ� ���ø� �ؽ�ó�� �ٲ��ִ� ���̴� �ڵ�.
float4 ConvertMSPS(in VS_OUTPUT_NULL input) : SV_Target
{
   int2 loadPos = (int2)(input.UV * g_Resolution);
   return g_InputMSTex.Load(loadPos,0);
}

PS_OUTPUT_SINGLE ConvertPSMS(in VS_OUTPUT_NULL input) : SV_Target
{
   PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
   int2 loadPos = (int2)(input.UV * g_Resolution);
   float4  textureColor = g_InputPSTex.Load(loadPos, 0);
   output.Color.rgb = textureColor.rgb * g_MtrlBaseColor.rgb;
   output.Color.a = textureColor.a * g_MtrlOpacity;
   return output;
}

/*
   int3 loadPos = int3((int2)(input.UV * g_Resolution),0);
   return g_InputMSTex.Load(loadPos);
*/