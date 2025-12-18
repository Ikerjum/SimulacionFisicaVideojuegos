#include "ParticleSystem.h"
#include "checkML.h"
#include "WaterParticleGenerator.h"
ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
	for (int i = 0; i < _particleGenerators.size();++i) {
		if (_particleGenerators[i] != nullptr) {
			delete _particleGenerators[i];
			_particleGenerators[i] = nullptr;
		}
	}
	_particleGenerators.clear();

	if (particleModel) {
		delete particleModel;
		particleModel = nullptr;
	}
}

void ParticleSystem::createModelParticle(Vector3 velModel, Vector3 generatorPos, Vector3 accModel, float massModel, Vector4 colorModel, PxReal tamModel, float timeOfLifeModel)
{
	particleModel = new Particle(velModel, generatorPos, accModel, massModel, colorModel, tamModel, timeOfLifeModel);
}

void ParticleSystem::update(double t) {
	for (int i = 0; i < _particleGenerators.size();++i) {
		if (_particleGenerators[i] != nullptr) {
			_particleGenerators[i]->update(t);
		}
	}
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* particleG, std::vector<ForceGenerator*>& forceGenerators)
{
	for (auto& f : forceGenerators) {
		particleG->addForceGenerator(f);
	}
	_particleGenerators.push_back(particleG);
}
