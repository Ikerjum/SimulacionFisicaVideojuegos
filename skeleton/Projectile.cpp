#include "Projectile.h"
#include "checkML.h"
#include "Particula.h"
Projectile::Projectile(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype, PxReal tam) :
	Particula(initialDir, initialPos, Vector3(0.0f, -9.8f, 0.0f), 20.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f), tam, 200.0f)
{
	_projectileType = projectileType;
	_integratortype = integratortype;

	setPos(initialPos);
	initialDir.normalize();

	Vector3 dirParticle = initialDir;
	setVel(dirParticle * 100.f);
	setOldPos(initialPos - getVel() * Particula::OLD_POS_CONSTANT);
	setTimeOfLife(100.0f);

	switch (_projectileType) {
	case PAINT_WHITE: setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); break;
	case PAINT_BLACK: setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f)); break;
	case PAINT_RED: setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f)); break;
	case PAINT_BLUE: setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f)); break;
	case PAINT_GREEN: setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f)); break;
	case PAINT_YELLOW: setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f)); break;
	default: break;
	}
}

Projectile::~Projectile()
{
	//for (int i = 0; i < _forceGenerators.size(); i++) {
	//	if (_forceGenerators[i] != nullptr) {
	//		delete _forceGenerators[i];
	//		_forceGenerators[i] = nullptr;
	//	}
	//}
	_forceGenerators.clear();
}

void Projectile::update(double t)
{
	ApplyForces(t);

	switch (_integratortype) {
	case EULER_EXPLICIT: integrate_EulerExplicit(t); break;
	case EULER_SEMIEXPLICIT: integrate_EulerSemiImplicit(t); break;
	case VERLET: integrate_Verlet(t); break;
	default: break;
	}
}

void Projectile::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Projectile::ApplyForces(double t)
{
	this->setAcc(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < _forceGenerators.size(); ++i) {
		Vector3 newForce = _forceGenerators[i]->putForce(this);
		if (this->getMass() != 0.0f) this->setAcc(this->getAcc() + newForce / this->getMass());
	}
}

void Projectile::resetProperties(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype)
{	
	_projectileType = projectileType;
	_integratortype = integratortype;
	
	setPos(initialPos);
	initialDir.normalize();

	Vector3 dirParticle = initialDir;
	setVel(dirParticle * 100.f);
	setOldPos(initialPos - getVel() * Particula::OLD_POS_CONSTANT);
	setTimeOfLife(100.0f);

	switch (_projectileType) {
	case PAINT_WHITE: setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); break;
	case PAINT_BLACK: setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f)); break;
	case PAINT_RED: setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f)); break;
	case PAINT_BLUE: setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f)); break;
	case PAINT_GREEN: setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f)); break;
	case PAINT_YELLOW: setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f)); break;
	default: break;
	}
}
