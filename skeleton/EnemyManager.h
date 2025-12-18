#pragma once

#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include <array>
#include <vector>

class EnemyGenerator;

//CLASE QUE CONTROLA LAS VIDAS, ENEMIGOS Y PUNTUACION DE LA PARTIDA
class EnemyManager {
private:
	int _punt;
	int _lifes;
	int _record;
	std::array<EnemyGenerator*, 2> _enemyGenerators;
public:
	EnemyManager(PxPhysics* gPhysics, PxScene* gScene, std::vector<ForceGenerator*> const& _forceGenerators);
	~EnemyManager();
	void updateEnemyManager(double t);
};