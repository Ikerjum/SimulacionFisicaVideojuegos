#include "Particula.h"
#include <PxPhysicsAPI.h>
#include <iostream>

using namespace physx;

Particula::Particula(Vector3 vel, Vector3 pos, float mass, float timeOfLife)
{
	//CREATIONOFGEOMETRY
	PxSphereGeometry _sphereParticle(2);
	PxShape* shapeParticle = CreateShape(_sphereParticle);
	//TRANSFORM
	_vel = vel;
	_pos = new PxTransform(pos);
	_mass = mass;
	_acc = Vector3(0.f, 0.f, 0.f);
	_timeOfLife = timeOfLife;
	//COLOR
	Vector4 colorParticle = { 0.1f,0.7f,1.0f,1.0f };
	//RENDERITEM
	_renderItem = new RenderItem(shapeParticle,_pos,colorParticle);
	//REGISTER
	RegisterRenderItem(_renderItem);
}

Particula::~Particula()
{
	delete _pos;
	DeregisterRenderItem(_renderItem);
}

void Particula::integrate(double t, double& dumping, float acc_x, float acc_y, float acc_z)
{
	_acc.x = acc_x;
	_acc.y = acc_y;
	_acc.z = acc_z;

	//std::cout << _vel.x << "\n";
	//std::cout << _vel.y << "\n";
	//std::cout << _vel.z << "\n";

	_vel = _vel + (t * _acc);
	_vel = _vel	* pow(dumping,t);
	_pos->p = _pos->p + (_vel * t);

	std::cout << _timeOfLife << "\n";
}

void Particula::integrate_semiimp(double t, double& dumping, float acc_x, float acc_y, float acc_z)
{
}


