#include "GeneradorP.h"
#include <PxPhysicsAPI.h>
#include <iostream>

GeneradorP::GeneradorP()
{
	std::uniform_real_distribution<double> u(_minProb, _maxProb);
	_u = u;
	//std::cout << _u(_generationRadiusTotal) << std::endl;

	//_modelPos = Vector3(0.0, 0.0, 0.0);
	//_modelVel = Vector3(0.0, 0.0, 0.0);
	//_modelDir = Vector3(0.0, 0.0, 0.0);


	//std::normal_distribution<double> d(_minProb, _maxProb);
	//_d = d;
}

GeneradorP::~GeneradorP()
{
	for (Particula* e : _listaP) {
		delete e;
		e = nullptr;
	}
}

Particula*
GeneradorP::CreateModelP(Vector3& pos, Vector3& vel, Vector3& dir, double& probGen)
{
	return _modelP;
}

std::list<Particula*> 
GeneradorP::GenerateP(Vector3& pos, Vector3& vel, Vector3& dir, double& probGen)
{
	//_listaP.clear(); //Hacemos una lista limpia nueva para pasar
	_modelP = CreateModelP(pos, vel, dir, probGen); //Creamos una nueva particula modelo con los parametros necesarios
	for (int i = 0; i < _nParticulas; ++i) {
		Vector3 posVar = _modelPos * _u(_generationRadiusTotal);
		Vector3 velVar = _modelVel * _u(_generationRadiusTotal);
		Vector3 dirVar = _modelDir * _u(_generationRadiusTotal);
		Particula* p = new Particula(posVar, velVar, dirVar, 1.0f, { 0.0f,0.0f,0.0f,1.0f });
		_listaP.push_back(p);
	}
	return _listaP;
}

void GeneradorP::clonarP()
{
}

//void GeneradorP::imprime()
//{
//	std::cout << _u(_generationRadiusTotal) << std::endl;
//}
