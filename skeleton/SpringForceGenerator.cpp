#include "SpringForceGenerator.h"
#include "checkML.h"
SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particula* other)
{
	_k = k;
	_resting_length = resting_length;
	_other = other;
}

Vector3 SpringForceGenerator::putForce(Particula* particle)
{
	Vector3 relative_pos_vector = _other->getPos().p - particle->getPos().p;
	Vector3 force;

	const float length = relative_pos_vector.normalize();
	const float delta_x = length - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	return force;
}
