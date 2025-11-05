#include "WindForceGenerator.h"

Vector3 
WindForceGenerator::putForce(Particula* p)
{
	if (p->getMass() == 0 || p == nullptr) 
		return Vector3(0, 0, 0);
	
	float massParticle = p->getMass();

	Vector3 v = p->getVel();
	Vector3 v_wind = _windVel;

	Vector3 v_rel = v_wind - v;

	float speed = v_rel.magnitude();

	Vector3 WindForce = _k1 * v_rel + _k2 * speed * v_rel;
	return WindForce;
}
