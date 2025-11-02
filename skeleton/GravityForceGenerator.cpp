#include "GravityForceGenerator.h"

void GravityForceGenerator::putAcceleration(Particula* p)
{
	if (p->getMass() != 0) { //Si la masa es distinta de 0
		p->setAcc(Vector3(p->getAcc().x,
						  getForceAcceleration().y / p->getMass() + p->getAcc().y,
						  p->getAcc().z));
	}
	else { //En caso de que la masa sea 0, no tenemos aceleracion
		p->setAcc(Vector3(0,0,0));
	}
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}
