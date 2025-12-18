#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "DynamicRigidSolid.h"
#include "ForceGenerator.h"
#include "BuoyancyBounceGenerator.h"
#include <vector>
#include <array>

class Enemy : public DynamicRigidSolid {
private:
	std::vector<ForceGenerator*> _forceGenerators;
	PxTransform _lastTransform;
public:
	Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color, float mass);
	void updateEnemy(double t);
	void addForceGenerator(ForceGenerator* newForceGenerator);
	void ApplyForcesDynamic(double t);
};