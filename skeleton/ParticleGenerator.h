#pragma once
#include "Particle.h"
#include <random>
#include <vector>
#include <list>
#include "ForceGenerator.h"

using namespace physx;
using namespace std;

class ParticleGenerator
{
public:

	//APLICAMOS LA PARTICULA MODELO EN LA CONSTRUCTORA Y LA POSICION DEL GENERADOR
	ParticleGenerator(Vector3 pos, Particle* modelP, int ParticlesPerFrame);
	virtual ~ParticleGenerator();

	//GENERAR PARTICULA (VIRTUAL)
	virtual Particle* generateP() = 0;
	//UPDATE DE PARTICULAS (VIRTUAL)
	virtual void update(double t) = 0;
	
	//PARTICULA MODELO
	void setModel(Particle* modelP);
	Particle* getModel() const { return _modelP; }
	
	//POSICION DEL GENERADOR
	void setPos(Vector3 pos);
	PxTransform getPos() const { return _pos; }

	void addForceGenerator(ForceGenerator* newForceGenerator);
	virtual void ApplyForces(Particle* newParticle, double t);

protected:
	PxTransform _pos;
	Particle* _modelP;
	int _particlesPerFrame;
	std::vector<Particle*> _generatorParticlesV;
	std::list<Particle*> _generatorParticlesL;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u;
	std::normal_distribution<double> _n;

	std::vector<ForceGenerator*> _forceGenerators;
};

