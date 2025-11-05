#pragma once
#include "ParticleGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "GravityForceGenerator.h"
#include <random>
#include <iostream>

class PaintParticleGenerator :
    public ParticleGenerator
{
private:
    ExplosionForceGenerator* _explosionForceGenerator;
    std::vector<Particula*> _paintParticles;
public:
    PaintParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame);
    Particula* generateP() override;
    void update(double t) override;
    void ApplyForces(Particula* newParticle, double t);
    void triggerExplosion(Vector3 pos);
    void unpaint();
};