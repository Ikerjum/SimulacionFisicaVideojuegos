#include "Enemy.h"

Enemy::Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color) :
	DynamicRigidSolid(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), 5.f, color, tam.y, 5.0f, tam, linearVelocity, angularVelocity, gScene, gPhysics, DynamicRigidSolid::SPHERE)
{
}

void Enemy::updateEnemy(double t)
{
	ApplyForcesDynamic(t);
}

void Enemy::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Enemy::ApplyForcesDynamic(double t)
{
	_actor->clearForce();
	for (auto& fg : _forceGenerators) {
		Vector3 f = fg->putForce(this); // fg debe usar datos reales si target tiene actor
		_actor->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eFORCE);
	}
}
