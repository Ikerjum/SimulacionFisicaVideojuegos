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
    WaterParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame);
    ~WaterParticleGenerator();
    Particula* generateP() override;
    void update(double t) override;
    
    //void ApplyForces(Particula* newParticle, double t);
};

