#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particula* other);
	Vector3 putForce(Particula* particle) override;
	inline void setK(double k) { _k = k; }
	virtual ~SpringForceGenerator() {}
protected:
	double _k;
	double _resting_length;
	Particula* _other;
};