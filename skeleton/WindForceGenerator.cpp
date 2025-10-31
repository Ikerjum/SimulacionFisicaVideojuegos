#include "WindForceGenerator.h"

void WindForceGenerator::putAcceleration(Particula* p)
{
	if (p->getMass() != 0) { //Si la masa es distinta de 0
		p->setAcc(Vector3(getForceAcceleration().x * p->getMass() + p->getAcc().x,
			p->getAcc().y,
			getForceAcceleration().z * p->getMass() + p->getAcc().z));
	}
	else { //En caso de que la masa sea 0, no tenemos aceleracion
		p->setAcc(Vector3(0, 0, 0));
	}
	//Fg = m * g;
	// a = F / m; Esto es para modificar la aceleracion
}
