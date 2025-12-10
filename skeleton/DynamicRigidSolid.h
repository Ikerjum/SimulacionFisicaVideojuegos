#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;


class DynamicRigidSolid
{
public:
    enum Form { BOX, SPHERE };

    DynamicRigidSolid(PxScene* gScene, PxPhysics* gPhysics);
    DynamicRigidSolid(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife, Vector3 dimensions,
        Vector3 linearVelocity, Vector3 angularVelocity, PxScene* gScene, PxPhysics* gPhysics, Form form);

    virtual ~DynamicRigidSolid();

    void update(double t);

    //SETTERS
    void setPos(Vector3 pos) { _pos.p = pos; }
    void setLinearVelocity(Vector3 linearVelocity) { _linearVelocity = linearVelocity; }
    void setAngularVelocity(Vector3 angularVelocity) { _angularVelocity = angularVelocity; }
    void setMass(float mass) { _mass = mass; }
    void setTam(PxReal tam) { _tam = tam; }
    void setColor(Vector4 color) {
        _color = color;
        if (_renderItem) _renderItem->color = color;
    }
    void setTimeOfLife(float time) { _timeOfLife = time; }
    void setDimensions(Vector3 dimensions) { _dimensions = dimensions; }

    //GETTERS
    PxTransform getPos() const { return _pos; }
    Vector3 getLinearVelocity() const { return _linearVelocity; }
    Vector3 getAngularVelocity() const { return _angularVelocity; }
    float getMass() const { return _mass; }
    PxReal getTam() const { return _tam; }
    Vector4 getColor() const { return _color; }
    float getTimeOfLife() const { return _timeOfLife; }
    Vector3 getDimensions() const { return _dimensions; }

    RenderItem* getRenderItem() const { return _renderItem; }

private:
    PxTransform _pos;
    Vector3 _linearVelocity;
    Vector3 _angularVelocity;
    float _mass;
    PxReal _tam;
    Vector4 _color;
    float _timeOfLife;
    Vector3 _dimensions;

    PxScene* _gScene;
    PxPhysics* _gPhysics;

protected:
    RenderItem* _renderItem;
    PxShape* _shape;
    PxRigidDynamic* _actor;
};