#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 acc) : ForceGenerator(acc) 
	{
		setActive(true); //Por defecto tenemos gravedad siempre
	}
	Vector3 putForce(Particula* p) override;
};