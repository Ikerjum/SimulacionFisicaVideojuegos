#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;

class Particula
{
public:
	Particula(Vector3 vel, Vector3 pos, float mass, float timeOfLife);
	~Particula();
	void integrate(double t, double& dumping, float acc_x, float acc_y, float acc_z);
	void integrate_semiimp(double t, double& dumping, float acc_x, float acc_y, float acc_z);
	//PxTransform setPos(PxTransform newPos) { _pos = &newPos; }

	float getTimeOfLife() { return _timeOfLife; }
	void setTimeOfLife(float time) { _timeOfLife = time; }
private:
	Vector3 _vel;
	Vector3 _acc;
	float _mass;
	PxTransform* _pos;
	RenderItem* _renderItem;
	float _timeOfLife;
};

