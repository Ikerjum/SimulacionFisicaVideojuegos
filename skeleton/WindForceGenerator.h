#pragma once

#include "Particle.h"
#include "DynamicRigidSolid.h"
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
private:
	float _k1;
	float _k2;
	Vector3 _windVel;
public:
	WindForceGenerator(Vector3 windVel, bool active = false, float k1 = 0.1f, float k2 = 0.0f);
	Vector3 putForce(Particle* p) override;
	Vector3 putForce(DynamicRigidSolid* p) override;

	ForceGenerator* clone() const;

	void setWindVel(Vector3 windVel) { _windVel = windVel; }
	Vector3 getWindVel() const { return _windVel; }
};