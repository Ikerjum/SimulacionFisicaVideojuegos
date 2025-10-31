#pragma once

#include "Particula.h"
#include "core.hpp"

class ForceGenerator {
private:
	Vector3 _ForceAcceleration;
	Vector3 _ForceVelocity; //Para el viento
public:
	//ForceGenerator() : _ForceAcceleration(), _ForceVelocity() {};
	ForceGenerator(Vector3 acc) : _ForceAcceleration(acc), _ForceVelocity() {};
	ForceGenerator(Vector3 acc, Vector3 vel) : _ForceAcceleration(acc), _ForceVelocity(acc) {}; //Para el viento
	~ForceGenerator() {};
	virtual void putAcceleration(Particula* p) = 0;
	Vector3 getForceAcceleration() { return _ForceAcceleration; }
	void setForceAcceleration(Vector3 acc) { _ForceAcceleration = acc; }
};