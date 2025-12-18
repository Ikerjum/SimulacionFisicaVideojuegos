#include "Enemy.h"
#include "checkML.h"
#include <iostream>

Enemy::Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color, float mass) :
	DynamicRigidSolid(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), mass, color, tam.y, 2.0f, tam, linearVelocity, angularVelocity, gScene, gPhysics, DynamicRigidSolid::SPHERE)
{
}

void Enemy::updateEnemy(double t)
{
	ApplyForcesDynamic(t);
	//update(t); //No necesitamos que los enemigos se eliminen, ya lo haran al llegar a su meta o al sobrepasar al suelo en updateEnemies/EnemyGenerator.cpp

	//DEPURACION
	//std::cout << _actor->getGlobalPose().p.x << " " << _actor->getGlobalPose().p.y << " " << _actor->getGlobalPose().p.z << "\n";
}

void Enemy::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator); //AÑADIMOS FUERZAS AL ENEMIGO
}

void Enemy::ApplyForcesDynamic(double t)
{
	_actor->clearForce(PxForceMode::eFORCE);
	for (auto& fg : _forceGenerators) {
		Vector3 f = fg->putForce(this); // fg debe usar datos reales si target tiene actor
		_actor->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eACCELERATION); //APLICAMOS FUERZAS AL ENEMIGO QUE ES SOLIDO RIGIDO DINAMICO
	}
}
