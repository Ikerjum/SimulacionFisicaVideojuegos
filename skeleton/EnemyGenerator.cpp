#include "EnemyGenerator.h"
#include <iostream>
#include <string>

EnemyGenerator::EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene) :
	Particle(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), 0.0f, Vector4(0.0f,1.0,1.0f,1.0f),5,1000.f),
	_timer(0.0), _spawnEnemy(1.0), _pos(pos), _posGoal(posGoal), _gPhysics(gPhysics), _gScene(gScene), _winPoints(false), _loseLife(false)
{
}

//HACER MOMENTOS DE INERCIA PARA LOS ENEMIGOS, ENEMIGOS DE DISTINTO PESO Y TAMAÑO

EnemyGenerator::~EnemyGenerator()
{
	for (Enemy* enemy : _enemies) {
		delete enemy;
		enemy = nullptr;
	}
	_enemies.clear();
}

void EnemyGenerator::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void EnemyGenerator::updateEnemyGenerator(double t)
{
	generateEnemy(t);
	updateEnemies(t);
}

void EnemyGenerator::generateEnemy(double t)
{
	_timer += t;
	if (_timer >= _spawnEnemy) {
		_timer = 0;
		Enemy* newEnemy = new Enemy(_gPhysics, _gScene, _pos, Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 0.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		for (ForceGenerator* fg : _forceGenerators) {
			if (fg->getType() == ForceGenerator::GRAVITY || fg->getType() == ForceGenerator::WIND) {
				newEnemy->addForceGenerator(fg);
			}
		}
		_enemies.push_back(newEnemy);
	}
}

void EnemyGenerator::updateEnemies(double t)
{
	if (!getWinPoints() && !getLoseLife()) {
		for (auto it = _enemies.begin(); it != _enemies.end(); ) {

			(*it)->updateEnemy(t);

			if ((*it)->getActor()->getGlobalPose().p.y <= 0) {
				delete (*it);
				it = _enemies.erase(it);
				setWinPoints(true);
				break;
			}
			else if ((*it)->getActor()->getGlobalPose().p.z >= _posGoal.z) {
				delete (*it);
				it = _enemies.erase(it);
				setLoseLife(true);
				break;
			}
			else {
				++it;
			}
		}
	}
}

void EnemyGenerator::resetEnemies()
{
	for (auto it = _enemies.begin(); it != _enemies.end(); ) {
		delete (*it);
		it = _enemies.erase(it);
	}
	_enemies.clear();
}
