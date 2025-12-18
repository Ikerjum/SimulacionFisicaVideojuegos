#include "WaterParticleGenerator.h"
#include "checkML.h"
//GENERADOR DE LLUVIA
WaterParticleGenerator::WaterParticleGenerator(Vector3 pos, Particle* model, int ParticlesPerFrame) : ParticleGenerator(pos,model,ParticlesPerFrame)
{
    std::random_device rd;
    _mt.seed(rd());

    //Ejemplos de generadores de fuerzas para aplicar a la lluvia
    //_forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas
    //_forceGenerators.push_back(new WindForceGenerator(Vector3(100.0, 0, 0.0),0.5f,0.02f));
    
    //_bounceGenerator = new BuoyancyForceGenerator(0.1f, 1.0f, 1000.0f, 0.0f);
    //_forceGenerators.push_back(_bounceGenerator);
    
    //_explosionForceGenerator = new ExplosionForceGenerator(Vector3(0, 0, 0), 100000.0, 50.0f, 50.0f);
    //_forceGenerators.push_back(_explosionForceGenerator);
}

WaterParticleGenerator::~WaterParticleGenerator()
{
    //for (int i = 0; i < _forceGenerators.size(); ++i) {
    //    delete _forceGenerators[i];
    //    _forceGenerators[i] = nullptr;
    //}
    _forceGenerators.clear();
}

Particle* WaterParticleGenerator::generateP()
{
    Particle* newP = _modelP->clone();

    //VARIACION DE POSICION
    Vector3 basePos = getPos().p;
    float RANGO_POS_X = 30.0f;
    float RANGO_POS_Y = 0.0f;
    float RANGO_POS_Z = 30.0f;

    Vector3 newPos = basePos + Vector3(
        _n(_mt) * RANGO_POS_X,
        _n(_mt) * RANGO_POS_Y,
        _n(_mt) * RANGO_POS_Z
    );

    newP->setPos(newPos);
    //newP->setPos(basePos);

    //VARIACION DE VELOCIDAD
    Vector3 baseVel = _modelP->getVel();

    //float RANGO_VEL_X = 5.0f;
    //float RANGO_VEL_Y = 0.0f;
    //float RANGO_VEL_Z = 5.0f;
    //
    //Vector3 newVel = baseVel + Vector3(
    //    _n(_mt) * RANGO_VEL_X,
    //    _n(_mt) * RANGO_VEL_Y,
    //    _n(_mt) * RANGO_VEL_Z
    //);
    //
    //newP->setVel(newVel);
    //Adaptacion de la posicion antigua para verlet LO HACEMOS EN EL UPDATE
    /*newP->setOldPos(newP->getPos().p - newVel * Particle::OLD_POS_CONSTANT);*/

    float offset = _n(_mt);

    //float baseTam = _modelP->getTam();
    //float VARIACION_TAM = baseTam * offset;
    //
    //float baseMass = _modelP->getMass();
    //float VARIACION_MASA = baseMass * offset;
    //
    //newP->setTam(VARIACION_TAM);
    //newP->setMass(VARIACION_MASA);

    return newP;
}

void WaterParticleGenerator::update(double t)
{
    for (int i = 0; i < _particlesPerFrame; ++i) {
        Particle* newParticle = generateP();

        if (newParticle) {
            newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
            newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * float(t));
            ApplyForces(newParticle, t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada

            _generatorParticlesV.push_back(newParticle);
        }
    }

    for (int i = 0; i < _generatorParticlesV.size(); ) {
        Particle* p = _generatorParticlesV[i];

        ApplyForces(p, t); //Aplicamos fuerzas antes de integrar y recalculamos en cada frame
        p->integrate_Verlet(t);

        if (p->getTimeOfLife() <= 0.0f) {
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
