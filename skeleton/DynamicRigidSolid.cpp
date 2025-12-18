#include "DynamicRigidSolid.h"
#include "checkML.h"
#include <PxPhysicsAPI.h>
#include <iostream>
using namespace physx;

DynamicRigidSolid::DynamicRigidSolid(PxScene* gScene, PxPhysics* gPhysics)
    : _mass(1.0f), _color(0, 0, 0, 0), _tam(1), _timeOfLife(5.0f),_dimensions(Vector3(1.0f,1.0f,1.0f)),
    _linearVelocity(Vector3(0.0f,0.0f,0.0f)), _angularVelocity(Vector3(0.0f,0.0f,0.0f)),  _gScene(gScene), _gPhysics(gPhysics),
    _renderItem(nullptr), _shape(nullptr), _actor(nullptr)
{
}

DynamicRigidSolid::DynamicRigidSolid(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife, Vector3 dimensions, 
    Vector3 linearVelocity, Vector3 angularVelocity, PxScene* gScene, PxPhysics* gPhysics, Form form)
    : _mass(mass), _color(color), _tam(tam), _timeOfLife(timeOfLife), _dimensions(dimensions),
    _linearVelocity(linearVelocity), _angularVelocity(angularVelocity), _gScene(gScene), _gPhysics(gPhysics), _pos(pos)
{

    if (form == DynamicRigidSolid::BOX) {
        _shape = CreateShape(PxBoxGeometry(dimensions));
        _actor = gPhysics->createRigidDynamic(PxTransform(pos));
        _actor->attachShape(*_shape);
        _actor->setLinearVelocity(linearVelocity);
        _actor->setAngularVelocity(angularVelocity);
        //_actor->setMass(mass);
        PxRigidBodyExt::updateMassAndInertia(*_actor, mass);
        gScene->addActor(*_actor);
    }
    else if (form == DynamicRigidSolid::SPHERE) {
        _shape = CreateShape(PxSphereGeometry(tam));
        _actor = gPhysics->createRigidDynamic(PxTransform(pos));
        _actor->attachShape(*_shape);
        _actor->setLinearVelocity(linearVelocity);
        _actor->setAngularVelocity(angularVelocity);
        //_actor->setMass(mass);
        PxRigidBodyExt::updateMassAndInertia(*_actor, mass);
        gScene->addActor(*_actor);
    }

    //PxSphereGeometry sphereGeom(tam);
    //PxShape* shapeHead = CreateShape(sphereGeom);
    //_offsetHead = Vector3(0.0f, 15.0f, 0.0f);
    //_headTransform = PxTransform(initialPos + _offsetHead);
    //_head = new RenderItem(shapeHead, &_headTransform, color);

    //PxBoxGeometry boxGeom(scale);
    //PxShape* shape = CreateShape(boxGeom);
    //physx::PxRigidStatic* actor = gPhysics->createRigidStatic(PxTransform(pos + offset));
    //actor->attachShape(*shape);
    //gScene->addActor(*actor);
    //RenderItem* item = new RenderItem(shape, actor, color);
    ////RegisterRenderItem(item);

    _renderItem = new RenderItem(_shape, _actor, color);
}

DynamicRigidSolid::~DynamicRigidSolid()
{
    if (_renderItem) {
        _renderItem->release();
        _renderItem = nullptr;
    }
    if (_actor) {
        _gScene->removeActor(*_actor);
        _actor->release();
        _actor = nullptr;
    }
}

void DynamicRigidSolid::update(double t) {
    _timeOfLife -= static_cast<float>(t);
    if (_timeOfLife < 0.0f) _timeOfLife = 0.0f;
}