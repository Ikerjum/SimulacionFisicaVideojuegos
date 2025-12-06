#pragma once
#include "Particula.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

using namespace physx;

class Projectile : public Particula 
{
public:
	enum ProjectileType { CANNON_BULLET, TANK_BULLET, PISTOL, LASER_PISTOL, EXPLOSIVE_MINE,
		PAINT_WHITE, PAINT_BLACK, PAINT_RED, PAINT_BLUE, PAINT_GREEN, PAINT_YELLOW
	};
	enum IntegratorType { EULER_EXPLICIT, EULER_SEMIEXPLICIT, VERLET };
private:
	ProjectileType _projectileType;
	IntegratorType _integratortype;
	double _damping;
	Vector3 _initialPos;
	std::vector<ForceGenerator*> _forceGenerators;
	WindForceGenerator* _windForceGenerator;
	Vector3 _speedWind;
public:
	Projectile(Vector3 initialPos,Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype, PxReal tam);
	~Projectile();
	void update(double t);

	void addForceGenerator(ForceGenerator* newForceGenerator);
	void ApplyForces(double t);

	void resetProperties(Vector3 initialPos, Vector3 initialVel, ProjectileType projectileType, IntegratorType integratortype);

	Vector4 getProjectileColor() { return getColor(); }
};

