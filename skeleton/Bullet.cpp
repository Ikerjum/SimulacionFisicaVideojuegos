#include "Bullet.h"
#include <iostream>

//BULLET COMO SOLIDO RIGIDO DINAMICO AL CUAL SE LE APLICAN FUERZAS A SU RESPECTIVO ACTOR
Bullet::Bullet(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color) :
	DynamicRigidSolid(Vector3(0,0,0),pos,Vector3(0,0,0),5.f,color,tam.y,2.0f,tam,linearVelocity,angularVelocity,gScene,gPhysics,DynamicRigidSolid::BOX)
{
	//_forceGenerators.push_back(new BuoyancyForceGenerator(0.1f, 0.2f, 1000.0f, 2.5f));
}

void Bullet::updateBullet(double t)
{
	ApplyForcesDynamic(t);
	//integrate_Verlet(t);
	update(t); //Para controlar su tiempo de vida
}


void Bullet::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Bullet::ApplyForcesDynamic(double t)
{
	_actor->clearForce();
	for (auto& fg : _forceGenerators) {
		Vector3 f = fg->putForce(this); // fg debe usar datos reales si target tiene actor
		_actor->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eACCELERATION);
	}
}

