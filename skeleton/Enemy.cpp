#include "Enemy.h"
#include <iostream>

Enemy::Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color, float mass) :
	DynamicRigidSolid(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), mass, color, tam.y, 2.0f, tam, linearVelocity, angularVelocity, gScene, gPhysics, DynamicRigidSolid::SPHERE)
{
}

void Enemy::updateEnemy(double t)
{
	//ApplyForcesDynamic(t);

	ApplyForcesDynamic(t);
	//integrate_Verlet(t);
	//update(t); //Para controlar su tiempo de vida

	//std::cout << _actor->getGlobalPose().p.x << " " << _actor->getGlobalPose().p.y << " " << _actor->getGlobalPose().p.z << "\n";
}

void Enemy::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Enemy::ApplyForcesDynamic(double t)
{
	_actor->clearForce(PxForceMode::eFORCE);
	for (auto& fg : _forceGenerators) {
		Vector3 f = fg->putForce(this); // fg debe usar datos reales si target tiene actor
		_actor->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eACCELERATION);
	}
}
