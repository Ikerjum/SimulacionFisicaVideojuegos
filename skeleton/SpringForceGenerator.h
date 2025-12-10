#pragma once

#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other);
	Vector3 putForce(Particle* particle) override;
	Vector3 putForce(DynamicRigidSolid* p) override;
	inline void setK(double k);
	virtual ~SpringForceGenerator();
	ForceGenerator* clone() const;

protected:
	double _k;
	double _resting_length;
	Particle* _other;
};