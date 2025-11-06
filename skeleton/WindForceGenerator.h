#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
private:
	float _k1;
	float _k2;
	Vector3 _windVel;
public:
	WindForceGenerator(Vector3 windVel, bool active = false, float k1 = 0.1f, float k2 = 0.0f) 
		: ForceGenerator(Vector3(0, 0, 0), windVel), _windVel(windVel), _k1(k1), _k2(k2) 
	{ 
		setActive(active); //Por defecto no tenemos viento
	}
	Vector3 putForce(Particula* p) override;

	void setWindVel(Vector3 windVel) { _windVel = windVel; }
	Vector3 getWindVel() const { return _windVel; }
};