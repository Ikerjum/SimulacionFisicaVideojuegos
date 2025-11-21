#pragma once
#include "ParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "AnchoredSpringFG.h"
#include <random>
#include <iostream>

class WaterParticleGenerator :
    public ParticleGenerator
{
private:
    WindForceGenerator* _windForceGenerator;
public:
    WaterParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame);
    ~WaterParticleGenerator();
    Particula* generateP() override;
    void update(double t) override;
    void ApplyForces(Particula* newParticle, double t);

    void addWindForce(WindForceGenerator* externalForceGenerator);
    WindForceGenerator* getWindForce() { return _windForceGenerator; }
};

