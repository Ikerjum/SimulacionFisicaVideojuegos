#pragma once
#include "Particula.h"
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
	ParticleGenerator(Vector3 pos, Particula* modelP, int ParticlesPerFrame);
	virtual ~ParticleGenerator();

	//GENERAR PARTICULA (VIRTUAL)
	virtual Particula* generateP() = 0;
	//UPDATE DE PARTICULAS (VIRTUAL)
	virtual void update(double t) = 0;
	
	//PARTICULA MODELO
	void setModel(Particula* modelP);
	Particula* getModel() const { return _modelP; }
	
	//POSICION DEL GENERADOR
	void setPos(Vector3 pos);
	PxTransform getPos() const { return _pos; }

protected:
	PxTransform _pos;
	Particula* _modelP;
	int _particlesPerFrame;
	std::vector<Particula*> _generatorParticlesV;
	std::list<Particula*> _generatorParticlesL;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u;
	std::normal_distribution<double> _n;

	std::vector<ForceGenerator*> _forceGenerators;
};

