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
    ExplosionParticleGenerator(Vector3 pos, Particle* model, int ParticlesPerFrame);
    ~ExplosionParticleGenerator();
    Particle* generateP() override;
    Particle* generatePInOnePosition(Vector3 pos);
    void update(double t) override;
    
    void ApplyForces(Particle* newParticle, double t);

    void triggerExplosion(Vector3 pos);
};

