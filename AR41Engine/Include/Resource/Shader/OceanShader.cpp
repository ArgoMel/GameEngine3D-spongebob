#include "OceanShader.h"

COceanShader::COceanShader()
{
	SetTypeID<COceanShader>();
}

COceanShader::~COceanShader()
{
}

bool COceanShader::Init()
{
	if (!LoadVertexShader("OceanVS", TEXT("Ocean.fx"), SHADER_PATH))
	{
		return false;
	}
	if (!LoadPixelShader("OceanPS", TEXT("Ocean.fx"), SHADER_PATH))
	{
		return false;
	}
	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!CreateInputLayout())
	{
		return false;
	}
	return true;
}
