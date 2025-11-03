#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
private:
	float _k1;
	float _k2;
public:
	WindForceGenerator(Vector3 windVel, float k1 = 0.1f, float k2 = 0.0f) 
		: ForceGenerator(Vector3(0, 0, 0), windVel), _k1(k1), _k2(k2) {}
	Vector3 putForce(Particula* p) override;
};