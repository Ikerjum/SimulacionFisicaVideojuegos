#include "GeneradorP.h"
#include "Particula.h"
#include <PxPhysicsAPI.h>
#include <iostream>

using namespace std;

GeneradorP::GeneradorP()
{
	//RANDOM
	std::random_device(rd);
	std::mt19937 mt(rd());
	_mt = mt;
	std::uniform_real_distribution<double> u(_minProb, _maxProb);
	_u = u;

	std::cout << _u(_mt) << std::endl;

	_modelP = new Particula();
	//_modelPos = Vector3(0.0, 0.0, 0.0)
	//_modelVel = Vector3(0.0, 0.0, 0.0);
	//_modelDir = Vector3(0.0, 0.0, 0.0);

	_nParticulas = 100;

	//std::normal_distribution<double> d(_minProb, _maxProb);
	//_d = d;
}

GeneradorP::~GeneradorP()
{
	for (Particula* e : _listaP) {
		delete e;
		e = nullptr;
	}
	delete _modelP;
	_modelP = nullptr;
}

Particula*
GeneradorP::CreateModelP(Vector3& vel, Vector3& pos, Vector3& dir, double& probGen)
{
	_modelVel = vel;
	_modelPos = pos;
	_modelDir = dir;
	_modelProbGen = probGen;

	_modelP->setPos(_modelPos);
	_modelP->setVel(_modelVel);
	_modelP->setAcc(_modelDir);
	return _modelP;
}

void
GeneradorP::GenerateP(Vector3& vel, Vector3& pos, Vector3& dir, double& probGen)
{
	//_listaP.clear(); //Hacemos una lista limpia nueva para pasar
	_modelP = CreateModelP(vel, pos, dir, probGen); //Creamos una nueva particula modelo con los parametros necesarios
	for (int i = 0; i < _nParticulas; ++i) {
		Vector3 velVar = _modelVel * _u(_mt);
		std::cout << "PosX: " << (_modelPos.x * _u(_mt)) << "\n";
		std::cout << "PosY: " << (_modelPos.y * _u(_mt)) << "\n";
		std::cout << "PosZ: " << (_modelPos.z * _u(_mt)) << "\n";
		Vector3 posVar = Vector3(_modelPos.x * _u(_mt),_modelPos.y, _modelPos.z * _u(_mt));
		Vector3 dirVar = _modelDir * _u(_mt);
		Particula* p = new Particula(velVar, posVar, dirVar, 1.0f, { 1.0f,1.0f,1.0f,1.0f },2,3.0f);
		p->setTimeOfLife(3.0f * _u(_mt));
		_listaP.push_back(p);
	}
}

Particula* 
GeneradorP::clonarP(Particula* particula)
{
	Particula* particulaClonada = particula;
	return particulaClonada;
}

void GeneradorP::updateParticles(double t)
{

		//it = _listaP.begin();
		//it++;

		for (list<Particula*>::iterator it = _listaP.begin(); it != _listaP.end(); it++) {
			(*it)->integrate_Verlet(t,0.99);
		}


		bool borrado = false;
		list<Particula*>::iterator it = _listaP.begin();
		
		//while (it != _listaP.end() && !borrado) {
		//	if ((*it)->getTimeOfLife() < 0) {
		//		std::cout << "BORRADO" << std::endl;
		//		delete (*it);
		//		_listaP.erase(it);
		//		break;
		//	}
		//	else it++;
		//}

		_listaP.remove_if([](Particula* p) {  
			if (p->getTimeOfLife() <= 0) {
			delete p;
			return true; 
			}
			return false;
		});

		//for (list<Particula*>::iterator it = _listaP.begin(); it != _listaP.end(); it++) {
		//	//(*it)->integrate_Verlet(t, 0.99);
		//	_listaP.erase(it);
		//	it++;
		//}


		//for (auto it : _listaP) {
		//	it->integrate_Verlet(t, 0.99);
		//	if (it->getTimeOfLife() < 0) _listaP.erase(it);
		//}
}

//void GeneradorP::imprime()
//{
//	std::cout << _u(_generationRadiusTotal) << std::endl;
//}
