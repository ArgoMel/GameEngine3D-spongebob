#ifndef __FXAA_HLSL__
#define __FXAA_HLSL__

#include "Share.fx"

#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 39
#define FXAA_GREEN_AS_LUMA 1

#include "Fxaa3_11.h"
//위에 순서 중요
// #ifndef은 .fx에서는 안먹히는듯
#define fxaaQualitySubpix				0.75f
#define fxaaQualityEdgeThreshold		0.166f
#define fxaaQualityEdgeThresholdMin		0.0833f
#define fxaaConsoleEdgeSharpness		8.0f
#define fxaaConsoleEdgeThreshold		0.125f
#define fxaaConsoleEdgeThresholdMin		0.05f

cbuffer FXAACBuffer : register(b6)
{
	float4 rcpFrame;
};

SamplerState clamp : register(s3)
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
	MipLodBias = 0.0f;
	MaxAnisotropy = 1;
	ComparisonFunc = ALWAYS;
};

struct VS_OUTPUT_NULL
{
	//float4 Pos : SV_POSITION;
	//float2 UV  : TEXCOORD;
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
};

VS_OUTPUT_NULL NullVS(uint vertexID : SV_VertexID)
{
	// VertexID : 널 버퍼를 출력할 때 사용. 몇 번째 점인지 확인하는 용도
	//VS_OUTPUT_NULL output = (VS_OUTPUT_NULL)0;
	//output.Pos = float4(g_NullPos[vertexID], 0.f, 1.f);
	//output.UV = g_NullUV[vertexID];
	//return output;

	VS_OUTPUT_NULL	output = (VS_OUTPUT_NULL)0;
	output.ProjPos = float4(g_NullPos[vertexID], 0.f, 1.f);
	output.Pos = output.ProjPos;
	return output;
}

PS_OUTPUT_SINGLE FXAAPS(in VS_OUTPUT_NULL input) : SV_Target
{
	PS_OUTPUT_SINGLE output0 = (PS_OUTPUT_SINGLE)0;

	float4 output = (float4)1.f;
	FxaaTex InputFXAATex = { clamp, g_ScreenTex };
	float2 fxaaQualityRcpFrame = 1.f / g_Resolution;
	float4 fxaaConsolePosPos;
	float4 fxaaConsoleRcpFrameOpt;
	float4 fxaaConsoleRcpFrameOpt2;
	float4 fxaaConsole360RcpFrameOpt2;
	float4 fxaaConsole360ConstDir;

	float2  UV;
	UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
	UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

	output = FxaaPixelShader
	(
		UV,
		//input.UV,
		fxaaConsolePosPos,
		InputFXAATex,			// FxaaTex tex,
		InputFXAATex,			// FxaaTex fxaaConsole360TexExpBiasNegOne,
		InputFXAATex,			// FxaaTex fxaaConsole360TexExpBiasNegTwo,
		fxaaQualityRcpFrame,
		fxaaConsoleRcpFrameOpt,
		fxaaConsoleRcpFrameOpt2,
		fxaaConsole360RcpFrameOpt2,
		fxaaQualitySubpix,
		fxaaQualityEdgeThreshold,
		fxaaQualityEdgeThresholdMin,
		fxaaConsoleEdgeSharpness,
		fxaaConsoleEdgeThreshold,
		fxaaConsoleEdgeThresholdMin,
		fxaaConsole360ConstDir
	);
	if (output.a == 0.f)
	{
		clip(-1);
	}
	output0.Color = output;
	return output0;
}

// 멀티샘플링 텍스처를 일반 샘플링 텍스처로 바꿔주는 쉐이더 코드.
float4 ConvertMSPS(in VS_OUTPUT_NULL input) : SV_Target
{
	float2  UV;
	UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
	UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;
	int2 loadPos = (int2)0;
	loadPos.x = (int)(UV.x * g_Resolution.x);
	loadPos.y = (int)(UV.y * g_Resolution.y);
   return g_InputMSTex.Load(loadPos,0);
}
#endif	// __FXAA_HLSL__