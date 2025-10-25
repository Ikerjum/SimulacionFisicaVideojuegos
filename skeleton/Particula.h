#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;


class Particula
{
public:
    Particula();
    Particula(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife);

    Particula* operator=(Particula* copia) {
        Particula* p;
        p->_vel = copia->_vel;
        p->_pos = copia->_pos;
        p->_oldPos = copia->_oldPos;
        p->_acc = copia->_acc;
        p->_mass = copia->_mass;
        p->_color = copia->_color;
        p->_tam = copia->_tam;
        p->_timeOfLife = copia->_timeOfLife;
        delete copia;
        return p;
    }

    ~Particula();
    void update(double t);
    void integrate_EulerExplicit(double t, double damping);
    void integrate_EulerSemiImplicit(double t, double damping);
    void integrate_Verlet(double t, double damping);

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
};