#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 acc) : ForceGenerator(acc) {}
	Vector3 putForce(Particula* p) override;
};