#include "ExplosionForceGenerator.h"
#include "checkML.h"
#include <cmath>

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
