#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particula.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "BuoyancyBounceGenerator.h"
#include "WindForceGenerator.h"
#include <vector>
#include <array>
#include <list>
#include "Bullet.h"

class Defense : public Particula
{
private:
    std::vector<Particula*> _defenseParts;
    std::vector<ForceGenerator*> _forceGenerators;
    BuoyancyForceGenerator* _buoyancyForce;
    WindForceGenerator* _windForceGenerator;

    RenderItem* _body;
    PxTransform _bodyTransform;
    Vector3 _offsetBody;

    RenderItem* _head;
    PxTransform _headTransform;
    Vector3 _offsetHead;

    std::vector<RenderItem*> _littlePartsItem;
    std::vector<PxTransform> _littlePartsTransform;
    std::vector<Vector3> _littlePartsOffset;

    double _countToShoot;
    double _momentOfShoot;

    std::list<Bullet*> _bullets;

    physx::PxPhysics* _gPhysics;
    physx::PxScene* _gScene;

    Vector4 _color;

public:
    Defense(Vector3 initialPos, Vector3 initialDir, Vector4 color, PxReal tam, physx::PxPhysics* gPhysics, physx::PxScene* gScene);
    ~Defense();

    void CreateLittlePart(physx::PxReal tam, Vector3& initialPos, Vector4& color, Vector3& offset);

    void update(double t);
    void CheckWindForce();
    void updateBullets(double t);
    void GenerateBullet(double t);
    void updatePartOfDefense();

    void addForceGenerator(ForceGenerator* newForceGenerator);
    void initializeForceBounce();
    void ApplyForces(double t);
};