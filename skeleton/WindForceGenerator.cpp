#include "WindForceGenerator.h"
#include "checkML.h"
#ifdef _DEBUG
#define new DBG_NEW
#endif

Vector3 
WindForceGenerator::putForce(Particula* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive()) 
		return Vector3(0, 0, 0);
	
	float massParticle = p->getMass();

	Vector3 v = p->getVel();
	Vector3 v_wind = _windVel;

	Vector3 v_rel = v_wind - v;

	float speed = v_rel.magnitude();

	Vector3 WindForce = _k1 * v_rel + _k2 * speed * v_rel;
	return WindForce;
}
