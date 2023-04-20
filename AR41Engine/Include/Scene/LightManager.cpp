#include "LightManager.h"
#include "Scene.h"
#include "../Device.h"
#include "../GameObject/GameObject.h"
#include "../Component/LightComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Shader.h"

CLightManager::CLightManager()
	: m_Owner(nullptr)
	, m_LightInfoBufferCount(10)
{
	m_LightInfoBuffer = new CStructuredBuffer;
}

CLightManager::~CLightManager()
{
	SAFE_DELETE(m_LightInfoBuffer);
}

void CLightManager::SetGlobalLightObject(CGameObject* light)
{
	m_GlobalLightObj = light;
	if(m_GlobalLightObj)
	{
		m_GlobalLightComponent = (CLightComponent*)m_GlobalLightObj->GetRootComponent();
	}
}

void CLightManager::AddLight(CLightComponent* light)
{
	m_LightList.push_back(light);
	if (m_LightInfoBufferCount < m_LightList.size())
	{
		m_LightInfoBufferCount *= 2;
		m_LightInfoBuffer->Init("LightInfo", sizeof(LightCBuffer), 
			(unsigned int)m_LightInfoBufferCount, 41, true, (int)EShaderBufferType::Pixel);
	}
}

void CLightManager::DeleteLight(CLightComponent* light)
{
	m_LightList.remove(light);
}

void CLightManager::DeleteLight(const std::string& name)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			m_LightList.erase(iter);
			break;
		}
	}
}

void CLightManager::SetLightAccShader(const std::string& name)
{
	m_LightAccShader = CResourceManager::GetInst()->FindShader(name);
}

void CLightManager::Start()
{

}

bool CLightManager::Init()
{
	m_LightInfoBuffer->Init("LightInfo", sizeof(LightCBuffer), 
		(unsigned int)m_LightInfoBufferCount, 41, true, (int)EShaderBufferType::Pixel);
	//m_LightAccShader = CResourceManager::GetInst()->FindShader("LightAccShader");
	m_LightAccShader = CResourceManager::GetInst()->FindShader("LightCelShader");

	//CGameObject* lightObj = m_Owner->CreateObject<CGameObject>("LightSpot");
	//CLightComponent* lightCom = lightObj->CreateComponent<CLightComponent>("Light");
	//lightCom->SetLightType(ELightType::Spot);
	//lightCom->SetRelativeScale(0.1f, 0.1f, 0.1f);
	//lightCom->SetLightAngleIn(30.f);
	//lightCom->SetLightAngleOut(90.f);
	//lightCom->SetLightColor(Vector4(1.f, 0.f, 0.f, 1.f));

	//CGameObject* lightObj = m_Owner->CreateObject<CGameObject>("Light1");
	//CLightComponent* lightCom = lightObj->CreateComponent<CLightComponent>("Light");
	//lightCom->SetLightType(ELightType::Point);
	//lightCom->SetWorldPosition(-300.f, 400.f, -100.f);
	//lightCom->SetLightColor(Vector4(1.f, 0.f, 0.f, 1.f));
	//lightCom->SetLightDistance(2000.f);

	//lightObj = m_Owner->CreateObject<CGameObject>("Light2");
	//lightCom = lightObj->CreateComponent<CLightComponent>("Light");
	//lightCom->SetLightType(ELightType::Point);
	//lightCom->SetWorldPosition(300.f, 400.f, -100.f);
	//lightCom->SetLightColor(Vector4(0.f, 1.f, 0.f, 1.f));
	//lightCom->SetLightDistance(2000.f);

	//lightObj = m_Owner->CreateObject<CGameObject>("Light3");
	//lightCom = lightObj->CreateComponent<CLightComponent>("Light");
	//lightCom->SetLightType(ELightType::Point);
	//lightCom->SetWorldPosition(0.f, 100.f, -50.f);
	//lightCom->SetLightColor(Vector4(1.f, 0.f, 1.f, 1.f));
	//lightCom->SetLightDistance(2000.f);
	return true;
}

void CLightManager::Update(float deltaTime)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_LightList.erase(iter);
			iterEnd = m_LightList.end();
			continue;
		}
		++iter;
	}
}

void CLightManager::Render()
{
	if (m_LightList.empty())
	{
		return;
	}
	std::vector<LightCBuffer>	vecInstancingBuffer;
	vecInstancingBuffer.resize(m_LightList.size());
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();
	for (int i = 0; iter != iterEnd; ++iter, ++i)
	{
		const LightCBuffer& info = (*iter)->GetLightInfo();
		vecInstancingBuffer[i] = info;
	}
	m_LightInfoBuffer->UpdateBuffer(&vecInstancingBuffer[0], (int)vecInstancingBuffer.size());
	m_LightInfoBuffer->SetShader();	// 구조화버퍼 적용
	m_LightAccShader->SetShader();
	iter = m_LightList.begin();
	// Transform 정보를 넘겨준다. World는 의미없고 그 외의 정보들을 넘겨주는게 중요하다.
	(*iter)->GetTransform()->SetTransform();
	ID3D11DeviceContext* context = CDevice::GetInst()->GetContext();
	UINT	offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	context->DrawInstanced(4, (int)vecInstancingBuffer.size(), 0, 0);
}

void CLightManager::Save(FILE* file)
{
}

void CLightManager::Load(FILE* file)
{

}
