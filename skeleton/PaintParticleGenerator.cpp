#include "PaintParticleGenerator.h"
#include "checkML.h"

PaintParticleGenerator::PaintParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame) :
	ParticleGenerator(pos,model,ParticlesPerFrame), _explosionForceGenerator(nullptr)
{
    std::random_device rd;
    _mt.seed(rd());

    //GENERADORES DE FUERZAS ADICIONALES, LO HACEMOS SOLO UNA VEZ EN LA CONSTRUCTORA
    _explosionForceGenerator = new ExplosionForceGenerator(Vector3(0, 0, 0), 80000.0, 2.0f, 30.0f);
    _forceGenerators.push_back(_explosionForceGenerator);
}

PaintParticleGenerator::~PaintParticleGenerator()
{
    for (Defense* DF : _defenders) {
        delete DF;
        DF = nullptr;
    }
    if (_explosionForceGenerator) {
        delete _explosionForceGenerator;
        _explosionForceGenerator = nullptr;
    }
}

Particula* PaintParticleGenerator::generateP()
{
    PxReal tamP = 0.3;
    Particula* newP = _modelP->clone(tamP);

    //VARIACION DE POSICION
    Vector3 basePos = getPos().p; //Ponemos la posicion base en el proyectil, es decir en la zona donde reposicionamos el generador de particulas
    float RANGO_POS_X = 1.5f;
    float RANGO_POS_Y = 1.5f;
    float RANGO_POS_Z = 1.5f;

    Vector3 newPos = basePos + Vector3(
        _n(_mt) * RANGO_POS_X,
        _n(_mt) * RANGO_POS_Y,
        _n(_mt) * RANGO_POS_Z
    );

    newP->setPos(newPos);

    return newP;
}

Particula* PaintParticleGenerator::generateDefense()
{
    PxReal tamP = 2.0;
    Particula* newP = _modelP->clone(tamP);

    ////VARIACION DE POSICION
    //Vector3 basePos = getPos().p; //Ponemos la posicion base en el proyectil, es decir en la zona donde reposicionamos el generador de particulas
    //float RANGO_POS_X = 1.5f;
    //float RANGO_POS_Y = 1.5f;
    //float RANGO_POS_Z = 1.5f;

    //Vector3 newPos = basePos + Vector3(
    //    _n(_mt) * RANGO_POS_X,
    //    _n(_mt) * RANGO_POS_Y,
    //    _n(_mt) * RANGO_POS_Z
    //);

    //newP->setPos(newPos);

    return newP;
}

void PaintParticleGenerator::update(double t)
{
    _explosionForceGenerator->update(t); //Gestiona la duracion de la explosion

    if (!_generatorParticlesV.empty()) {
        for (int i = 0; i < _generatorParticlesV.size(); ) {
            Particula* p = _generatorParticlesV[i];
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

    if (!_defenders.empty()) {
        for (Defense* DF : _defenders) {
            if (DF) {
                DF->update(t);
            }
        }
    }

    if (!_explosionForceGenerator->getIsActive()) return;

    for (int i = 0; i < _particlesPerFrame; ++i) {
        Particula* newParticle = generateP();
        if (newParticle) {
            newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
            newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * float(t));
            newParticle->setTimeOfLife(1.0f);
            ApplyForces(newParticle, t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada
            newParticle->integrate_Verlet(t);

            _generatorParticlesV.push_back(newParticle);
        }
    }
    
}

//void PaintParticleGenerator::ApplyForces(Particula* newParticle, double t)
//{
//    newParticle->setAcc(Vector3(0, 0, 0));
//    float massParticle = newParticle->getMass();
//    for (int i = 0; i < _forceGenerators.size(); ++i) {
//        Vector3 newForce = _forceGenerators[i]->putForce(newParticle);
//        if (massParticle != 0.0f) newParticle->setAcc(newParticle->getAcc() + newForce / massParticle);
//    }
//}

void PaintParticleGenerator::triggerExplosion(Vector3 pos, Vector4 color, std::vector<ForceGenerator*> forceGeneratorsGlobal)
{
    if (_explosionForceGenerator && !_explosionForceGenerator->getIsActive()) {
        _modelP->setColor(color);
        setPos(pos); //Ponemos el generador en la posicion pasada por referencia que es la posicion del proyectil

        Defense* newDefense = new Defense(pos,Vector3(0.f,0.f,0.f),color,6.0f);
        if (newDefense) {
            newDefense->setPos(pos);
            for (ForceGenerator* FG : forceGeneratorsGlobal) {
                newDefense->addForceGenerator(FG);
            }
            _defenders.push_back(newDefense);
        }

        _explosionForceGenerator->activate(pos);
    }
}

void PaintParticleGenerator::unpaint()
{
    if (_defenders.empty()) return;
    for (Defense* defense : _defenders) {
        delete defense;
        defense = nullptr;
    }
    _defenders.clear();
}
