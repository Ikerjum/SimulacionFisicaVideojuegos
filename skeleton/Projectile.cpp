#include "Projectile.h"
#include "Particula.h"

Projectile::Projectile(Vector3 initialPos,Vector3 initialDir, ProjectileType projectileType, IntegratorType integratortype, float timeOfLife) :
	Particula(initialPos, initialDir, Vector3(0.0f, -9.8f, 0.0f), 1.0f,Vector4(0.0f, 0.0f, 0.0f, 1.0f), timeOfLife)
{
	_projectileType = projectileType;
	_integratortype = integratortype;

	setPos(initialPos);
	
	initialDir.normalize();

	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	setAcc(accParticle);

	Vector3 velParticle = Vector3(initialPos.x, initialPos.y, initialPos.z);

	switch (_projectileType) {
	case CANNON_BULLET:
		setVel(velParticle * 250.f);
		setColor(Vector4(1.0f,0.0f,0.0f,1.0f));
		break;
	case TANK_BULLET:
		setVel(velParticle * 1800.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PISTOL:
		setVel(velParticle * 330.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case LASER_PISTOL:
		setVel(velParticle * 10000.f);
		setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	}

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

	incrementTimeOfLife(t);
}

void Projectile::resetPhysics(Vector3 initialPos,Vector3 initialVel, ProjectileType projectileType, IntegratorType integratortype)
{
	_projectileType = projectileType;
	_integratortype = integratortype;

	setPos(initialPos);
	setVel(initialVel);

	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	setAcc(accParticle);

	Vector3 velParticle = Vector3(initialPos.x, initialPos.y, initialPos.z);

	switch (_projectileType) {
	case CANNON_BULLET:
		setVel(velParticle * 250.f);
		setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case TANK_BULLET:
		setVel(velParticle * 1800.f);
		setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	case PISTOL:
		setVel(velParticle * 330.f);
		setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	case LASER_PISTOL:
		setVel(velParticle * 10000.f);
		setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	}
}
