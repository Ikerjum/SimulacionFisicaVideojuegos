#include "Projectile.h"
#include "Particula.h"

Projectile::Projectile(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype) :
	Particula(initialDir, initialPos, Vector3(0.0f, -9.8f, 0.0f), 20.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 2.0f, 200.0f)
{
	_projectileType = projectileType;
	_integratortype = integratortype;
	_damping = 0.995;

	setPos(initialPos);

	initialDir.normalize();

	if (!_forceGenerators.empty()) {
		for (int i = 0; i < _forceGenerators.size(); ++i) {
			delete _forceGenerators[i];
			_forceGenerators[i] = nullptr;
		}
	}

	_forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas

	Vector3 velParticle = initialDir;

	switch (_projectileType) {
	case CANNON_BULLET:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case TANK_BULLET:
		setVel(velParticle * 3600.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PISTOL:
		setVel(velParticle * 660.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case LASER_PISTOL:
		setVel(velParticle * 20000.f);
		setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case EXPLOSIVE_MINE:
		setVel(velParticle * 700.f);
		setColor(Vector4(1.0f, 0.6f, 0.0f, 1.0f));
		break;
	//-----------------------------------------------
	case PAINT_WHITE:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case PAINT_BLACK:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	case PAINT_RED:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case PAINT_BLUE:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case PAINT_GREEN:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PAINT_YELLOW:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	}

	setOldPos(initialPos - getVel() * 0.001f);
	setTimeOfLife(100.0f);
}

Projectile::~Projectile()
{
	for (int i = 0; i < _forceGenerators.size(); i++) {
		if (_forceGenerators[i] != nullptr) {
			delete _forceGenerators[i];
			_forceGenerators[i] = nullptr;
		}
	}
	_forceGenerators.clear();
}

void Projectile::update(double t)
{
	ApplyForces(t);

	switch (_integratortype)
	{
	case EULER_EXPLICIT:
		integrate_EulerExplicit(t);
		break;
	case EULER_SEMIEXPLICIT:
		integrate_EulerSemiImplicit(t);
		break;
	case VERLET:
		integrate_Verlet(t);
		break;
	default:
		break;
	}
}

void Projectile::ApplyForces(double t)
{
	this->setAcc(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < _forceGenerators.size(); ++i) {
		Vector3 newForce = _forceGenerators[i]->putForce(this);
		if (this->getMass() != 0.0f) this->setAcc(this->getAcc() + newForce / this->getMass());
	}
}

void Projectile::resetPhysics(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype)
{
	_projectileType = projectileType;
	_integratortype = integratortype;
	_damping = 0.995;


	setPos(initialPos);
	initialDir.normalize();

	//Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	//setAcc(accParticle);

	if (!_forceGenerators.empty()) {
		for (int i = 0; i < _forceGenerators.size(); ++i) {
			delete _forceGenerators[i];
			_forceGenerators[i] = nullptr;
		}
	}

	_forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas

	Vector3 velParticle = initialDir;

	switch (_projectileType) {
	case CANNON_BULLET:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case TANK_BULLET:
		setVel(velParticle * 3600.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PISTOL:
		setVel(velParticle * 660.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case LASER_PISTOL:
		setVel(velParticle * 20000.f);
		setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case EXPLOSIVE_MINE:
		setVel(velParticle * 700.f);
		setColor(Vector4(1.0f, 0.6f, 0.0f, 1.0f));
		break;
		//-----------------------------------------------
	case PAINT_WHITE:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case PAINT_BLACK:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	case PAINT_RED:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case PAINT_BLUE:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case PAINT_GREEN:
		setVel(velParticle * 500.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PAINT_YELLOW:
		setVel(velParticle * 500.f);
		setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	}

	setOldPos(initialPos - getVel() * 0.001f);
	setTimeOfLife(100.0f);
}

void Projectile::addWindForce(WindForceGenerator* externalForceGenerator)
{
	_windForceGenerator = new WindForceGenerator(externalForceGenerator->getWindVel(),externalForceGenerator->isActive());
	_forceGenerators.push_back(_windForceGenerator);
}
