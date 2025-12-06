#include "BuoyancyBounceGenerator.h"
#include "checkML.h"
BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d)
{
	_height = h;
	_volume = V;
	_liquid_density = d;
}

Vector3 BuoyancyForceGenerator::putForce(Particula* particle)
{
	float h = particle->getPos().p.y;
	float h0 = _liquid_particle->getPos().p.y;

	Vector3 force(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}
	force.y = _liquid_density * _volume * immersed * 9.8;
	
	return force;
}

BuoyancyForceGenerator::~BuoyancyForceGenerator()
{
}
