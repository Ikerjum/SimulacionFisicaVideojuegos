#pragma once
#include <PxRigidStatic.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

class Ground
{
private:
	physx::PxRigidStatic* _groundRigid;
	RenderItem* _groundItem;
	RenderItem* _path1;
	RenderItem* _path2;
	Vector3 _pos;
public:
	Ground(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos);
	~Ground();

	Vector3 getPos() const { return _pos; }
};

