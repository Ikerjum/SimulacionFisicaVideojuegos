#include "Enemy.h"
#include "Vector3D.h"
#include "core.hpp"
#include <list>

class EnemyGenerator : public Particle {
private:
	std::vector<ForceGenerator*> _forceGenerators;
	std::list<Enemy*> _enemies;
	double _timer;
	double _spawnEnemy;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

	Vector3 _pos;
	Vector3 _posGoal;

	int _punt;
public:
	EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene);
	~EnemyGenerator();
	void updateEnemyGenerator(double t);
	void generateEnemy(double t);
	void updateEnemies(double t);
	void addForceGenerator(ForceGenerator* newForceGenerator);
};