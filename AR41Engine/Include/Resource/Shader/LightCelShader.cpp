/*
	셀세이더
	스텐실버퍼보단 렌더타켓쓰는게 좋음
	engine-texutre-lut 이용
*/

#include "LightCelShader.h"

CLightCelShader::CLightCelShader()
{
}

CLightCelShader::~CLightCelShader()
{
}

bool CLightCelShader::Init()
{
	if (!LoadVertexShader("LightAccVS", TEXT("Light.fx"), SHADER_PATH))
	{
		return false;
	}
	if (!LoadPixelShader("CelShaderAccPS", TEXT("Light.fx"), SHADER_PATH))
	{
		return false;
	}
	return true;
}
