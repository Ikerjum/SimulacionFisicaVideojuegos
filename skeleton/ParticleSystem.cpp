#include "ParticleSystem.h"
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
