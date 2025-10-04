#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;

class Particula
{
public:
	Particula(Vector3 vel, Vector3 pos, Vector3 acc,float mass, Vector4 color,float timeOfLife);
	~Particula();
	void integrate_EulerExplicit(double t, double damping);
	void integrate_EulerSemiImplicit(double t, double damping);
	void integrate_Verlet(double t, double damping);

	float getTimeOfLife() { return _timeOfLife; }
	void incrementTimeOfLife(float time) { _timeOfLife += time; }
	void setTimeOfLife(float time) { _timeOfLife = time; }
	void setPos(Vector3 pos) { _pos->p = pos; }
	void setVel(Vector3 vel) { _vel = vel; }
	void setAcc(Vector3 acc) { _acc = acc; }
private:
	Vector3 _vel;
	Vector3 _acc;
	Vector3 _oldPos;
	Vector4 _color;
	float _mass;
	PxTransform* _pos;
	RenderItem* _renderItem;
	float _timeOfLife;
};

