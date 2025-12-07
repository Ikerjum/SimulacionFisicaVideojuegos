#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
#include "Particula.h"

class BuoyancyForceGenerator : public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float V, float d, float waterHeight);
	Vector3 putForce(Particula* particle) override;
	virtual ~BuoyancyForceGenerator();
protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	float _waterHeight;
};