#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;


class Particula
{
public:
    static constexpr double DAMPING = 0.999; //Para el damping global en todas las particulas
    static constexpr float OLD_POS_CONSTANT = 0.01f; //Para calcular la anterior posicion de verlet

    Particula();
    Particula(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife);
    // Constructor de copia de datos simple (opcional, pero útil)
    Particula(const Particula& other);
    Particula& operator=(const Particula& other);

    ~Particula();
    void update(double t);
    void integrate_EulerExplicit(double t);
    void integrate_EulerSemiImplicit(double t);
    void integrate_Verlet(double t);

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

    PxTransform getPos() { return _pos; }
    Vector3 getOldPos() { return _oldPos; }
    Vector3 getVel() { return _vel; }
    Vector3 getAcc() { return _acc; }
    float getMass() { return _mass; }
    PxReal getTam() { return _tam; }
    Vector4 getColor() { return _color; }
    float getTimeOfLife() const { return _timeOfLife; }

    RenderItem* getRenderItem() { return _renderItem; }

    Particula* clone(PxReal tam = 0) const;

private:
    PxTransform _pos;
    Vector3 _oldPos;
    Vector3 _vel;
    Vector3 _acc;
    float _mass;
    Vector4 _color;
    PxReal _tam;
    float _timeOfLife;

protected:
    RenderItem* _renderItem;
    PxShape* _shape;
};