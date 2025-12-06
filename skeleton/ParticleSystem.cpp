#include "ParticleSystem.h"
#include "checkML.h"

#ifdef _DEBUG
#define new DBG_NEW
#endif
#include "WaterParticleGenerator.h"
ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::update(double t) {
	for (int i = 0; i < _particleGenerators.size();++i) {
		if (_particleGenerators[i] != nullptr) {
			_particleGenerators[i]->update(t);
		}
	}
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* particleG) {
}

void ParticleSystem::addForce(ForceGenerator* forceG)
{
	for (int i = 0; i < _particleGenerators.size(); ++i) {
		if (_particleGenerators[i] != nullptr) {
			//_particleGenerators[i]->addForce(forceG);
		}
	}
}
