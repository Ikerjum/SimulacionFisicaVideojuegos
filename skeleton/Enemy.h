#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particula.h"
#include "ForceGenerator.h"
#include "BuoyancyBounceGenerator.h"
#include <vector>
#include <array>

class Enemy {
private:
	double _timeOfLife;

	RenderItem* _item;
	PxRigidDynamic* _actor;
	PxShape* _shape;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

public:
	Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color);
	~Enemy();
	void updateBullet(double t);
	int getTimeOfLife() const { return _timeOfLife; }
};