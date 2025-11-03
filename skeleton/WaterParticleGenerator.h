#pragma once
#include "ParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include <random>
#include <iostream>

class WaterParticleGenerator :
    public ParticleGenerator
{
private:
    ExplosionForceGenerator* _explosionForceGenerator = nullptr;
public:
    WaterParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame) : ParticleGenerator(pos,model,ParticlesPerFrame)
    {
        std::random_device rd;
        _mt.seed(rd());

        //GENERADORES DE FUERZAS, LO HACEMOS SOLO UNA VEZ EN LA CONSTRUCTORA
        _forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas
        //_forceGenerators.push_back(new WindForceGenerator(Vector3(100.0, 0, 0.0),0.5f,0.02f));
        _explosionForceGenerator = new ExplosionForceGenerator(Vector3(0, 0, 0), 100000.0, 50.0f, 50.0f);
        _forceGenerators.push_back(_explosionForceGenerator);
    }
    Particula* generateP() override {

        //SE PUEDEN REDEFINIR LAS CONSTANTES PARA ESTE GENERADOR DE PARTICULAS
        //_u = std::uniform_real_distribution<double>(-1.0, 1.0);
        //_n = std::normal_distribution<double>(-1.0, 1.0);

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
        /*newP->setOldPos(newP->getPos().p - newVel * Particula::OLD_POS_CONSTANT);*/

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

    void update(double t) override {

        for (int i = 0; i < _particlesPerFrame; ++i) {
            Particula* newParticle = generateP();
            
            if (newParticle) {
                newParticle->setAcc(Vector3(0.0f, 0.0f, 0.0f));
                newParticle->setOldPos(newParticle->getPos().p - newParticle->getVel() * static_cast<float>(t));
                ApplyForces(newParticle,t); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada
                
                _generatorParticlesV.push_back(newParticle);
            }
        }

        for (int i = 0; i < _generatorParticlesV.size(); ) {
            Particula* p = _generatorParticlesV[i];
            
            ApplyForces(p,t); //Aplicamos fuerzas antes de integrar y recalculamos en cada frame
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
    void ApplyForces(Particula* newParticle,double t)
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
            }
        }
    }

    void triggerExplosion(Vector3 pos) {
        if (_explosionForceGenerator && !_explosionForceGenerator->getIsActive()) {
            _explosionForceGenerator->activate(pos);
        }
    }
};

