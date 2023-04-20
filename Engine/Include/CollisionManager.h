#pragma once
#include "EngineInfo.h"

class CCollisionManager
{
	DECLARE_SINGLE(CCollisionManager)
private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;
	std::vector<CollisionChannel*>	m_vecChannel;
	Box2DInfo ConvertBox2DInfo(const Sphere2DInfo& info);
	Box2DInfo ConvertBox2DInfo(const OBB2DInfo& info);
	Box2DInfo OverlapBox2D(const Box2DInfo& src, const Box2DInfo& dest);
	Box2DInfo OverlapBox2D(const Box2DInfo& src, const Sphere2DInfo& dest);
	Box2DInfo OverlapBox2D(const Box2DInfo& src, const OBB2DInfo& dest);
	void ComputeHitPoint(Vector2& hitPoint, const Box2DInfo& src, const Box2DInfo& dest);

	CubeInfo ConvertCubeInfo(const Sphere3DInfo& info);
	CubeInfo ConvertCubeInfo(const OBB3DInfo& info);
	CubeInfo OverlapCube(const CubeInfo& src, const CubeInfo& dest);
	CubeInfo OverlapCube(const CubeInfo& src, const OBB3DInfo& dest);
	CubeInfo OverlapCube(const CubeInfo& src, const Sphere3DInfo& dest);
	void ComputeHitPoint(Vector3& hitPoint, const CubeInfo& src, const CubeInfo& dest);
public:
	std::unordered_map<std::string, CollisionProfile*>::iterator MapProfileBegin()
	{
		return  m_mapProfile.begin();
	}
	std::unordered_map<std::string, CollisionProfile*>::iterator MapProfileEnd()
	{
		return  m_mapProfile.end();
	}
	bool Init();
	bool CreateProfile(const std::string& name, const std::string& channelName, bool enable, 
		ECollision_Interaction baseInteraction = ECollision_Interaction::Collision);
	bool SetCollisionInteraction(const std::string& name, const std::string& channelName, 
		ECollision_Interaction interaction);
	bool SetCollisionInteractionAllChannel(const std::string& name, ECollision_Interaction interaction);
	bool CheckCollisionInteraction(const std::string& name, const std::string& channelName,
		ECollision_Interaction interaction);
	bool CreateChannel(const std::string& name, ECollision_Interaction interaction = ECollision_Interaction::Collision);
	CollisionProfile* FindProfile(const std::string& name);
	void GetProfileNames(std::vector<std::string>& vecNames);

	bool CollisionBox2DToBox2D(Vector2& hitPoint, class CColliderBox2D* src, class CColliderBox2D* dest);
	bool CollisionBox2DToOBB2D(Vector2& hitPoint, class CColliderBox2D* src, class CColliderOBB2D* dest);
	bool CollisionBox2DToSphere2D(Vector2& hitPoint, class CColliderBox2D* src, class CColliderSphere2D* dest);
	bool CollisionBox2DToPixel(Vector2& hitPoint, class CColliderBox2D* src, class CColliderPixel* dest);

	bool CollisionSphere2DToOBB2D(Vector2& hitPoint, class CColliderSphere2D* src, class CColliderOBB2D* dest);
	bool CollisionSphere2DToSphere2D(Vector2& hitPoint, class CColliderSphere2D* src, class CColliderSphere2D* dest);
	bool CollisionSphere2DToPixel(Vector2& hitPoint, class CColliderSphere2D* src, class CColliderPixel* dest);

	bool CollisionOBB2DToOBB2D(Vector2& hitPoint, class CColliderOBB2D* src, class CColliderOBB2D* dest);
	bool CollisionOBB2DToPixel(Vector2& hitPoint, class CColliderOBB2D* src, class CColliderPixel* dest);

	bool CollisionPixelToPixel(Vector2& hitPoint, class CColliderPixel* src, class CColliderPixel* dest);

	bool CollisionPointToBox2D(Vector2& hitPoint, const Vector2& src, class CColliderBox2D* dest);
	bool CollisionPointToOBB2D(Vector2& hitPoint, const Vector2& src, class CColliderOBB2D* dest);
	bool CollisionPointToSphere2D(Vector2& hitPoint, const Vector2& src, class CColliderSphere2D* dest);
	bool CollisionPointToPixel(Vector2& hitPoint, const Vector2& src, class CColliderPixel* dest);

	bool CollisionBox2DToBox2D(Vector2& hitPoint, const Box2DInfo& src, const Box2DInfo& dest);
	bool CollisionBox2DToOBB2D(Vector2& hitPoint, const Box2DInfo& src, const OBB2DInfo& dest);
	bool CollisionBox2DToSphere2D(Vector2& hitPoint, const Box2DInfo& src, const Sphere2DInfo& dest);
	bool CollisionBox2DToPixel(Vector2& hitPoint, const Box2DInfo& src, const PixelInfo& dest);
	bool CollisionSphere2DToOBB2D(Vector2& hitPoint, const Sphere2DInfo& src, const OBB2DInfo& dest);
	bool CollisionSphere2DToSphere2D(Vector2& hitPoint, const Sphere2DInfo& src, const Sphere2DInfo& dest);
	bool CollisionSphere2DToPixel(Vector2& hitPoint, const Sphere2DInfo& src, const PixelInfo& dest);
	bool CollisionOBB2DToOBB2D(Vector2& hitPoint, const OBB2DInfo& src, const OBB2DInfo& dest);
	bool CollisionOBB2DToPixel(Vector2& hitPoint, const OBB2DInfo& src, const PixelInfo& dest);
	bool CollisionPixelToPixel(Vector2& hitPoint, const PixelInfo& src, const PixelInfo& dest);
	bool CollisionPointToBox2D(Vector2& hitPoint, const Vector2& src, const Box2DInfo& dest);
	bool CollisionPointToOBB2D(Vector2& hitPoint, const Vector2& src, const OBB2DInfo& dest);
	bool CollisionPointToSphere2D(Vector2& hitPoint, const Vector2& src, const Sphere2DInfo& dest);
	bool CollisionPointToPixel(Vector2& hitPoint, const Vector2& src, const PixelInfo& dest);
//3d
	bool CollisionCubeToCube(Vector3& hitPoint, class CColliderCube* src, class CColliderCube* dest);
	bool CollisionCubeToOBB3D(Vector3& hitPoint, class CColliderCube* src, class CColliderOBB3D* dest);
	bool CollisionCubeToSphere3D(Vector3& hitPoint, class CColliderCube* src, class CColliderSphere3D* dest);
	bool CollisionOBB3DToOBB3D(Vector3& hitPoint, class CColliderOBB3D* src, class CColliderOBB3D* dest);
	bool CollisionOBB3DToCube(Vector3& hitPoint, class CColliderOBB3D* src, class CColliderCube* dest);
	bool CollisionSphere3DToCube(Vector3& hitPoint, class CColliderSphere3D* src, class CColliderCube* dest);
	bool CollisionSphere3DToOBB3D(Vector3& hitPoint, class CColliderSphere3D* src, class CColliderOBB3D* dest);
	bool CollisionSphere3DToSphere3D(Vector3& hitPoint, class CColliderSphere3D* src, class CColliderSphere3D* dest);
	bool CollisionCubeToCube(Vector3& hitPoint, const CubeInfo& src, const CubeInfo& dest);
	bool CollisionCubeToOBB3D(Vector3& hitPoint, const CubeInfo& src, const OBB3DInfo& dest);
	bool CollisionCubeToSphere3D(Vector3& hitPoint, const CubeInfo& src, const Sphere3DInfo& dest);
	bool CollisionOBB3DToOBB3D(Vector3& hitPoint, const OBB3DInfo& src, const OBB3DInfo& dest);
	bool CollisionOBB3DToCube(Vector3& hitPoint, const OBB3DInfo& src, const CubeInfo& dest);
	bool CollisionSphere3DToCube(Vector3& hitPoint, const Sphere3DInfo& src, const CubeInfo& dest);
	bool CollisionSphere3DToOBB3D(Vector3& hitPoint, const Sphere3DInfo& src, const OBB3DInfo& dest);
	bool CollisionSphere3DToSphere3D(Vector3& hitPoint, const Sphere3DInfo& src, const Sphere3DInfo& dest);

	bool CollisionRayToSphere(PickingResult& result, const Ray& ray, const Vector3& center, float radius);
};

