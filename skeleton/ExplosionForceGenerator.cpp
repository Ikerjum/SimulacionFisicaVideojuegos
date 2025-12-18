#include "ExplosionForceGenerator.h"
#include "checkML.h"
#include <cmath>

//FUERZA DE EXPLOSION
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

Vector3 ExplosionForceGenerator::putForce(Particle* p)
{
    if (p == nullptr || p->getMass() == 0 || !isActive() || !_active) return Vector3(0, 0, 0);
    Vector3 pos = p->getPos().p; //Posicionn de la particula generada
    Vector3 dir = pos - _center;  //Direccion de la explosion que depende de la posicion en la que se genere la particula

    float r = dir.magnitude(); //Magnitud de la direccion, para comprobar si su valor no es nulo o es mayor al radio de la explosion para saber si esta demasiado lejos del alcance del radio de la explosion
    if (r > _R || r == 0) return Vector3(0, 0, 0);

    dir.normalize();

    float temporalFactor = std::exp(-_time / _tau);
    
     Vector3 explosionForce = dir * (_K / (r * r)) * temporalFactor; //Fuerza de explosion dependiendo de la direccion y de la distancia al centro, teniendo en cuenta el tiempo transcurrido

    return explosionForce;
}

Vector3 ExplosionForceGenerator::putForce(DynamicRigidSolid* p)
{
    if (p == nullptr || p->getMass() == 0 || !isActive() || !_active) return Vector3(0, 0, 0);
    Vector3 pos = p->getPos().p; //Posicionn de la particula generada
    Vector3 dir = pos - _center; //Direccion de la explosion que depende de la posicion en la que se genere la particula

    float r = dir.magnitude(); //Magnitud de la direccion, para comprobar si su valor no es nulo o es mayor al radio de la explosion para saber si esta demasiado lejos del alcance del radio de la explosion
    if (r > _R || r == 0) return Vector3(0, 0, 0);

    dir.normalize();

    float temporalFactor = std::exp(-_time / _tau);

    Vector3 explosionForce = dir * (_K / (r * r)) * temporalFactor; //Fuerza de explosion dependiendo de la direccion y de la distancia al centro, teniendo en cuenta el tiempo transcurrido

    return explosionForce;
}

ForceGenerator*
ExplosionForceGenerator::clone() const
{
    return new ExplosionForceGenerator(*this);
}
