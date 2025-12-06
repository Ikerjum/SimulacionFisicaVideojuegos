#include "GravityForceGenerator.h"
#include "checkML.h"
#ifdef _DEBUG
#define new DBG_NEW
#endif

Vector3 
GravityForceGenerator::putForce(Particula* p)
{
	if (p == nullptr || p->getMass() == 0 || !isActive())
		return Vector3(0, 0, 0);

	return getForceAcceleration() * p->getMass();
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}
