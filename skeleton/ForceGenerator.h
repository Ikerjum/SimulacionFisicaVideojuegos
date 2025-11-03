#pragma once

#include "Particula.h"
#include "core.hpp"

class ForceGenerator {
private:
	Vector3 _ForceAcceleration;
	Vector3 _ForceVelocity; //Para el viento
public:
	ForceGenerator() : _ForceAcceleration(), _ForceVelocity() {};
	ForceGenerator(Vector3 acc) : _ForceAcceleration(acc), _ForceVelocity() {};
	ForceGenerator(Vector3 acc, Vector3 vel) : _ForceAcceleration(acc), _ForceVelocity(vel) {}; //Para el viento
	~ForceGenerator() {};
	virtual Vector3 putForce(Particula* p) = 0;
	virtual void update(double dt) {}

	Vector3 getForceAcceleration() { return _ForceAcceleration; }
	Vector3 getForceVelocity() { return _ForceVelocity; }
	void setForceAcceleration(Vector3 acc) { _ForceAcceleration = acc; }
	void setForceVelocity(Vector3 vel) { _ForceVelocity = vel; }
};