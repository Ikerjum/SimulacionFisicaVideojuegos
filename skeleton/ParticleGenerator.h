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
	ParticleGenerator(Vector3 pos, Particula* modelP, int ParticlesPerFrame) : 
		_pos(PxTransform(pos)), _modelP(modelP), _particlesPerFrame(ParticlesPerFrame), 
		_generatorParticlesV(), _generatorParticlesL(),
		_u(0.0,1.0), _n(0.0,1.0) //Valores por defecto de la distribucion uniforme y la normal
	{
	}

	virtual ~ParticleGenerator() { 
		for (Particula* p : _generatorParticlesV) {
			if (p != nullptr) {
				delete p;
				p = nullptr;
			}
		}
		_generatorParticlesV.clear();
		if (_modelP) delete _modelP; 
	}

	//GENERAR PARTICULA (VIRTUAL)
	virtual Particula* generateP() = 0;
	//UPDATE DE PARTICULAS (VIRTUAL)
	virtual void update(double t) = 0;
	
	//PARTICULA MODELO
	void setModel(Particula* modelP) { 
		if (_modelP) delete _modelP;
		_modelP = modelP;
	}
	Particula* getModel() { return _modelP; }
	
	//POSICION DEL GENERADOR
	void setPos(Vector3 pos) { _pos.p = pos; }
	PxTransform getPos() { return _pos; }


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

