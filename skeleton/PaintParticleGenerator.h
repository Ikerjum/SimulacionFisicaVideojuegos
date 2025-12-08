#pragma once
#include "ParticleGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "GravityForceGenerator.h"
#include "Defense.h"
#include <random>
#include <iostream>

class PaintParticleGenerator :
    public ParticleGenerator
{
private:
    ExplosionForceGenerator* _explosionForceGenerator;
    std::vector<Defense*> _defenders;
    Vector4 _paintColor;

    PxPhysics* _gPhysics;
    PxScene* _gScene;
public:
    PaintParticleGenerator(Vector3 pos, Particula* model, int ParticlesPerFrame, PxPhysics* _gPhysics, PxScene* _gScene);
    ~PaintParticleGenerator();
    Particula* generateP() override;
    Particula* generateDefense();
    void update(double t) override;
    void triggerExplosion(Vector3 pos, Vector4 color, std::vector<ForceGenerator*> forceGenerators);
    void unpaint();

    //void ApplyForces(Particula* newParticle, double t);

    void setColor(Vector4 newColor) { _paintColor = newColor; }
    Vector4 getColor() const { return _paintColor; }
};