#pragma once
#include "Particula.h"
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;

class Projectile : public Particula 
{
public:
	enum ProjectileType { CANNON_BULLET, TANK_BULLET, PISTOL, LASER_PISTOL };
	enum IntegratorType { EULER_EXPLICIT, EULER_SEMIEXPLICIT, VERLET };
private:
	ProjectileType _projectileType;
	IntegratorType _integratortype;
	double _damping;
	Vector3 _initialPos;
public:
	Projectile(Vector3 initialPos,Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype, float timeOfLife);
	void update(double t);
	void resetPhysics(Vector3 initialPos, Vector3 initialVel, ProjectileType projectileType, IntegratorType integratortype);
};

