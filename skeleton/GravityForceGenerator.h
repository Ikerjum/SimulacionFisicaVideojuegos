#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 acc) : ForceGenerator(acc) {}
	void putAcceleration(Particula* p) override;
};