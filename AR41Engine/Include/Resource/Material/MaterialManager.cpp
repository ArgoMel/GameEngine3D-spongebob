#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	CMaterial* material = CreateMaterial<CMaterial>("DefaultColor");
	material->SetShader("SpriteColorShader");

	material = CreateMaterial<CMaterial>("DefaultTexture");
	material->SetShader("SpriteShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture", TEXT("neutral.png"));

	material = CreateMaterial<CMaterial>("TileMap");
	std::vector<const TCHAR*>	vecFileNames;
	for (int i = 0; i <= 379; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor_%d.png"), i);
		vecFileNames.push_back(fileName);
	}
	material->SetShader("TileMapShader");
	material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultTileIsometric", vecFileNames);
	material->SetRenderState("DepthLessEqual");
	material->SetRenderState("AlphaBlend");
	for (int i = 0; i <= 379; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileNames[i]);
	}

	material = CreateMaterial<CMaterial>("DefaultTileMapBack");
	material->SetShader("TileMapBackShader");
	material->SetRenderState("DepthLessEqual");

	material = CreateMaterial<CMaterial>("Sky");
	material->SetShader("SkyShader");
	material->AddTexture(10, (int)EShaderBufferType::Pixel, "SkyTexture", TEXT("Sky/sky0331.dds"));
	material->SetRenderState("DepthLessEqual");
	material->SetRenderState("FrontFaceCull");

	material = CreateMaterial<CMaterial>("DefaultTerrain");
	material->SetShader("TerrainShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTerrainTexture", TEXT("LandScape/ROCK_01+MOSS.bmp"));
	material->AddTexture(1, (int)EShaderBufferType::Pixel, "DefaultTerrainNormalTexture", 
		TEXT("LandScape/ROCK_01+MOSS_NRM.png"));
	material->AddTexture(2, (int)EShaderBufferType::Pixel, "DefaultTerrainSpecularTexture", 
		TEXT("LandScape/ROCK_01+MOSS_SPEC.png"));
	std::vector<const TCHAR*>	vecFileName;
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));
	material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultSplatDif", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));
	material->AddTextureArray(11, (int)EShaderBufferType::Pixel, "DefaultSplatNormal", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));
	material->AddTextureArray(12, (int)EShaderBufferType::Pixel, "DefaultSplatSpc", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/RoadAlpha1.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterStonAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterBaseAlpha.bmp"));
	material->AddTextureArray(13, (int)EShaderBufferType::Pixel, "DefaultSplatAlpha", vecFileName);
	material->EnableBump();
	material->EnableSpecular();
	material->SetSpecularPower(3.2f);

	material = CreateMaterial<CMaterial>("BikiniCityTerrain");
	material->SetShader("TerrainShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "BikiniCity", TEXT("LandScape/T_BB_Sand_01_D.tga"));

	material = CreateMaterial<CMaterial>("Sea");
	material->SetShader("OceanShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Sea", TEXT("Particle/sea.png"));
	material->SetRenderState("AlphaBlend");
	material->SetOpacity(0.9f);
	material->AddTexture(1, (int)EShaderBufferType::Pixel, "DefaultTerrainNormalTexture",
		TEXT("LandScape/ROCK_01+MOSS_NRM.png"));
	material->AddTexture(2, (int)EShaderBufferType::Pixel, "DefaultTerrainSpecularTexture",
		TEXT("LandScape/ROCK_01+MOSS_SPEC.png"));
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));
	material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultSplatDif", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));
	material->AddTextureArray(11, (int)EShaderBufferType::Pixel, "DefaultSplatNormal", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));
	material->AddTextureArray(12, (int)EShaderBufferType::Pixel, "DefaultSplatSpc", vecFileName);
	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/RoadAlpha1.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterStonAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterBaseAlpha.bmp"));
	material->AddTextureArray(13, (int)EShaderBufferType::Pixel, "DefaultSplatAlpha", vecFileName);
	material->EnableBump();
	material->EnableSpecular();
	material->SetSpecularPower(3.2f);

	material = CreateMaterial<CMaterial>("Billboard");
	material->SetShader("BillboardShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultBillboardTexture", TEXT("neutral.png"));

	material = CreateMaterial<CMaterial>("DebugDecal");
	material->SetShader("DecalDebugShader");
	material->SetBaseColor(1.f, 1.f, 0.f, 1.f);

	material = CreateMaterial<CMaterial>("DefaultDecal");
	material->SetShader("DecalShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefauleDecal", TEXT("Decal/free-blood-texture_COLOR.png"));
	material->AddTexture(1, (int)EShaderBufferType::Pixel, "DefauleDecalNormal", TEXT("Decal/free-blood-texture_NRM.png"));
	material->AddTexture(2, (int)EShaderBufferType::Pixel, "DefauleDecalSpecular", 
		TEXT("Decal/free-blood-texture_SPEC.png"));
	material->EnableBump();
	material->EnableSpecular();

	material = CreateMaterial<CMaterial>("Bubble");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Bubble", TEXT("Particle/Bubbles50px.png"));
	material->SetShader("SpriteShader");

	material = CreateMaterial<CMaterial>("ShockWave");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "ShockWave", TEXT("Particle/T_ShockWave.png"));
	material->SetShader("SpriteShader");

	material = CreateMaterial<CMaterial>("Glow");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Glow", TEXT("Particle/T_Blue_Glow_00.tga"));
	material->SetShader("SpriteShader");

	material = CreateMaterial<CMaterial>("Fire2");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Fire2", TEXT("Particle/flame2.png"));
	material->SetShader("SpriteShader");
	material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("Flare");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Flare", TEXT("Particle/light.png"));
	material->SetShader("SpriteShader");
	material->SetOpacity(0.8f);

	material = CreateMaterial<CMaterial>("Shower");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Shower", TEXT("Particle/T_JF_Shower_Drops_01.tga"));
	material->SetShader("SpriteShader");
	material->SetOpacity(0.8f);

	material = CreateMaterial<CMaterial>("WaterRing");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "WaterRing", TEXT("Particle/T_Waterdrop_01.tga"));
	material->SetShader("SpriteShader");
	material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("Lightning");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Lightning", TEXT("T_Lightning_Balt_01.tga"));
	material->SetShader("SpriteShader");
	material->SetBaseColor(1.f, 1.f, 1.f, 1.f);
	material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("blueflame");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "blueflame", TEXT("Particle/blueflame.png"));
	material->SetShader("SpriteShader");

	material = CreateMaterial<CMaterial>("Effect");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Water", TEXT("Particle/T_Seafoam_BC.tga"));
	material->SetShader("TranslationShader");
	material->SetRenderState("AlphaBlend");
	material->SetOpacity(0.5f);

	material = CreateMaterial<CMaterial>("WaterDrop");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "WaterDrop", TEXT("Particle/T_Seafoam_BC.tga"));
	material->SetShader("DropShader");
	material->SetRenderState("AlphaBlend");
	material->SetOpacity(0.5f);

	material = CreateMaterial<CMaterial>("WaterFall");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "WaterFall", TEXT("Particle/waterfall.png"));
	material->SetShader("DropShader");
	material->SetRenderState("AlphaBlend");
	//material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("WaterFall2");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "WaterFall2", TEXT("Particle/Waterfall2.png"));
	material->SetShader("DropShader");
	material->SetRenderState("AlphaBlend");
	material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("RBSpongeBooster");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "RBSpongeBooster", TEXT("Particle/blueflame.png"));
	material->SetShader("DropShader");
	//material->SetRenderState("AlphaBlend");
	material->SetOpacity(0.7f);

	material = CreateMaterial<CMaterial>("Hm");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "Hm", TEXT("Hm.jpg"));
	material->SetShader("SpriteShader");
	//material->SetRenderState("AlphaBlend");
	//material->SetOpacity(0.5f);
	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& name)
{
	auto	iter = m_mapMaterial.find(name);
	if (iter == m_mapMaterial.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& name)
{
	auto	iter = m_mapMaterial.find(name);
	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMaterial.erase(iter);
		}
	}
}