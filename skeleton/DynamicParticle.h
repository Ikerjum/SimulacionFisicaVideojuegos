#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;


class DynamicParticle
{
public:
    enum Form { BOX, SPHERE};

    static constexpr double DAMPING = 0.999; //Para el damping global en todas las particulas
    static constexpr float OLD_POS_CONSTANT = 0.01f; //Para calcular la anterior posicion de verlet

    DynamicParticle();
    DynamicParticle(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife, Vector3 dimensions,
        Vector3 linearVelocity, Vector3 angularVelocity, PxScene* gScene, PxPhysics* gPhysics, Form form);
    DynamicParticle(const DynamicParticle& other);
    DynamicParticle& operator=(const DynamicParticle& other);

    virtual ~DynamicParticle();

    void update(double t);
    void integrate_EulerExplicit(double t);
    void integrate_EulerSemiImplicit(double t);
    void integrate_Verlet(double t);

    //SETTERS
    void setPos(Vector3 pos) { _pos.p = pos; }
    void setOldPos(Vector3 pos) { _oldPos = pos; }
    void setVel(Vector3 vel) { _vel = vel; }
    void setAcc(Vector3 acc) { _acc = acc; }
    void setMass(float mass) { _mass = mass; }
    void setTam(PxReal tam) { _tam = tam; }
    void setColor(Vector4 color) {
        _color = color;
        if (_renderItem) _renderItem->color = color;
    }
    void setTimeOfLife(float time) { _timeOfLife = time; }

    //GETTERS
    PxTransform getPos() const { return _pos; }
    Vector3 getOldPos() const { return _oldPos; }
    Vector3 getVel() const { return _vel; }
    Vector3 getAcc() const { return _acc; }
    float getMass() const { return _mass; }
    PxReal getTam() const { return _tam; }
    Vector4 getColor() const { return _color; }
    float getTimeOfLife() const { return _timeOfLife; }

    RenderItem* getRenderItem() const { return _renderItem; }

    //DynamicParticle* clone(PxReal tam = 0) const;

private:
    PxTransform _pos;
    Vector3 _oldPos;
    Vector3 _vel;
    Vector3 _acc;
    float _mass;
    Vector4 _color;
    PxReal _tam;
    float _timeOfLife;
    Vector3 _dimensions;

    Vector3 _linearVelocity;
    Vector3 _angularVelocity;

    PxScene* _gScene;
    PxPhysics* _gPhysics;

protected:
    RenderItem* _renderItem;
    PxShape* _shape;
    PxRigidDynamic* _actor;
};