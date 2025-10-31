#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
public:
	WindForceGenerator(Vector3 acc) : ForceGenerator(acc) {}
	void putAcceleration(Particula* p) override;
};