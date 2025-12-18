#pragma once
#include "ParticleGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "GravityForceGenerator.h"
#include "Defense.h"
#include <random>
#include <iostream>

class DefenseParticleGenerator :
    public ParticleGenerator
{
private:
    ExplosionForceGenerator* _explosionForceGenerator;
    std::vector<Defense*> _defenders;
    Vector4 _paintColor;

    PxPhysics* _gPhysics;
    PxScene* _gScene;
public:
    DefenseParticleGenerator(Vector3 pos, Particle* model, int ParticlesPerFrame, PxPhysics* _gPhysics, PxScene* _gScene);
    ~DefenseParticleGenerator();
    Particle* generateP() override;
    Particle* generateDefense();
    void update(double t) override;
    void triggerExplosion(Vector3 pos, Vector4 color, std::vector<ForceGenerator*> forceGenerators);
    void unpaint();

    //void ApplyForces(Particle* newParticle, double t);

    void setColor(Vector4 newColor) { _paintColor = newColor; }
    Vector4 getColor() const { return _paintColor; }
};