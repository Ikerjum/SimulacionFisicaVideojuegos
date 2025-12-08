#include "DynamicParticle.h"
#include "checkML.h"
#include <PxPhysicsAPI.h>
#include <iostream>
using namespace physx;

DynamicParticle::DynamicParticle()
    : _vel(0, 0, 0), _pos(Vector3(0, 0, 0)), _oldPos(_pos.p - _vel),
    _acc(0, 0, 0), _mass(1.0f), _color(0, 0, 0, 0), _tam(1), _timeOfLife(5.0f),
    _renderItem(nullptr), _shape(nullptr), _actor(nullptr)
{
}

DynamicParticle::DynamicParticle(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife, Vector3 dimensions, 
    Vector3 linearVelocity, Vector3 angularVelocity, PxScene* gScene, PxPhysics* gPhysics, Form form)
    : _vel(vel), _pos(pos), _oldPos(pos - vel * OLD_POS_CONSTANT), _acc(acc), _mass(mass), _color(color), _tam(tam), _timeOfLife(timeOfLife), _dimensions(dimensions),
    _linearVelocity(linearVelocity), _angularVelocity(angularVelocity), _gScene(gScene), _gPhysics(gPhysics)
{
    //_shape = CreateShape(PxSphereGeometry(tam));
    //_renderItem = new RenderItem(_shape, &_pos, _color); // Se registra al construir

    if (form == DynamicParticle::BOX) {
        _actor = gPhysics->createRigidDynamic(PxTransform(pos));
        _actor->setLinearVelocity(linearVelocity);
        _actor->setAngularVelocity(angularVelocity);
        _shape = CreateShape(PxBoxGeometry(dimensions));
        _actor->attachShape(*_shape);
        PxRigidBodyExt::updateMassAndInertia(*_actor, 0.15);
        gScene->addActor(*_actor);
    }
    else if (form == DynamicParticle::SPHERE) {
        _actor = gPhysics->createRigidDynamic(PxTransform(pos));
        _actor->setLinearVelocity(linearVelocity);
        _actor->setAngularVelocity(angularVelocity);
        _shape = CreateShape(PxSphereGeometry(tam));
        _actor->attachShape(*_shape);
        PxRigidBodyExt::updateMassAndInertia(*_actor, 0.15);
        gScene->addActor(*_actor);
    }

    _renderItem = new RenderItem(_shape, _actor, color);
}

DynamicParticle::DynamicParticle(const DynamicParticle& other)
{
    _vel = other._vel;
    _pos = other._pos;
    _oldPos = other._oldPos;
    _acc = other._acc;
    _mass = other._mass;
    _color = other._color;
    _tam = other._tam;
    _timeOfLife = other._timeOfLife;

    _shape = nullptr;
    _renderItem = nullptr;
    _actor = nullptr;
}

DynamicParticle& DynamicParticle::operator=(const DynamicParticle& other)
{
    if (this != &other)
    {
        _vel = other._vel;
        _pos = other._pos;
        _oldPos = other._oldPos;
        _acc = other._acc;
        _mass = other._mass;
        _color = other._color;
        _tam = other._tam;
        _timeOfLife = other._timeOfLife;

        // Liberamos memoria previa si existía
        if (_renderItem) {
            _renderItem->release();
            _renderItem = nullptr;
        }
        _shape = nullptr;
        if (_actor) {
            _gScene->removeActor(*_actor);
            _actor->release();
            _actor = nullptr;
        }
    }
    return *this;
}

DynamicParticle::~DynamicParticle()
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

void DynamicParticle::update(double t) {
    _timeOfLife -= static_cast<float>(t);
    if (_timeOfLife < 0.0f) _timeOfLife = 0.0f;
}

void DynamicParticle::integrate_EulerExplicit(double t)
{
    _pos.p = _pos.p + (_vel * t);
    _vel = _vel + (_acc * t);
    _vel = _vel * pow(DAMPING, t);
    update(t);
}

void DynamicParticle::integrate_EulerSemiImplicit(double t)
{
    _vel = _vel + (_acc * t);
    _vel = _vel * pow(DAMPING, t);
    _pos.p = _pos.p + (_vel * t);
    update(t);
}

void DynamicParticle::integrate_Verlet(double t)
{
    Vector3 temp = _pos.p;
    Vector3 nextPos = _pos.p + (_pos.p - _oldPos) * DAMPING + _acc * (t * t);
    _vel = (nextPos - _oldPos) / (2.0 * t);
    _oldPos = _pos.p;
    _pos.p = nextPos;
    update(t);
}

//DynamicParticle* DynamicParticle::clone(PxReal tam) const
//{
//    DynamicParticle* p = new DynamicParticle(*this);
//
//    if (tam == 0) {
//        p->_shape = CreateShape(PxSphereGeometry(p->_tam));
//    }
//    else {
//        p->_shape = CreateShape(PxSphereGeometry(tam));
//        p->_tam = tam;
//    }
//
//    if (p->_renderItem) {
//        p->_renderItem->release();
//        p->_renderItem = nullptr;
//    }
//
//    p->_renderItem = new RenderItem(p->_shape, &p->_pos, p->_color);
//
//    return p;
//}
