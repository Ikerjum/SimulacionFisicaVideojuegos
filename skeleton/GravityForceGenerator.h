#pragma once

#include "Particle.h"
#include "DynamicRigidSolid.h"
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 acc);
	Vector3 putForce(Particle* p) override;
	Vector3 putForce(DynamicRigidSolid* p) override;
	ForceGenerator* clone() const;

};