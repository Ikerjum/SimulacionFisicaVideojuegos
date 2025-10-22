#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;


class Particula
{
public:
	Particula();
	Particula(Vector3 vel, Vector3 pos, Vector3 acc,float mass, Vector4 color);
	
	Particula* operator=(Particula* copia) {
		Particula* p;
		p->_pos = copia->_pos;
		p->_vel = copia->_vel;
		p->_acc = copia->_acc;
		p->_mass = copia->_mass;
		p->_color = copia->_color;
		delete copia;
		return p;
	}

	~Particula();
	void update();
	void integrate_EulerExplicit(double t, double damping);
	void integrate_EulerSemiImplicit(double t, double damping);
	void integrate_Verlet(double t, double damping);

	float getTimeOfLife() { return _timeOfLife; }
	void incrementTimeOfLife(float time) { _timeOfLife += time; }
	void setTimeOfLife(float time) { _timeOfLife = time; }
	void setPos(Vector3 pos) { _pos.p = pos; }
	void setVel(Vector3 vel) { _vel = vel; }
	void setAcc(Vector3 acc) { _acc = acc; }
	void setColor(Vector4 color) { 
		_color = color; 
		if (_renderItem) _renderItem->color = color;
	}

	RenderItem* getRenderItem() { return _renderItem; }
	PxTransform getPos() { return _pos; }
	Vector4 getColor() { return _color; }
private:
	Vector3 _vel;
	Vector3 _acc;
	Vector3 _oldPos;
	Vector4 _color;
	float _mass;
	PxTransform _pos;
	float _timeOfLife;
protected:
	RenderItem* _renderItem;
};

