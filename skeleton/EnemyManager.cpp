#include "EnemyManager.h"
#include "EnemyGenerator.h"

EnemyManager::EnemyManager(PxPhysics* gPhysics, PxScene* gScene, std::vector<ForceGenerator*> const& _forceGenerators) :
	_punt(0), _lifes(3), _record(0)
{
	//VARIABLES DE LA UI
	Puntuation = "PUNTUACION: " + std::to_string(_punt);
	Lifes = "LIFES: " + std::to_string(_lifes);
	Record = "RECORD: " + std::to_string(_record);

	//GENERADORES DE ENEMIGOS, SOLO TENEMOS DOS
	_enemyGenerators[0] = new EnemyGenerator(Vector3(50.0f, 20.0f, -190.0f), Vector3(50.0f, 20.0f, 190.0f), gPhysics, gScene);
	for (auto& f : _forceGenerators) {
		if (f->getType() == ForceGenerator::GRAVITY) _enemyGenerators[0]->addForceGenerator(f); //AÑADIMOS SOLO GRAVEDAD, NO QUEREMOS VIENTO
	}
	_enemyGenerators[1] = new EnemyGenerator(Vector3(-50.0f, 20.0f, -190.0f), Vector3(-50.0f, 20.0f, 190.0f), gPhysics, gScene);
	for (auto& f : _forceGenerators) {
		if (f->getType() == ForceGenerator::GRAVITY) _enemyGenerators[1]->addForceGenerator(f); //AÑADIMOS SOLO GRAVEDAD, NO QUEREMOS VIENTO
	}
}

EnemyManager::~EnemyManager()
{
	for (int i = 0; i < _enemyGenerators.size(); ++i) {
		delete _enemyGenerators[i];
		_enemyGenerators[i] = nullptr;
	}
}

void EnemyManager::updateEnemyManager(double t)
{
	for (int i = 0; i < _enemyGenerators.size(); ++i) {
		_enemyGenerators[i]->updateEnemyGenerator(t);

		if (_enemyGenerators[i]->getWinPoints()) { //EN CASO DE GANAR PUNTOS
			_punt += 100;
			Puntuation = "POINTS: " + std::to_string(_punt);
			_enemyGenerators[i]->setWinPoints(false);
			break;
		}

		if (_enemyGenerators[i]->getLoseLife()) { //EN CASO DE PERDER UNA VIDA (POSIBILIDAD DE DERROTA)
			
			if (_lifes > 0) {
				_lifes--; //QUITAMOS UNA VIDA EN CASO DE QUE UN ENEMIGO LLEGUE A LA META
			}
			else {

				for (int i = 0; i < _enemyGenerators.size(); ++i) {
					_enemyGenerators[i]->resetEnemies(); //RESETEAMOS LOS GENERADORES QUITANDO TODOS LOS ENEMIGOS
				}

				if (_punt > _record) {
					_record = _punt; //ACTUALIZAMOS RECORD SI LA PUNTUACION ES MAYOR
					Record = "RECORD: " + std::to_string(_record);
				}
				_punt = 0;
				Puntuation = "POINTS: " + std::to_string(_punt); //RESETEAMOS PUNTUACION A 0
				_lifes = 3; //PONEMOS VIDAS A 3
			}
			Lifes = "LIFES: " + std::to_string(_lifes);
			_enemyGenerators[i]->setLoseLife(false);
			break;

		}
	}
}
