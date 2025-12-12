#include "Enemy.h"
#include "Vector3D.h"
#include "core.hpp"

class EnemyGenerator : public Particle {
private:
	std::vector<ForceGenerator*> _forceGenerators;
	std::vector<Enemy*> _enemies;
	double _timer;
	double _spawnEnemy;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

	Vector3 _pos;
	Vector3 _posGoal;
public:
	EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene);
	~EnemyGenerator();
	void spawnEnemy();
	void updateEnemyGenerator(double t);
	void addForceGenerator(ForceGenerator* newForceGenerator);
};