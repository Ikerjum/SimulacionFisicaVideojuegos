#include "GravityForceGenerator.h"
#include "checkML.h"

//FUERZA DE GRAVEDAD
GravityForceGenerator::GravityForceGenerator(Vector3 acc) : ForceGenerator(acc)
{
	setActive(true); //Por defecto tenemos gravedad siempre
	setType(ForceGenerator::GRAVITY);
}

Vector3
GravityForceGenerator::putForce(Particle* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	return getForceAcceleration() * p->getMass(); //Aplicamos gravedad
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}

Vector3 GravityForceGenerator::putForce(DynamicRigidSolid* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	return getForceAcceleration() * p->getMass(); //Aplicamos gravedad
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}

ForceGenerator*
GravityForceGenerator::clone() const
{
	return new GravityForceGenerator(*this);
}
