#pragma once
#include "ParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include <random>
#include <iostream>

class ExplosionParticleGenerator :
    public ParticleGenerator
{
private:
    ExplosionForceGenerator* _explosionForceGenerator = nullptr;
public:
    ExplosionParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame);
    Particula* generateP() override;
    void update(double t) override;
    void ApplyForces(Particula* newParticle, double t);
    void triggerExplosion(Vector3 pos);
};

