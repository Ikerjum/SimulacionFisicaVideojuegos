#include "AnchoredSpringFG.h"
#include "checkML.h"
AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) :
	SpringForceGenerator(k,resting,nullptr)
{
	_other = new Particula(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1.0f, Vector4(1.0, 1.0, 1.0, 1.0), 2.0, 5.0);
}
