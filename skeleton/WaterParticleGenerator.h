#pragma once
#include "ParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include <random>

class WaterParticleGenerator :
    public ParticleGenerator
{
public:
    WaterParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame) : ParticleGenerator(pos,model,ParticlesPerFrame)
    {
        std::random_device rd;
        _mt.seed(rd());
    }
    Particula* generateP() override {

        //SE PUEDEN REDEFINIR LAS CONSTANTES PARA ESTE GENERADOR DE PARTICULAS
        //_u = std::uniform_real_distribution<double>(-1.0, 1.0);
        //_n = std::normal_distribution<double>(-1.0, 1.0);

        //GENERADORES DE FUERZAS
        _forceGenerators.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0))); //Aplicamos la gravedad al generador de fuerzas
        //_forceGenerators.push_back(new WindForceGenerator(Vector3(5.0, 0, 5.0)));

        Particula* newP = _modelP->clone();

        //VARIACION DE POSICION
        Vector3 basePos = _modelP->getPos().p;
        //float RANGO_POS_X = 2.0f;
        //float RANGO_POS_Y = 2.0f;
        //float RANGO_POS_Z = 2.0f;
        //
        //Vector3 newPos = basePos + Vector3(
        //    _n(_mt) * RANGO_POS_X,
        //    _n(_mt) * RANGO_POS_Y,
        //    _n(_mt) * RANGO_POS_Z
        //);
        //
        //newP->setPos(newPos);
        newP->setPos(basePos);
         
        //VARIACION DE VELOCIDAD
        Vector3 baseVel = _modelP->getVel();

        float RANGO_VEL_X = 20.0f;
        float RANGO_VEL_Y = 0.0f;
        float RANGO_VEL_Z = 20.0f;

        Vector3 newVel = baseVel + Vector3(
            _n(_mt) * RANGO_VEL_X,
            _n(_mt) * RANGO_VEL_Y,
            _n(_mt) * RANGO_VEL_Z
        );

        newP->setVel(newVel);
        //Adaptacion de la posicion antigua para verlet
        newP->setOldPos(newP->getPos().p - newVel * Particula::OLD_POS_CONSTANT);

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

            ApplyForces(newParticle); //Recorremos los generadores de fuerzas y aplicamos la aceleracion necesitada

            if (newParticle) {
                _generatorParticlesV.push_back(newParticle);
            }
        }

        for (int i = 0; i < _generatorParticlesV.size(); ) {
            Particula* p = _generatorParticlesV[i];
            p->integrate_Verlet(t);

            if (p->getTimeOfLife() <= 0.0f || p->getPos().p.y < -10.0f) {
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
    void ApplyForces(Particula* newParticle)
    {
        for (int i = 0; i < _forceGenerators.size(); ++i) {
            _forceGenerators[i]->putAcceleration(newParticle);
        }

    }
};

