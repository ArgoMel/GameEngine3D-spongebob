#include "MSShader.h"

CMSShader::CMSShader()
{
}

CMSShader::~CMSShader()
{
}

bool CMSShader::Init()
{
	if (!LoadVertexShader("NullVS", TEXT("FXAA.hlsl"), SHADER_PATH))
	{
		return false;
	}
	if (!LoadPixelShader("ConvertMSPS", TEXT("FXAA.hlsl"), SHADER_PATH))
	{
		return false;
	}
	return true;
}
