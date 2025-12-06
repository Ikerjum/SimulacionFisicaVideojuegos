#include "Particula.h"
#include "checkML.h"

#ifdef _DEBUG
#define new DBG_NEW
#endif
#include <PxPhysicsAPI.h>
#include <iostream>
#include "Defense.h"
using namespace physx;

Particula::Particula() : _vel(Vector3(0, 0, 0)), _pos(Vector3(0, 0, 0)), _oldPos(Vector3(_pos.p - _vel)), _acc(Vector3(0, 0, 0)), _mass(1.0f),
_color(Vector4(0, 0, 0, 0)), _tam(1), _timeOfLife(5.0f), _renderItem(nullptr)
{
}

Particula::Particula(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color, PxReal tam, float timeOfLife)
{
	//CREATIONOFGEOMETRY
	PxSphereGeometry _sphereParticle(tam);
	_shape = CreateShape(_sphereParticle);

	//TRANSFORM
	_vel = vel;
	_pos = PxTransform(pos);
	_oldPos = pos - vel * OLD_POS_CONSTANT;
	_acc = acc;
	_mass = mass;
	_tam = tam;
	_color = color;
	_timeOfLife = timeOfLife;

	//RENDERITEM
	_renderItem = new RenderItem(_shape, &_pos, _color); //REGISTER EN LA CONSTRUCTORA

}

Particula::Particula(const Particula& other)
{
	_vel = other._vel;
	_pos = other._pos;
	_oldPos = other._pos.p - other._vel * OLD_POS_CONSTANT;
	_acc = other._acc;
	_mass = other._mass;
	_color = other._color;
	_tam = other._tam;
	_timeOfLife = other._timeOfLife;
	_renderItem = nullptr;
	_shape = nullptr;
}

Particula& Particula::operator=(const Particula& other)
{
	if (this != &other)
	{
		_vel = other._vel;
		_pos = other._pos;
		_oldPos = other._pos.p - other._vel * OLD_POS_CONSTANT;
		_acc = other._acc;
		_mass = other._mass;
		_color = other._color;
		_tam = other._tam;
		_timeOfLife = other._timeOfLife;
		_renderItem = nullptr;
		_shape = nullptr;
	}
	return *this;
}

Particula::~Particula()
{
	if (_renderItem) {
		_renderItem->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		_renderItem = nullptr;
	}
	_shape = nullptr; //En el release del renderItem ya se hace release del shape
}

void
Particula::update(double t) {
	_timeOfLife -= (float)t;
	if (_timeOfLife < 0.0f) {
		_timeOfLife = 0.0f;
	}
}

/*
	Xi : posicion en el instante t
	Vi : posicion en el instante t
	Ai : aceleracion en el instante ti (derivada de la velocidad, viene de fuerzas/masa)
	h/t : tiempo transcurrido entre el momento actual y el anterior
	d (dumping): factor de correccion para contrarrestrar la acumulacion de energía numerica

	EULER SEMI-IMPLICITO
	Vi+1 = Vi + h * Ai
	Xi+1 = Xi + h * Vi

	EULER EXPLICITO
	Xi+1 = Xi + h * Vi
	Vi+1 = Vi + h * Ai

	Con damping, actualizamos la velocidad que ya calculamos para corregir
	Vi+1 = Vi+1 * d^Δt Asegurandonos de que el dumping va de 0 a 1 en los parametros a pasar
	Si es 0 siendo el dumping total la velocidad se va reduciendo a 0 siempre por lo que habria que estar aplicando fuerzas constantemente
	Si es 1 siendo el dumping nulo la velocidad es constante como si no tuviera rozamiento

	VERLET
	Xi+1 = 2*Xi - Xi-1 + h^2 * Ai (necesitamos la posicion anterior, que guardamos en la constructora y despues la guardamos para actualizarla al integrar con Verlet

*/

//Actualizamos primero la posicion con la velocidad vieja y luego la velocidad, barato computacionalmente pero inestable y con errores acumulativos
void Particula::integrate_EulerExplicit(double t)
{
	_pos.p = _pos.p + (_vel * t);
	_vel = _vel + (t * _acc);
	_vel = _vel * pow(Particula::DAMPING, t); //Correccion de velocidad con dumping

	update(t);
}

//Actualizamos primero la velocidad y se usa la misma velocidad para calcular la posicion
void Particula::integrate_EulerSemiImplicit(double t)
{
	_vel = _vel + (t * _acc);
	_vel = _vel * pow(Particula::DAMPING, t); //Correccion de velocidad con dumping
	_pos.p = _pos.p + (_vel * t);

	update(t);
}

void Particula::integrate_Verlet(double t)
{
	Vector3 temp = _pos.p;  // guardar posición actual (será la nueva "anterior")
	_pos.p = _pos.p + (_pos.p - _oldPos) * pow(Particula::DAMPING, t) + _acc * (t * t);
	_vel = (_pos.p - _oldPos) / (2.0 * t);
	_oldPos = temp;  // actualizar la posición anterior

	update(t);
}

Particula* Particula::clone(PxReal tam) const
{
	Particula* p = new Particula(*this);

	if (tam == 0) {
		PxSphereGeometry _sphereParticle(p->_tam);
		p->_shape = CreateShape(_sphereParticle);
	}
	else {
		PxSphereGeometry _sphereParticle(tam);
		p->_shape = CreateShape(_sphereParticle);
	}
	p->_renderItem = new RenderItem(p->_shape, &p->_pos, p->_color);

	return p;
}
