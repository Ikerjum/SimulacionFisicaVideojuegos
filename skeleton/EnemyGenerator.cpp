#include "EnemyGenerator.h"
#include <iostream>

EnemyGenerator::EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene) :
	Particle(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), 0.0f, Vector4(0.0f,1.0,1.0f,1.0f),5,1000.f),
	_timer(0.0), _spawnEnemy(1.0), _posGoal(posGoal), _gPhysics(gPhysics), _gScene(gScene)
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

void EnemyGenerator::spawnEnemy()
{
	Enemy* newEnemy = new Enemy(_gPhysics, _gScene, _pos, Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, _posGoal.z - _pos.z), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f,1.0f,0.0f,1.0f));
	for (ForceGenerator* fg : _forceGenerators) {
		if (fg->getType() == ForceGenerator::GRAVITY) {
			newEnemy->addForceGenerator(fg);
		}
	}
	_enemies.push_back(newEnemy);
	std::cout << "Enemigo generado\n";
}

void EnemyGenerator::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void EnemyGenerator::updateEnemyGenerator(double t)
{
	_timer += t;
	if (_timer >= _spawnEnemy) {
		spawnEnemy();
		_timer = 0;
	}

	//for (Enemy* enemy : _enemies) {
	//	enemy->update(t);
	//}

	//Particle::update(t);
}
