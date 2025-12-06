#include "ParticleGenerator.h"
#include "checkML.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, Particula* modelP, int ParticlesPerFrame) :
	_pos(PxTransform(pos)), _modelP(modelP), _particlesPerFrame(ParticlesPerFrame),
	_generatorParticlesV(), _generatorParticlesL(),
	_u(-1.0, 1.0), _n(0.0, 1.0) //Valores por defecto de la distribucion uniforme y la normal
{
}

ParticleGenerator::~ParticleGenerator()
{
	for (Particula* pV : _generatorParticlesV) {
		if (pV != nullptr) {
			delete pV;
			pV = nullptr;
		}
	}
	_generatorParticlesV.clear();

	for (Particula* pL : _generatorParticlesL) {
		if (pL != nullptr) {
			delete pL;
			pL = nullptr;
		}
	}
	_generatorParticlesL.clear();

	//FUERZAS GLOBALES,  LAS ADICIONALES SE BORRAN EN SUS CLASES HEREDADAS
	_forceGenerators.clear();

	if (_modelP) delete _modelP;
}

void ParticleGenerator::setModel(Particula* modelP)
{
	if (_modelP) delete _modelP;
	_modelP = modelP;
}

void ParticleGenerator::setPos(Vector3 pos) {
	_pos.p = pos;
}

void ParticleGenerator::addForceGenerator(ForceGenerator* newForceGenerator)
{
	_forceGenerators.push_back(newForceGenerator);
}

void ParticleGenerator::ApplyForces(Particula* newParticle, double t) {
	newParticle->setAcc(Vector3(0, 0, 0));
	float massParticle = newParticle->getMass();
	for (int i = 0; i < _forceGenerators.size(); ++i) {
		Vector3 newForce = _forceGenerators[i]->putForce(newParticle);
		if (massParticle != 0.0f) newParticle->setAcc(newParticle->getAcc() + newForce / massParticle);
	}
}