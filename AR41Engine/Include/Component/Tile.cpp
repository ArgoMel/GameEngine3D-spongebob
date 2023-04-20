#include "Tile.h"
#include "TileMapComponent.h"

CTile::CTile()
	: m_Owner(nullptr)
	, m_Enable(true)
	, m_TileOption(ETileOption::None)
	, m_IndexX(0)
	, m_IndexY(0)
	, m_Index(0)
	, m_Anim2DType(EAnimation2DType::None)
	, m_Frame(0)
	, m_Time(0.f)
	, m_FrameTime(0.f)
	, m_PlayTime(1.f)
	, m_PlayScale(1.f)
	, m_Loop(true)
	, m_Reverse(false)
	, m_Opacity(1.f)
{
}

CTile::CTile(const CTile& tile)
{
	*this = tile;
	m_Owner = nullptr;
}

CTile::~CTile()
{
}
// 애니메이션 연산
void CTile::Update(float deltaTime)
{
	
}
// 트랜스폼 연산
void CTile::UpdateTransform(float deltaTime)
{
	Vector3	ownerPos = m_Owner->GetWorldPos();
	Vector3	pos = ownerPos + m_Pos;
	Matrix	matScale;
	Matrix matTranslate;
	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matTranslate.Translation(pos);
	m_matWorld = matScale * matTranslate;
}

CTile* CTile::Clone()
{
	return new CTile(*this);
}

void CTile::Save(FILE* file)
{
	fwrite(&m_Enable, sizeof(bool), 1, file);
	fwrite(&m_TileOption, sizeof(ETileOption), 1, file);
	fwrite(&m_Pos, sizeof(Vector3), 1, file);
	fwrite(&m_Size, sizeof(Vector3), 1, file);
	fwrite(&m_Center, sizeof(Vector3), 1,file);
	fwrite(&m_IndexX, sizeof(int), 1, file);
	fwrite(&m_IndexY, sizeof(int), 1, file);
	fwrite(&m_Index, sizeof(int), 1, file);
	fwrite(&m_TileStart, sizeof(Vector2), 1, file);
	fwrite(&m_TileEnd, sizeof(Vector2), 1, file);
	int	frameCount = (int)m_vecFrameData.size();
	fwrite(&frameCount, sizeof(int), 1, file);
	for (int i = 0; i < frameCount; ++i)
	{
		fwrite(&m_vecFrameData[i], sizeof(Animation2DFrameData), 1, file);
	}
	fwrite(&m_Anim2DType, sizeof(EAnimation2DType), 1, file);
	fwrite(&m_Frame, sizeof(int), 1, file);
	fwrite(&m_FrameTime, sizeof(float), 1, file);
	fwrite(&m_PlayTime, sizeof(float), 1, file);
	fwrite(&m_PlayScale, sizeof(float), 1, file);
	fwrite(&m_Loop, sizeof(bool), 1, file);
	fwrite(&m_Reverse, sizeof(bool), 1, file);
	fwrite(&m_Opacity, sizeof(float), 1, file);
}

void CTile::Load(FILE* file)
{
	fread(&m_Enable, sizeof(bool), 1, file);
	fread(&m_TileOption, sizeof(ETileOption), 1, file);
	fread(&m_Pos, sizeof(Vector3), 1, file);
	fread(&m_Size, sizeof(Vector3), 1, file);
	fread(&m_Center, sizeof(Vector3), 1, file);
	fread(&m_IndexX, sizeof(int), 1, file);
	fread(&m_IndexY, sizeof(int), 1, file);
	fread(&m_Index, sizeof(int), 1, file);
	fread(&m_TileStart, sizeof(Vector2), 1, file);
	fread(&m_TileEnd, sizeof(Vector2), 1, file);
	int	frameCount = 0;
	fread(&frameCount, sizeof(int), 1, file);
	if (frameCount > 0)
	{
		m_vecFrameData.resize((size_t)frameCount);
	}
	for (int i = 0; i < frameCount; ++i)
	{
		fread(&m_vecFrameData[i], sizeof(Animation2DFrameData), 1, file);
	}
	fread(&m_Anim2DType, sizeof(EAnimation2DType), 1, file);
	fread(&m_Frame, sizeof(int), 1, file);
	fread(&m_FrameTime, sizeof(float), 1, file);
	fread(&m_PlayTime, sizeof(float), 1, file);
	fread(&m_PlayScale, sizeof(float), 1, file);
	fread(&m_Loop, sizeof(bool), 1, file);
	fread(&m_Reverse, sizeof(bool), 1, file);
	fread(&m_Opacity, sizeof(float), 1, file);
}
