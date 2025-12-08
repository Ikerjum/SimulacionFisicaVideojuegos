#include "GravityForceGenerator.h"
#include "checkML.h"

GravityForceGenerator::GravityForceGenerator(Vector3 acc) : ForceGenerator(acc)
{
	setActive(true); //Por defecto tenemos gravedad siempre
	setType(ForceGenerator::GRAVITY);
}

Vector3
GravityForceGenerator::putForce(Particula* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	return getForceAcceleration() * p->getMass();
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}

Vector3 GravityForceGenerator::putForce(DynamicParticle* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	return getForceAcceleration() * p->getMass();
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}

ForceGenerator*
GravityForceGenerator::clone() const
{
	return new GravityForceGenerator(*this);
}
