#include "GravityForceGenerator.h"

Vector3 
GravityForceGenerator::putForce(Particula* p)
{
	if (p->getMass() != 0 || p != nullptr) { //Si la masa es distinta de 0 o p == nullptr
		return getForceAcceleration() * p->getMass();
	}
	//En caso de que la masa sea 0, no tenemos aceleracion
	return Vector3(0, 0, 0);
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}
