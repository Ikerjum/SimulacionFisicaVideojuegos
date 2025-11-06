#pragma once
#include <vector>
#include "ForceGenerator.h"
#include "ParticleGenerator.h"
#include "WindForceGenerator.h"
class ParticleSystem
{
private:
	std::vector<ParticleGenerator*> _particleGenerators; //Conjunto de generadores de particulas
public:
	ParticleSystem();
	~ParticleSystem();
	void update(double t);
	void addParticleGenerator(ParticleGenerator* particleG);
	void addForce(ForceGenerator* forceG);
};

