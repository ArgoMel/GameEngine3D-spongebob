#pragma once
#include "../EngineInfo.h"

class CTile
{
private:
	friend class CTileMapComponent;
	class CTileMapComponent* m_Owner;
	EAnimation2DType    m_Anim2DType;
	ETileOption			m_TileOption;
	Vector3				m_Pos;
	Vector3				m_Size;
	Vector3				m_Center;
	Matrix				m_matWorld;
	Vector2				m_TileStart;
	Vector2				m_TileEnd;
	bool				m_Enable;
	int					m_IndexX;
	int					m_IndexY;
	int					m_Index;
	int					m_Frame;
	float				m_Time;
	float				m_FrameTime;
	float				m_PlayTime;
	float				m_PlayScale;
	bool				m_Loop;
	bool				m_Reverse;
	float				m_Opacity;
	std::vector<Animation2DFrameData>   m_vecFrameData;
	CTile();
	CTile(const CTile& tile);
	~CTile();
public:
	ETileOption GetTileOption()	const
	{
		return m_TileOption;
	}
	const Vector3& GetPos()	const
	{
		return m_Pos;
	}
	const Vector3& GetCenter()	const
	{
		return m_Center;
	}
	const Matrix& GetWorldMatrix()	const
	{
		return m_matWorld;
	}
	const Vector2& GetTileStart()	const
	{
		return m_TileStart;
	}
	const Vector2& GetTileEnd()	const
	{
		return m_TileEnd;
	}
	bool GetEnable()	const
	{
		return m_Enable;
	}	
	int GetIndexX()	const
	{
		return m_IndexX;
	}
	int GetIndexY()	const
	{
		return m_IndexY;
	}
	int GetIndex()	const
	{
		return m_Index;
	}
	float GetOpacity()	const
	{
		return m_Opacity;
	}
	Animation2DFrameData GetFrameData()
	{
		return m_vecFrameData[m_Frame];
	}
	bool FrameEmpty()	const
	{
		return m_vecFrameData.empty();
	}
	void Update(float deltaTime);
	void UpdateTransform(float deltaTime);
	CTile* Clone();
	void Save(FILE* file);
	void Load(FILE* file);
};

