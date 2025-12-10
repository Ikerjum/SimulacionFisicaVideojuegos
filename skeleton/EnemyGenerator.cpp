#include "EnemyGenerator.h"

EnemyGenerator::EnemyGenerator(Vector3 pos, Vector3 posGoal) :
	Particle(Vector3(0, 0, 0), pos, Vector3(0, 0, 0), 0.0f, Vector4(0.0f,1.0,1.0f,1.0f),5,1000.f),
	_timer(0.0), _spawnEnemy(1.0), _posGoal(posGoal)
{

}

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
	_enemies.push_back(new Enemy(_gPhysics, _gScene, _pos, Vector3(10,10,10), Vector3(_posGoal - _pos), Vector3(0, 0, 0), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
}

void EnemyGenerator::updateEnemyGenerator(double t)
{
	_timer += t;
	if (_timer >= _spawnEnemy) {
		spawnEnemy();
		_timer = 0;
	}
}
