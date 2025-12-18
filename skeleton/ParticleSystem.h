#pragma once
#include <vector>
#include "ForceGenerator.h"
#include "ParticleGenerator.h"
#include "WindForceGenerator.h"
#include "WaterParticleGenerator.h"
#include "Particle.h"

class ParticleSystem
{
private:
	std::vector<ParticleGenerator*> _particleGenerators; //Conjunto de generadores de particulas
	Particle* particleModel;
public:
	ParticleSystem();
	~ParticleSystem();

	void createModelParticle(Vector3 velModel, Vector3 generatorPos, Vector3 accModel, float massModel, Vector4 colorModel, PxReal tamModel, float timeOfLifeModel);
	void update(double t);
	void addParticleGenerator(ParticleGenerator* particleG, std::vector<ForceGenerator*>& forceGenerators);

	std::vector<ParticleGenerator*> getParticleGenerators() { return _particleGenerators; }
	Particle* getParticleModel() const { return particleModel; }
};

