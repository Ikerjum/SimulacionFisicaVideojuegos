#include "SpringForceGenerator.h"
#include "checkML.h"

//FUERZA DE MUELLE
SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other)
{
	setActive(true);
	setType(ForceGenerator::SPRING);

	_k = k;
	_resting_length = resting_length;
	_other = other;
}

Vector3 SpringForceGenerator::putForce(Particle* particle)
{
	Vector3 relative_pos_vector = _other->getPos().p - particle->getPos().p;
	Vector3 force;

	const float length = relative_pos_vector.normalize();
	const float delta_x = length - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	return force;
}

Vector3 SpringForceGenerator::putForce(DynamicRigidSolid* particle)
{
	Vector3 relative_pos_vector = _other->getPos().p - particle->getPos().p;
	Vector3 force;

	const float length = relative_pos_vector.normalize();
	const float delta_x = length - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	return force;
}

inline void SpringForceGenerator::setK(double k)
{
	_k = k;
}

SpringForceGenerator::~SpringForceGenerator()
{
	if (_other) {
		delete _other;
		_other = nullptr;
	}
}

ForceGenerator*
SpringForceGenerator::clone() const
{
	return new SpringForceGenerator(*this);
}
