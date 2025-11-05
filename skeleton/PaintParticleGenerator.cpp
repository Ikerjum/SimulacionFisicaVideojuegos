#include "PaintParticleGenerator.h"

PaintParticleGenerator::PaintParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame) :
	ParticleGenerator(pos,model,ParticlesPerFrame), _explosionForceGenerator()
{
    std::random_device rd;
    _mt.seed(rd());

    //GENERADORES DE FUERZAS, LO HACEMOS SOLO UNA VEZ EN LA CONSTRUCTORA
    //_forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas
    _explosionForceGenerator = new ExplosionForceGenerator(Vector3(0, 0, 0), 100000.0, 1.0f, 1.0f);
    _forceGenerators.push_back(_explosionForceGenerator);
}

Particula* PaintParticleGenerator::generateP()
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

void PaintParticleGenerator::update(double t)
{
    _explosionForceGenerator->update(t);
    std::cout << "ACTIVE: " << _explosionForceGenerator->getIsActive() << "\n";
    if (_explosionForceGenerator->getIsActive()) {

        for (int i = 0; i < _particlesPerFrame; ++i) {
            Particula* newParticle = generateP();
            if (newParticle) {
                newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
                newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * float(t));
                newParticle->setColor({ 1.0f,1.0f,1.0f,1.0f });
                ApplyForces(newParticle, t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada

                _generatorParticlesV.push_back(newParticle);
            }
        }

        //PARA LAS PARTICULAS PINTADAS
        for (int i = 0; i < _particlesPerFrame; ++i) {
            Particula* newParticle = generateP();
            if (newParticle) {
                newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
                newParticle->setTimeOfLife(100.0f);
                newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * float(t));
                newParticle->setColor({ 1.0f,0.3f,0.0f,1.0f });
                //ApplyForces(newParticle, t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada

                _paintParticles.push_back(newParticle);
            }
        }
    }

    if (!_generatorParticlesV.empty()) {
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

    if (!_paintParticles.empty()) {
        for (int i = 0; i < _paintParticles.size(); ) {
            Particula* p = _paintParticles[i];
            //ApplyForces(p, t); //Aplicamos fuerzas antes de integrar y recalculamos en cada frame
            p->integrate_Verlet(t);
            if (p->getTimeOfLife() <= 0.0f) {
                delete p;
                p = nullptr;
                _paintParticles[i] = _paintParticles.back();
                _paintParticles.pop_back();
            }
            else {
                ++i;
            }
        }
    }
    
}

void PaintParticleGenerator::ApplyForces(Particula* newParticle, double t)
{
    newParticle->setAcc(Vector3(0, 0, 0));
    float massParticle = newParticle->getMass();
    for (int i = 0; i < _forceGenerators.size(); ++i) {
        Vector3 newForce = _forceGenerators[i]->putForce(newParticle);
        if (massParticle != 0.0f) newParticle->setAcc(newParticle->getAcc() + newForce / massParticle);
    }
}

void PaintParticleGenerator::triggerExplosion(Vector3 pos)
{

    if (_explosionForceGenerator && !_explosionForceGenerator->getIsActive()) {
        _explosionForceGenerator->activate(pos);
    }
}

void PaintParticleGenerator::unpaint()
{
    if (_paintParticles.empty()) return;

    for (int i = 0; i < _paintParticles.size(); ) {
        Particula* p = _paintParticles[i];
        delete p;
        p = nullptr;
        _paintParticles[i] = _paintParticles.back();
        _paintParticles.pop_back();
    }
}
