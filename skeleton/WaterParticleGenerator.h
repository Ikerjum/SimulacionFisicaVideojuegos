#pragma once
#include "ParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyBounceGenerator.h"
#include <random>
#include <iostream>

class WaterParticleGenerator :
    public ParticleGenerator
{
private:
    BuoyancyForceGenerator* _bounceGenerator;
public:
    WaterParticleGenerator(Vector3 pos, Particle* model, int ParticlesPerFrame);
    ~WaterParticleGenerator();
    Particle* generateP() override;
    void update(double t) override;
    
    //void ApplyForces(Particle* newParticle, double t);
};

