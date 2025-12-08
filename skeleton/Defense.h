#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particula.h"
#include "ForceGenerator.h"
#include "BuoyancyBounceGenerator.h"
#include <vector>
#include <array>

class Defense : public Particula
{
private:
    std::vector<Particula*> _defenseParts;
    std::vector<ForceGenerator*> _forceGenerators;
    BuoyancyForceGenerator* _buoyancyForce;

    RenderItem* _body;
    PxTransform _bodyTransform;
    Vector3 _offsetBody;

    RenderItem* _head;
    PxTransform _headTransform;
    Vector3 _offsetHead;

    std::vector<RenderItem*> _littlePartsItem;
    std::vector<PxTransform> _littlePartsTransform;
    std::vector<Vector3> _littlePartsOffset;

public:
    Defense(Vector3 initialPos, Vector3 initialDir, Vector4 color, PxReal tam);
    ~Defense();

    void CreateLittlePart(physx::PxReal tam, Vector3& initialPos, Vector4& color, Vector3& offset);

    void update(double t);
    void updatePartOfDefense();

    void addForceGenerator(ForceGenerator* newForceGenerator);
    void ApplyForces(double t);
};