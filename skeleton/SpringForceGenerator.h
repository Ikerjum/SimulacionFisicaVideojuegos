#pragma once

#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particula* other);
	Vector3 putForce(Particula* particle) override;
	Vector3 putForce(DynamicParticle* p) override;
	inline void setK(double k);
	virtual ~SpringForceGenerator();
	ForceGenerator* clone() const;

protected:
	double _k;
	double _resting_length;
	Particula* _other;
};