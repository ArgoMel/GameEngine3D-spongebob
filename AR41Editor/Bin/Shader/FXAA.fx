//#ifndef __FXAA_HLSL__
//#define __FXAA_HLSL__
//
//#include "Share.fx"
//
//#define FXAA_PC 1
//#define FXAA_HLSL_5 1
//#define FXAA_QUALITY__PRESET 39
//#define FXAA_GREEN_AS_LUMA 1
//
//#include "Fxaa3_11.h"
//
//#define fxaaQualitySubpix				0.75f
//#define fxaaQualityEdgeThreshold		0.166f
//#define fxaaQualityEdgeThresholdMin		0.0833f
//#define fxaaConsoleEdgeSharpness		8.0f
//#define fxaaConsoleEdgeThreshold		0.125f
//#define fxaaConsoleEdgeThresholdMin		0.05f
////위에 순서 중요
//
//Texture2D<float4> g_DeferredTex : register(t10); // 현재화면
//
//cbuffer FXAACBuffer : register(b6)
//{
//	float4 rcpFrame;
//};
//
//SamplerState clamp : register(s4)
//{
//	Filter = MIN_MAG_LINEAR_MIP_POINT;
//	AddressU = Clamp;
//	AddressV = Clamp;
//	AddressW = Clamp;
//	MipLodBias = 0.0f;
//	MaxAnisotropy = 1;
//	ComparisonFunc = ALWAYS;
//};
//
//struct VS_OUTPUT_NULL
//{
//	float4 Pos : SV_POSITION;
//	float2 UV  : TEXCOORD;
//};
//
//VS_OUTPUT_NULL NullVS(uint vertexID : SV_VertexID)
//{
//	// VertexID : 널 버퍼를 출력할 때 사용. 몇 번째 점인지 확인하는 용도
//	VS_OUTPUT_NULL output = (VS_OUTPUT_NULL)0;
//	output.Pos = float4(g_NullPos[vertexID], 0.f, 1.f);
//	output.UV = g_NullUV[vertexID];
//	return output;
//}
//
//float4 FXAAPS(in VS_OUTPUT_NULL input) : SV_Target
//{
//	float4 output = (float4)1.f;
//	FxaaTex InputFXAATex = { clamp, g_DeferredTex };
//	float2 fxaaQualityRcpFrame = 1.f / g_Resolution;
//	float4 fxaaConsolePosPos;
//	float4 fxaaConsoleRcpFrameOpt;
//	float4 fxaaConsoleRcpFrameOpt2;
//	float4 fxaaConsole360RcpFrameOpt2;
//	float4 fxaaConsole360ConstDir;
//
//	output = FxaaPixelShader
//	(
//		input.UV,
//		fxaaConsolePosPos,
//		InputFXAATex,			// FxaaTex tex,
//		InputFXAATex,			// FxaaTex fxaaConsole360TexExpBiasNegOne,
//		InputFXAATex,			// FxaaTex fxaaConsole360TexExpBiasNegTwo,
//		fxaaQualityRcpFrame,
//		fxaaConsoleRcpFrameOpt,
//		fxaaConsoleRcpFrameOpt2,
//		fxaaConsole360RcpFrameOpt2,
//		fxaaQualitySubpix,
//		fxaaQualityEdgeThreshold,
//		fxaaQualityEdgeThresholdMin,
//		fxaaConsoleEdgeSharpness,
//		fxaaConsoleEdgeThreshold,
//		fxaaConsoleEdgeThresholdMin,
//		fxaaConsole360ConstDir
//	);
//	return output;
//}
//#endif	// __FXAA_HLSL__