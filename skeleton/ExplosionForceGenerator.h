#pragma once

#include "Particula.h"
#include "DynamicParticle.h"
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
private:
	Vector3 _center; //Centro de la explosion
	float _K; //Intensidad de la explosion
	float _tau; //Tiempo necesario para que la explosion se desactive
	float _R; //radio maximo de accion
	double _time; //tiempo transcurrido desde que ha empezado la explosion
	bool _active;
public:
	ExplosionForceGenerator(Vector3 center, float K, float tau, float R);
	void activate(Vector3 newCenter);
	void deactivate();
	void update(double dt) override;
	Vector3 putForce(Particula* p) override;
	Vector3 putForce(DynamicParticle* p) override;

	bool getIsActive() const { return _active; }

	ForceGenerator* clone() const;
};