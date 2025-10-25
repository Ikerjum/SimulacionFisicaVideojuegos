#pragma once
#include "ParticleGenerator.h"
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

        float RANGO_VEL_X = 0.5f;
        float RANGO_VEL_Y = 0.0f;
        float RANGO_VEL_Z = 0.5f;

        Vector3 newVel = baseVel + Vector3(
            _n(_mt) * RANGO_VEL_X,
            _n(_mt) * RANGO_VEL_Y,
            _n(_mt) * RANGO_VEL_Z
        );

        newP->setVel(newVel);
        //Adaptacion de la posicion antigua para verlet
        newP->setOldPos(newP->getPos().p - newVel * Particula::OLD_POS_CONSTANT);

        return newP;
    }

    void update(double t) override {

        for (int i = 0; i < _particlesPerFrame; ++i) {
            Particula* newParticle = generateP();
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
};

