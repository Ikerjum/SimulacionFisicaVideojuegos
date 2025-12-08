#include "ExplosionForceGenerator.h"
#include "checkML.h"
#include <cmath>

ExplosionForceGenerator::ExplosionForceGenerator(Vector3 center, float K, float tau, float R)
    : ForceGenerator(), _center(center), _K(K), _tau(tau), _R(R), _time(0.0f), _active(false)
{
    setActive(true); //Por defecto siempre empezamos con el generador de explosiones activado
    setType(ForceGenerator::EXPLOSION);
}

void ExplosionForceGenerator::activate(Vector3 newCenter)
{
    _center = newCenter;
    _time = 0.0f;
    _active = true;
}

void ExplosionForceGenerator::deactivate()
{
    _active = false;
}

void ExplosionForceGenerator::update(double dt)
{
    if (_active) {
        _time += dt;
        if (_time > _tau) {
            deactivate();
            _time = 0.0f;
        }
    }
}

Vector3 ExplosionForceGenerator::putForce(Particula* p)
{
    if (p == nullptr || p->getMass() == 0 || !isActive() || !_active) return Vector3(0, 0, 0);
    Vector3 pos = p->getPos().p;
    Vector3 dir = pos - _center;

    float r = dir.magnitude();
    if (r > _R || r == 0) return Vector3(0, 0, 0);

    dir.normalize();

    float temporalFactor = std::exp(-_time / _tau);
    
     Vector3 explosionForce = dir * (_K / (r * r)) * temporalFactor;

    return explosionForce;
}

Vector3 ExplosionForceGenerator::putForce(DynamicParticle* p)
{
    if (p == nullptr || p->getMass() == 0 || !isActive() || !_active) return Vector3(0, 0, 0);
    Vector3 pos = p->getPos().p;
    Vector3 dir = pos - _center;

    float r = dir.magnitude();
    if (r > _R || r == 0) return Vector3(0, 0, 0);

    dir.normalize();

    float temporalFactor = std::exp(-_time / _tau);

    Vector3 explosionForce = dir * (_K / (r * r)) * temporalFactor;

    return explosionForce;
}

ForceGenerator*
ExplosionForceGenerator::clone() const
{
    return new ExplosionForceGenerator(*this);
}
