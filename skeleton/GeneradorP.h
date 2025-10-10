#pragma once
#include <random>
#include <list>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particula.h"
using namespace physx;
using namespace std;

class GeneradorP
{
private:
	const double _minProb = 0.0; //0.0
	const double _maxProb = 1.0;//1.0

	//PROBS
	double _generationRadiusTotal;
	
	int _nParticulas;
	list<Particula*> _listaP;

	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u;

	Particula* _modelP;
	Vector3 _modelPos;
	Vector3 _modelVel;
	Vector3 _modelDir;
	double _modelProbGen; //Dependera de la minProb y de la maxProb

	//std::normal_distribution<double> _d;
	//Vector3 _desviacionP;
	//Vector3 _desviacionV;
	//Vector3 _desviacionT;

public:
	GeneradorP();
	~GeneradorP();
	Particula* CreateModelP(Vector3& pos, Vector3& vel, Vector3& dir, double& probGen);
	virtual std::list<Particula*> GenerateP(Vector3& pos, Vector3& vel, Vector3& dir, double& probGen);
	void clonarP();

	//void imprime();
};

