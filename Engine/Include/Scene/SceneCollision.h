#pragma once
#include "../EngineInfo.h"

struct Section2D
{
	std::vector<class CCollisionSection2D*>	vecSection;
	Vector2	worldStart;
	Vector2	sectionSize;
	Vector2	worldSize;
	int		countX;
	int		countY;
	int		count;
	Section2D()
		: countX(0)
		, countY(0)
		, count(0)
	{
	}
};

struct Section3D
{
	std::vector<class CCollisionSection3D*>	vecSection;
	Vector3	worldStart;
	Vector3	sectionSize;
	Vector3	worldSize;
	int		countX;
	int		countY;
	int		countZ;
	int		count;
	Section3D()
		: countX(0)
		, countY(0)
		, countZ(0)
		, count(0)
	{
	}
};

class CSceneCollision
{
private:
	friend class CScene;
	class CScene* m_Owner;
	CSharedPtr<class CCollider> m_MouseCollision;
	Section2D	m_Section2D;
	Section3D	m_Section3D;
	bool		m_CollisionWidget;
	std::list<CSharedPtr<class CCollider>>	m_ColliderList;
	std::unordered_map<std::string, PixelInfo*>	m_mapPixelCollision;
	CSceneCollision();
	~CSceneCollision();
	static bool SortPickginSection(int src, int dest);
	static bool SortColliderList(CSharedPtr<class CCollider3D> src, CSharedPtr<class CCollider3D> dest);
	void CollisionMouse(float deltaTime);
	void CheckSection(class CCollider* collider);
public:
	void AddCollider(class CCollider* collider);
	bool CollisionWidget();
	bool Init();
	void Update(float deltaTime);
	void CreateSection2D(int countX, int countY, const Vector2& worldStart, const Vector2& sectionSize);
	void CreateSection3D(int countX, int countY, int countZ, const Vector3& worldStart, 
		const Vector3& sectionSize);
	void Save(FILE* file);
	void Load(FILE* file);
	bool CreatePixelCollision(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreatePixelCollisionFullPath(const std::string& name, const TCHAR* fullPath);
	bool CreatePixelCollisionMultibyte(const std::string& name, const char* fileName, 
		const std::string& pathName = TEXTURE_PATH);
	bool CreatePixelCollisionMultibyteFullPath(const std::string& name, const char* fullPath);
	PixelInfo* FindPixelCollision(const std::string& name);
	bool Picking(PickingResult& result);
};

