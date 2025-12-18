#include "EnemyGenerator.h"
#include <iostream>
#include <string>
#include <algorithm>

EnemyGenerator::EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene) :
	Particle(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), 0.0f, Vector4(0.0f,1.0,1.0f,1.0f),5,1000.f),
	_timer(0.0), _spawnEnemy(2.0), _pos(pos), _posGoal(posGoal), _gPhysics(gPhysics), _gScene(gScene), _winPoints(false), _loseLife(false),
	_u(-1.0, 1.0), _n(0.0, 1.0)
{
	std::random_device rd;
	_mt.seed(rd());
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

		float r = _n(_mt);

		EnemyType type;
		if (r < 0.33f) type = EnemyType::SMALL;
		else if (r < 0.66f) type = EnemyType::MEDIUM;
		else type = EnemyType::LARGE;

		Vector3 newTam;
		float newMass;
		Vector4 newColor;
		Vector3 newVelocity = Vector3(0.0f,0.0f,20.0f);

		switch (type)
		{
		case EnemyType::SMALL:
			newTam = Vector3(2.0f);
			newMass = 1.0f;   
			newColor = Vector4(0, 1, 0, 1); 
			break;

		case EnemyType::MEDIUM:
			newTam = Vector3(3.0f);
			newMass = 3.0f;
			newColor = Vector4(1, 1, 0, 1);
			break;

		case EnemyType::LARGE:
			newTam = Vector3(4.5f);
			newMass = 6.0f;
			newColor = Vector4(1, 0, 0, 1);
			break;
		}

		Enemy* newEnemy = new Enemy(_gPhysics, _gScene, _pos, newTam, newVelocity, Vector3(0.0f, 0.0f, 0.0f), newColor, newMass);
		for (ForceGenerator* fg : _forceGenerators) {
			if (fg->getType() == ForceGenerator::GRAVITY) {
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
