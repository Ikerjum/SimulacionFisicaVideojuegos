#include "BuoyancyBounceGenerator.h"
#include "checkML.h"
#include <algorithm>

//FUERZA DE FLOTACION
BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d, float waterHeight)
{
	setActive(true);
	setType(ForceGenerator::BUOYANCY);

	_height = h;
	_volume = V; //Unico valor modificable para que baje mas o baje menos
	_liquid_density = d; //1000 para el agua
	_waterHeight = waterHeight;
}

Vector3 BuoyancyForceGenerator::putForce(Particle* particle)
{
	float h = particle->getPos().p.y;
	float h0 = _waterHeight;

	Vector3 force(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}
	float buoyancy = _liquid_density * _volume * immersed * _gravity;
	float drag = -particle->getVel().y * 3.0f; //Resistencia para ir bajando la velociad
	force.y = buoyancy + drag;

	return force;
}

Vector3 BuoyancyForceGenerator::putForce(DynamicRigidSolid* particle)
{
	float h = particle->getPos().p.y;
	float h0 = _waterHeight;

	Vector3 force(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}
	float buoyancy = _liquid_density * _volume * immersed * _gravity;
	float drag = -particle->getLinearVelocity().y * 3.0f; //Resistencia para ir bajando la velociad
	force.y = buoyancy + drag;

	return force;
}

BuoyancyForceGenerator::~BuoyancyForceGenerator()
{
}

ForceGenerator* 
BuoyancyForceGenerator::clone() const {
	return new BuoyancyForceGenerator(*this);
}
