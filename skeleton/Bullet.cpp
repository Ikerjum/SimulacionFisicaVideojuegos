#include "Bullet.h"
#include <iostream>

Bullet::Bullet(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color) :
	DynamicParticle(Vector3(0,0,0),pos,Vector3(0,0,0),5.f,color,tam.y,5.0f,tam,linearVelocity,angularVelocity,gScene,gPhysics,DynamicParticle::SPHERE)
{
}

void Bullet::updateBullet(double t)
{
	ApplyForcesDynamic(t);
	integrate_Verlet(t);
}


void Bullet::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Bullet::ApplyForcesDynamic(double t)
{
	_actor->clearForce();
	for (auto& fg : _forceGenerators) {
		Vector3 f = fg->putForce(this); // fg debe usar datos reales si target tiene actor
		_actor->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eFORCE);
	}
}

