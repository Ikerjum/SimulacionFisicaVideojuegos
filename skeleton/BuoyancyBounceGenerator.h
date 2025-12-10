#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
#include "Particle.h"
#include "DynamicRigidSolid.h"

class BuoyancyForceGenerator : public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float V, float d, float waterHeight);
	Vector3 putForce(Particle* particle) override;
	Vector3 putForce(DynamicRigidSolid* particle) override;

	ForceGenerator* clone() const;

	virtual ~BuoyancyForceGenerator();
protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	float _waterHeight;
};