#include "Defense.h"
#include "checkML.h"

Defense::Defense(Vector3 initialPos, Vector3 initialDir, Vector4 color, PxReal tam) :
	Particula(initialDir, initialPos, Vector3(0.0f, -9.8f, 0.0f), 20.0f, color, tam, 200.0f)
{
	_buoyancyForce = new BuoyancyForceGenerator(0.1f, 0.2f, 1000.0f, 0.0f);
	_forceGenerators.push_back(_buoyancyForce);
}

Defense::~Defense() {
	_forceGenerators.clear();
	if (_buoyancyForce) {
		delete _buoyancyForce;
		_buoyancyForce = nullptr;
	}
}

void
Defense::update(double t) {
	ApplyForces(t);
	integrate_Verlet(t);
}

void Defense::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Defense::ApplyForces(double t) {
	this->setAcc(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < _forceGenerators.size(); ++i) {
		Vector3 newForce = _forceGenerators[i]->putForce(this);
		if (this->getMass() != 0.0f) this->setAcc(this->getAcc() + newForce / this->getMass());
	}
}
