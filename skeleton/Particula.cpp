#include "Particula.h"
#include <PxPhysicsAPI.h>
#include <iostream>

using namespace physx;

Particula::Particula() : _vel(Vector3(0,0,0)), _pos(Vector3(0,0,0)), _acc(Vector3(0,0,0)), _mass(1.0f), _color(Vector4(0, 0, 0, 0))
{
}

Particula::Particula(Vector3 vel, Vector3 pos, Vector3 acc, float mass, Vector4 color)
{
	//CREATIONOFGEOMETRY
	PxSphereGeometry _sphereParticle(2);
	PxShape* shapeParticle = CreateShape(_sphereParticle);
	//TRANSFORM
	_vel = vel;
	_pos = PxTransform(pos);
	_oldPos = pos - vel; 
	_acc = acc;
	_mass = mass;
	_color = color;
	_timeOfLife = 20.f;
	//RENDERITEM
	_renderItem = new RenderItem(shapeParticle,&_pos,_color);
	//REGISTER
	RegisterRenderItem(_renderItem);
	//_renderItem = nullptr;
}

Particula::~Particula()
{
	DeregisterRenderItem(_renderItem);
}

void
Particula::update() {
	_timeOfLife--;
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
void Particula::integrate_EulerExplicit(double t, double damping)
{
	_pos.p = _pos.p + (_vel * t);
	_vel = _vel + (t * _acc);
	_vel = _vel * pow(damping, t); //Correccion de velocidad con dumping
	
	update();
}

//Actualizamos primero la velocidad y se usa la misma velocidad para calcular la posicion
void Particula::integrate_EulerSemiImplicit(double t, double damping)
{
	_vel = _vel + (t * _acc);
	_vel = _vel * pow(damping, t); //Correccion de velocidad con dumping
	_pos.p = _pos.p + (_vel * t);
	
	update();
}

void Particula::integrate_Verlet(double t, double damping)
{
	Vector3 temp = _pos.p;  // guardar posición actual (será la nueva "anterior")
	_pos.p = _pos.p + (_pos.p - _oldPos) * pow(damping, t) + _acc * (t * t);
	_oldPos = temp;  // actualizar la posición anterior
	
	update();
}