#pragma once
#include <PxRigidStatic.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>
#include <vector>

class Ground
{
private:
	physx::PxTransform _shapeGroundTransform;

	std::vector<physx::PxRigidStatic*> _actors;
	std::vector<RenderItem*> _renderItems;
	Vector3 _pos;
	physx::PxScene* _gScene;
public:
	Ground(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos);
	void CreateGround(Vector3& pos, Vector3& scale, Vector3& offset, Vector4& color);
	void CreatePath(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3& pos, Vector3& scale, Vector3& offset, Vector4& color);
	~Ground();

	Vector3 getPos() const { return _pos; }
};

