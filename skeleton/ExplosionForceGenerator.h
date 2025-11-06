#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
private:
	Vector3 _center; //Centro de la explosion
	float _K; //Intensidad de la explosion
	float _tau; //Constante de decaimiento temporal
	float _R; //radio maximo de accion
	double _time; //tiempo transcurrido desde que ha empezado la explosion
	bool _active;
public:
	ExplosionForceGenerator(Vector3 center, float K, float tau, float R) 
		: ForceGenerator(), _center(center), _K(K), _tau(tau), _R(R), _time(0.0f), _active(false) 
	{
		setActive(true); //Por defecto siempre empezamos con el generador de explosiones activado
	}
	void activate(Vector3 newCenter) {
		_center = newCenter;
		_time = 0.0f;
		_active = true;
	}
	void deactivate() { _active = false; }
	void update(double dt) override { 
		if (_active) {
			_time += dt; 
			if (_time > _tau) {
				deactivate();
				_time = 0.0f;
			}
		}
	}
	Vector3 putForce(Particula* p) override;

	bool getIsActive() const { return _active; }
};