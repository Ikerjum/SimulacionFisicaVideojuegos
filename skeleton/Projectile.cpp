#include "Projectile.h"
#include "Particula.h"

Projectile::Projectile(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype) :
	Particula(initialDir, initialPos, Vector3(0.0f, -9.8f, 0.0f), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 2.0f, 40.0f)
{
	_projectileType = projectileType;
	_integratortype = integratortype;
	_damping = 0.995;

	setPos(initialPos);

	initialDir.normalize();

	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	setAcc(accParticle);

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
	}

	Vector3 actualVel = getVel();
	setOldPos(initialPos - actualVel * 0.001f);
	setTimeOfLife(40.0f);
}

void Projectile::update(double t)
{
	switch (_integratortype)
	{
	case EULER_EXPLICIT:
		integrate_EulerExplicit(t, _damping);
		break;
	case EULER_SEMIEXPLICIT:
		integrate_EulerSemiImplicit(t, _damping);
		break;
	case VERLET:
		integrate_Verlet(t, _damping);
		break;
	default:
		break;
	}
}

void Projectile::resetPhysics(Vector3 initialPos, Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype)
{
	_projectileType = projectileType;
	_integratortype = integratortype;
	_damping = 0.995;


	setPos(initialPos);
	initialDir.normalize();
	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	setAcc(accParticle);

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
	}

	Vector3 actualVel = getVel();
	setOldPos(initialPos - actualVel * 0.001f);
	setTimeOfLife(40.0f);
}