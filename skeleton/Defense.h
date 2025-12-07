#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particula.h"
#include "ForceGenerator.h"
#include "BuoyancyBounceGenerator.h"

using namespace physx;


class Defense : public Particula
{
private:
    std::vector<ForceGenerator*> _forceGenerators;
    BuoyancyForceGenerator* _buoyancyForce;
public:
    Defense(Vector3 initialPos, Vector3 initialDir, Vector4 color, PxReal tam);
    ~Defense();

    void update(double t);

    void addForceGenerator(ForceGenerator* newForceGenerator);
    void ApplyForces(double t);
};