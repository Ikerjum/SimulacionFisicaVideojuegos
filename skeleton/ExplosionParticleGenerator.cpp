#include "ExplosionParticleGenerator.h"
#include "checkML.h"
ExplosionParticleGenerator::ExplosionParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame) : ParticleGenerator(pos, model, ParticlesPerFrame)
{
    std::random_device rd;
    _mt.seed(rd());

    //GENERADORES DE FUERZAS, LO HACEMOS SOLO UNA VEZ EN LA CONSTRUCTORA
    _forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas
    _explosionForceGenerator = new ExplosionForceGenerator(Vector3(0, 0, 0), 100000.0, 50.0f, 100.0f);
    _forceGenerators.push_back(_explosionForceGenerator);
}

Particula* ExplosionParticleGenerator::generateP()
{
    Particula* newP = _modelP->clone();

    //VARIACION DE POSICION
    Vector3 basePos = _modelP->getPos().p;
    float RANGO_POS_X = 0.5f;
    float RANGO_POS_Y = 0.5f;
    float RANGO_POS_Z = 0.5f;

    Vector3 newPos = basePos + Vector3(
        _n(_mt) * RANGO_POS_X,
        _n(_mt) * RANGO_POS_Y,
        _n(_mt) * RANGO_POS_Z
    );

    newP->setPos(newPos);

    return newP;
}

Particula* ExplosionParticleGenerator::generatePInOnePosition(Vector3 pos)
{
    Particula* newP = _modelP->clone();

    //VARIACION DE POSICION
    Vector3 basePos = pos;
    float RANGO_POS_X = 0.5f;
    float RANGO_POS_Y = 0.5f;
    float RANGO_POS_Z = 0.5f;

    Vector3 newPos = basePos + Vector3(
        _n(_mt) * RANGO_POS_X,
        _n(_mt) * RANGO_POS_Y,
        _n(_mt) * RANGO_POS_Z
    );

    newP->setPos(newPos);

    return newP;
}

void ExplosionParticleGenerator::update(double t)
{
    for (int i = 0; i < _particlesPerFrame; ++i) {
        Particula* newParticle = generateP();

        if (newParticle) {
            newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
            newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * float(t));
            ApplyForces(newParticle, t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada

            _generatorParticlesV.push_back(newParticle);
        }
    }

    for (int i = 0; i < _generatorParticlesV.size(); ) {
        Particula* p = _generatorParticlesV[i];

        ApplyForces(p, t); //Aplicamos fuerzas antes de integrar y recalculamos en cada frame
        p->integrate_Verlet(t);

        if (p->getTimeOfLife() <= 0.0f || p->getPos().p.y < -20.0f) {
            delete p;
            p = nullptr;
            _generatorParticlesV[i] = _generatorParticlesV.back();
            _generatorParticlesV.pop_back();
        }
        else {
            ++i;
        }
    }
}

void ExplosionParticleGenerator::ApplyForces(Particula* newParticle, double t)
{
    newParticle->setAcc(Vector3(0, 0, 0));
    float massParticle = newParticle->getMass();
    for (int i = 0; i < _forceGenerators.size(); ++i) {
        Vector3 newForce = _forceGenerators[i]->putForce(newParticle);
        if (massParticle != 0.0f) newParticle->setAcc(newParticle->getAcc() + newForce / massParticle);
    }
    if (_explosionForceGenerator) {
        if (_explosionForceGenerator->getIsActive()) {
            _explosionForceGenerator->update(t);
            generatePInOnePosition(getPos().p);
        }
    }
}

void ExplosionParticleGenerator::triggerExplosion(Vector3 pos)
{

    if (_explosionForceGenerator && !_explosionForceGenerator->getIsActive()) {
        _explosionForceGenerator->activate(pos);
    }
}