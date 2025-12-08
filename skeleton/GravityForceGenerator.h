#pragma once

#include "Particula.h"
#include "DynamicParticle.h"
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 acc);
	Vector3 putForce(Particula* p) override;
	Vector3 putForce(DynamicParticle* p) override;
	ForceGenerator* clone() const;

};