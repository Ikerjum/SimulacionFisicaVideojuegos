#include "Enemy.h"
#include "Vector3D.h"
#include "core.hpp"
#include <list>
#include <random>

enum class EnemyType {
	SMALL,
	MEDIUM,
	LARGE
};

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

	bool _winPoints;
	bool _loseLife;

	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u;
	std::normal_distribution<double> _n;

	PxTransform _goalTransform;
	RenderItem* _goal;
public:
	EnemyGenerator(Vector3 pos, Vector3 posGoal, PxPhysics* gPhysics, PxScene* gScene);
	~EnemyGenerator();
	void updateEnemyGenerator(double t);

	void generateEnemy(double t);
	void updateEnemies(double t);
	void resetEnemies();

	void addForceGenerator(ForceGenerator* newForceGenerator);

	Vector3 getPos() const { return _pos; }
	Vector3 getPosGoal() const { return _posGoal; }

	bool getWinPoints() const { return _winPoints; }
	void setWinPoints(bool set) { _winPoints = set; }

	bool getLoseLife() const { return _loseLife; }
	void setLoseLife(bool set) { _loseLife = set; }
};