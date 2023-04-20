#pragma once
#include "../EngineInfo.h"
#include "RenderStateManager.h"
#include "RenderInstancing.h"
#include "../Resource/Texture/RenderTarget.h"
#include "../Resource/Shader/GraphicShader.h"

struct RenderLayer
{
	CSharedPtr<class CRenderState> alphaBlend;
	std::list<CSharedPtr<class CSceneComponent>>	renderList;
	std::string		name;
	int				layerPriority;
	RenderLayer()
		: layerPriority(0)
	{}
};

class CRenderManager
{
	DECLARE_SINGLE(CRenderManager)
private:
	CRenderStateManager* m_RenderStateManager;
	class CShadowConstantBuffer* m_ShadowCBuffer;
	class CFXAAConstantBuffer* m_FXAACBuffer;
	CSharedPtr<class CRenderState> m_AlphaBlend;
	CSharedPtr<class CRenderState> m_MRTAlphaBlend;
	CSharedPtr<class CRenderState> m_DepthDisable;
	CSharedPtr<class CRenderState> m_DepthWriteDisable;
	CSharedPtr<class CRenderState> m_LightAccBlend;
	CSharedPtr<CRenderTarget>		m_ScreenBuffer;
	CSharedPtr<CRenderTarget>		m_ShadowMapTarget;
	CSharedPtr<CRenderTarget>		m_MSBuffer;
	CSharedPtr<CRenderTarget>		m_FXAABuffer;
	CSharedPtr<CGraphicShader>		m_ScreenShader;
	//CSharedPtr<CGraphicShader>		m_DeferredRenderShader;
	CSharedPtr<CGraphicShader>		m_MSShader;
	CSharedPtr<CGraphicShader>		m_FXAAShader;
	Resolution	m_ShadowMapRS;
	EShaderType	m_ShaderType;
	bool m_IsFxaa;
	std::vector<RenderLayer*>	m_RenderLayerList;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecalBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;
	std::vector<CRenderInstancing*>	m_vecInstancingPool;
	std::list<CSharedPtr<class CSceneComponent>>	m_NormalRenderList;
	std::list<int>		m_EmptyPoolList;
	std::unordered_map<class CMesh*, CRenderInstancing*>	m_mapInstancing;
	static bool SortLayer(RenderLayer* src, RenderLayer* dest);
	static bool SortAlphaObject(class CSceneComponent* src, class CSceneComponent* dest);
	void Render2D(float deltaTime);
	void Render3D(float deltaTime);
	void RenderShadowMap(float deltaTime);
	void RenderGBuffer(float deltaTime);
	void RenderDecal(float deltaTime);
	void RenderLight(float deltaTime);
	void RenderScreen(float deltaTime);
	void RenderNoMultiSampling(float deltaTime);
	void RenderFXAA(float deltaTime); // FXAA
	void RenderDeferred(float deltaTime);
	void RenderParticle(float deltaTime);
	RenderLayer* FindLayer(const std::string& name);
public:
	class CShadowConstantBuffer* GetShadowBuffer()	const
	{
		return m_ShadowCBuffer;
	}
	const Resolution& GetShadowMapResolution()	const
	{
		return m_ShadowMapRS;
	}
	void SetFXAA(bool b)
	{
		m_IsFxaa = b;
	}
	void SetShaderType(EShaderType type);
	CRenderInstancing* FindInstancing(class CMesh* mesh);
	void CreateRenderTarget();
	void CreateLayer(const std::string& name, int priority);
	void SetLayerPriority(const std::string& name, int priority);
	void SetLayerAlphaBlend(const std::string& name);
	void DeleteLayer(const std::string& name);
	void AddRenderList(class CSceneComponent* component);
	bool Init();
	void Render(float deltaTime);
	void SetBlendFactor(const std::string& name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& name, bool blendEnable = true, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphBlend = D3D11_BLEND_ONE,	D3D11_BLEND destAlphBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphOp = D3D11_BLEND_OP_ADD, UINT8 writeMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable);
	bool CreateDepthStencil(const std::string& name, bool depthEnable = true, 
		D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL, 
		D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS, bool stencilEnable = false, 
		UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK, UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC frontFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC backFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
	template <typename T>
	T* FindRenderState(const std::string& name)
	{
		return m_RenderStateManager->FindRenderState<T>(name);
	}
};