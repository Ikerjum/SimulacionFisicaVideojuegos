#include "WindForceGenerator.h"
#include "checkML.h"

WindForceGenerator::WindForceGenerator(Vector3 windVel, bool active, float k1, float k2)
	: ForceGenerator(Vector3(0, 0, 0), windVel), _windVel(windVel), _k1(k1), _k2(k2)
{
	setActive(active);
	setType(ForceGenerator::WIND);
}

Vector3
WindForceGenerator::putForce(Particle* p)
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

Vector3 
WindForceGenerator::putForce(DynamicRigidSolid* p) {
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	float massParticle = p->getMass();

	Vector3 v = p->getLinearVelocity();
	Vector3 v_wind = _windVel;

	Vector3 v_rel = v_wind - v;

	float speed = v_rel.magnitude();

	Vector3 WindForce = _k1 * v_rel + _k2 * speed * v_rel;
	return WindForce;
}

ForceGenerator* 
WindForceGenerator::clone() const
{
	return new WindForceGenerator(*this);
}
