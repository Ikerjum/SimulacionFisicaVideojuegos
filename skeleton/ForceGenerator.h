#pragma once

#include "Particula.h"
#include "DynamicParticle.h"
#include "core.hpp"

class ForceGenerator {
public:
	enum typeGenerator {
		DEFAULT,
		GRAVITY,
		WIND,
		EXPLOSION,
		BUOYANCY,
		SPRING,
		OTHER
	};
private:
	Vector3 _ForceAcceleration;
	Vector3 _ForceVelocity;
	bool _isActive;
	typeGenerator _typeGenerator;
public:
	ForceGenerator() : _ForceAcceleration(), _ForceVelocity() { 
		_isActive = true; //Por defecto un generador de fuerzas esta activado
		setType(ForceGenerator::DEFAULT);
	};
	ForceGenerator(Vector3 acc) : _ForceAcceleration(acc), _ForceVelocity() {
		_isActive = true;
		setType(ForceGenerator::DEFAULT);
	};
	ForceGenerator(Vector3 acc, Vector3 vel) : _ForceAcceleration(acc), _ForceVelocity(vel) {
		_isActive = true;
		setType(ForceGenerator::DEFAULT);
	}; //Para el viento

	virtual ~ForceGenerator() {};
	virtual Vector3 putForce(Particula* p) = 0; //Aplicamos la fuerza a la particula dada
	virtual Vector3 putForce(DynamicParticle* p) = 0;
	virtual void update(double dt) {} //Actualizamos las fuerzas, como la explosion

	//GETTERS/SETTERS DE ACELERACION Y VELOCIDAD
	Vector3 getForceAcceleration() { return _ForceAcceleration; }
	Vector3 getForceVelocity() { return _ForceVelocity; }
	void setForceAcceleration(Vector3 acc) { _ForceAcceleration = acc; }
	void setForceVelocity(Vector3 vel) { _ForceVelocity = vel; }

	//ACTIVACION DEL GENERADOR DE FUERZAS
	bool isActive() const { return _isActive; }
	void setActive(bool active) { _isActive = active; }
	void toggleActive() { _isActive = !_isActive; }

	//GETTER DEL TIPO DE GENERADOR
	virtual void setType(typeGenerator type) { _typeGenerator = type; }
	typeGenerator getType() const { return _typeGenerator; }
};