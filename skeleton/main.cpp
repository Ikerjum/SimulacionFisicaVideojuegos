#include <PxPhysicsAPI.h>
#include "checkML.h"
#include <ctype.h>

#include <vector>
#include <chrono>
#include <thread>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"
#include "Axes.h"
#include "Particle.h"
#include "Projectile.h"
#include "WaterParticleGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionParticleGenerator.h"
#include "DefenseParticleGenerator.h"
#include "Ground.h"
#include "ParticleSystem.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyBounceGenerator.h"
#include "EnemyManager.h"

//TUTORIAL DE CONTROLES
std::string display_text = "CONTROLES: TECLAS 1,2,3,4,5,6 => DISPARAR PROYECTIL DEFENSIVO / ESPACIO => INVOCAR DEFENSA / FLECHAS => VIENTO / B => QUITAR DEFENSAS / V => VIENTO VERTICAL";

//LA UI SE GESTIONA EN ENEMYMANAGER
std::string Puntuation = "POINTS: ";
std::string Lifes = "LIFES: ";
std::string Record = "RECORD: ";

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;
PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;
PxMaterial*				gMaterial	= NULL;
PxPvd*                  gPvd        = NULL;
PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//VECTOR DE PARTICULAS Y PROYECTILES
std::vector<Particle*> particulas(10,nullptr);
std::vector<Projectile*> proyectiles(1,nullptr); //SOLO LANZAMOS UN PROYECTIL A LA VEZ

//CREACION DEL SUELO / SOLIDOS RIGIDOS ESTATICOS
//Axes* _axes = nullptr;
Ground* _GroundDown = nullptr;
Vector3 _GroundUpPosition;

//GENERADORES DE FUERZAS GLOBALES, GRAVEDAD Y VIENTO (viento no aplicable a los enemigos)
std::vector<ForceGenerator*> _forceGeneratorsGlobal;

//SISTEMAS DE PARTICULAS Y SOLIDOS RIGIDOS DINAMICOS
ParticleSystem* WaterSystem = nullptr;
ParticleSystem* ExplosionSystem = nullptr;
DefenseParticleGenerator* DefenseGenerator = nullptr;
EnemyManager* EnemySystem = nullptr;

//FUERZAS GENERALES A APLICAR
GravityForceGenerator* GravityDownGenerator = nullptr;
WindForceGenerator* WindUpGenerator = nullptr;
Vector3 WindUpVelocityOriginal;
Vector3 WindUpVelocityActual;
PxReal WindUpVelocityHorizontal;

//EXTRA
//SpringForceGenerator* SpringUpGenerator = nullptr;
//AnchoredSpringFG* AnchoredSpringUpGenerator = nullptr;

static constexpr PxReal TAM_PROJECTILE = 3; //Tamaño del proyectil

void CreateParticle()
{
	//PARTICULA
	Vector3 posParticle = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 velParticle = Vector3(10.0f, 90.0f, 0.0f);
	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	float massParticle = 1.0f;
	Vector4 color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	PxReal tamParticle = 2;
	float timeOfLifeParticle = 10.0f;
	particulas[0] = new Particle(velParticle,posParticle,accParticle,massParticle,color,tamParticle,timeOfLifeParticle);
}

void CreateParticles() {
	for (int i = 0; i < 3; ++i) {
		//PARTICULA
		Vector3 posParticle = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 velParticle = Vector3(-165.0f, 0.0f, 0.0f);
		Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
		float massParticle = 1.0f;
		Vector4 color1 = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		Vector4 color2 = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4 color3 = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		PxReal tamParticle = 2;
		float timeOfLifeParticle = 10.0f;
		particulas[0] = new Particle(velParticle, posParticle, accParticle, massParticle ,color1, tamParticle, timeOfLifeParticle );
		particulas[1] = new Particle(velParticle, posParticle, accParticle, massParticle ,color2, tamParticle, timeOfLifeParticle );
		particulas[2] = new Particle(velParticle, posParticle, accParticle, massParticle ,color3, tamParticle, timeOfLifeParticle );
	}
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f); //IMPORTANTE: Añadimos a mano la gravedad con GravityDownGenerator, por control total de las fuerzas aplicadas en el simulador 
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);


	#pragma region SUELO
		//GENERADOR DE SOLIDOS ESTATICOS, LOS PATHS
		_GroundDown = new Ground(gPhysics, gScene, PxVec3(0.0f, 0.0f, 0.0f));
		_GroundUpPosition = PxVec3(0.0f, 100.0f, 0.0f);
	#pragma endregion
	
	#pragma region GENERADORES_DE_FUERZAS
		//GENERADORES DE FUERZAS GLOBALES
		//GRAVEDAD
		GravityDownGenerator = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
		_forceGeneratorsGlobal.push_back(GravityDownGenerator);
		//VIENTO
		WindUpVelocityOriginal = Vector3(0.0f, 4000.0f, 0.0f);
		WindUpVelocityActual = Vector3(0.0f, 0.0f, 0.0f);
		WindUpVelocityHorizontal = 2000.0f;
		WindUpGenerator = new WindForceGenerator(WindUpVelocityActual, true);
		_forceGeneratorsGlobal.push_back(WindUpGenerator);
	#pragma endregion
	
	#pragma region SISTEMAS_DE_PARTICULAS
		WaterSystem = new ParticleSystem(); //LLUVIA, PARA LOS 5 GENERADORES DE LLUVIA
		//MODELO DE LA PARTICULA DE AGUA DE LLUVIA
		WaterSystem->createModelParticle(
			Vector3(0.0f, 0.0f, 0.0f),					//velModelWater
			Vector3(0.0f, _GroundUpPosition.y, 0.0f),	//generatorPosWater
			Vector3(0.0f, 0.0f, 0.0f),					//accModelWater
			20.0f,										//massModelWater
			Vector4(0.0f, 0.3f, 1.0f, 1.0f),			//colorWaterModel
			0.3f,										//tamModelWater
			5.0f);										//timeOfLifeModelWater
	
		ExplosionSystem = new ParticleSystem(); //EXPLOSIONES, PARA DEFENSE_GENERATOR
		//MODELO DE LA PARTICULA DE PINTURA
		ExplosionSystem->createModelParticle(
			Vector3(0.0f, 0.0f, 0.0f), //velModelExplosion
			Vector3(0.0f, 0.0f, 0.0f), //generatorPosExplosion
			Vector3(0.0f, 0.0f, 0.0f), //accModelExplosion
			100.0f, //massModelExplosion
			Vector4(1.0f, 0.3f, 0.0f, 1.0f), //colorModelExplosion
			0.6f, //tamModelExplosion
			100.0f); //timeOfLifeModelExplosion
	
		//APLICANDO LOS 5 GENERADORES DE LLUVIA AL SISTEMA
		WaterParticleGenerator* WaterGenerator1 = new WaterParticleGenerator(Vector3(0.0f, _GroundUpPosition.y, 0.0f), WaterSystem->getParticleModel(), 1);
		WaterParticleGenerator* WaterGenerator2 = new WaterParticleGenerator(Vector3(120.0f, _GroundUpPosition.y, 120.0f), WaterSystem->getParticleModel(), 1);
		WaterParticleGenerator* WaterGenerator3 = new WaterParticleGenerator(Vector3(120.0f, _GroundUpPosition.y, -120.0f), WaterSystem->getParticleModel(), 1);
		WaterParticleGenerator* WaterGenerator4 = new WaterParticleGenerator(Vector3(-120.0f, _GroundUpPosition.y, 120.0f), WaterSystem->getParticleModel(), 1);
		WaterParticleGenerator* WaterGenerator5 = new WaterParticleGenerator(Vector3(-120.0f, _GroundUpPosition.y, -120.0f), WaterSystem->getParticleModel(), 1);
		WaterSystem->addParticleGenerator(WaterGenerator1, _forceGeneratorsGlobal);
		WaterSystem->addParticleGenerator(WaterGenerator2, _forceGeneratorsGlobal);
		WaterSystem->addParticleGenerator(WaterGenerator3, _forceGeneratorsGlobal);
		WaterSystem->addParticleGenerator(WaterGenerator4, _forceGeneratorsGlobal);
		WaterSystem->addParticleGenerator(WaterGenerator5, _forceGeneratorsGlobal);
	#pragma endregion
	
	#pragma region MECANISMOS ENEMIGOS/DEFENSAS
		//MECANISMO DE DEFENSA //GENERADOR DE PARTICULAS CON FLOTACION
		DefenseGenerator = new DefenseParticleGenerator(Vector3(0.0f,0.0f,0.0f), ExplosionSystem->getParticleModel(), 6, gPhysics, gScene);
		//MECANISMO DE ENEMIGOS //GENERADOR DE SOLIDOS RIGIDOS
		EnemySystem = new EnemyManager(gPhysics,gScene,_forceGeneratorsGlobal);
	#pragma endregion

	#pragma region EXTRAS
	//_axes = new Axes(); //Creamos ejes para posicionarnos en el espacio
	//CreateParticles();
	//SpringUpGenerator = new SpringForceGenerator();
	//AnchoredSpringUpGenerator = new AnchoredSpringFG(500.0, 0.5, _GroundDown->getPos());
	#pragma endregion
}

void PaintInScene(Projectile* projectile) {
	if (DefenseGenerator && projectile) {
		DefenseGenerator->triggerExplosion(projectile->getPos().p - Vector3(0.0f, 0.5f, 0.0f), projectile->getProjectileColor(), _forceGeneratorsGlobal);
	}
}

void SpecialKeysDown(int key, int x, int y) {
	if (WindUpGenerator) {
		switch (key) {
		case GLUT_KEY_UP:
				WindUpGenerator->setWindVel(
					Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z - WindUpVelocityHorizontal));
			break;
		case GLUT_KEY_DOWN:
				WindUpGenerator->setWindVel(
					Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z + WindUpVelocityHorizontal));
			break;
		case GLUT_KEY_LEFT:
				WindUpGenerator->setWindVel(
					Vector3(WindUpVelocityActual.x - WindUpVelocityHorizontal, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		case GLUT_KEY_RIGHT:
				WindUpGenerator->setWindVel(
					Vector3(WindUpVelocityActual.x + WindUpVelocityHorizontal, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		default:
			break;
		}
	}
}

void SpecialKeysUp(int key, int x, int y) {
	if (WindUpGenerator) {
		switch (key) {
		case GLUT_KEY_UP:
			WindUpGenerator->setWindVel(Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		case GLUT_KEY_DOWN:
			WindUpGenerator->setWindVel(Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		case GLUT_KEY_LEFT:
			WindUpGenerator->setWindVel(Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		case GLUT_KEY_RIGHT:
			WindUpGenerator->setWindVel(Vector3(WindUpVelocityActual.x, WindUpVelocityActual.y, WindUpVelocityActual.z));
			break;
		default:
			break;
		}
	}
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	
	//PROYECTILES
	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr) {
			proyectiles[i]->update(t);

			if (//proyectiles[i]->getTimeOfLife() <= 0 || 
				proyectiles[i]->getPos().p.y + TAM_PROJECTILE >= _GroundUpPosition.y || 
				proyectiles[i]->getPos().p.y - TAM_PROJECTILE <= _GroundDown->getPos().y) {

				PaintInScene(proyectiles[i]);
				delete proyectiles[i];
				proyectiles[i] = nullptr;

			}
		}
	}

	WaterSystem->update(t); //Actualizamos todos los generadores de lluvia
	ExplosionSystem->update(t); //Actualizamos los generadores de explosiones, no necesario ya que solo necesitamos la particula modelo para el generador de defensas
	DefenseGenerator->update(t); //Actualizamos todas las defensas, PARTICULAS CON FLOTACION
	EnemySystem->updateEnemyManager(t); //Actualizamos todos los generadores de enemigos, SOLIDOS RIGIDOS

	//FUNCIONES CALLBACK PARA TECLAS, FLECHAS DIRECCIONALES
	glutSpecialFunc(SpecialKeysDown);
	glutSpecialUpFunc(SpecialKeysUp);

	//TIMESTEP
	std::this_thread::sleep_for(std::chrono::microseconds(10));

	//PARTICULAS
	//particulas[0]->integrate_EulerExplicit(t);
	//particulas[1]->integrate_EulerSemiImplicit(t);
	//for (int i = 0; i < particulas.size(); ++i) {
	//	if (particulas[i] != nullptr) {
	//		particulas[i]->integrate_EulerSemiImplicit(t);
	//	}
	//}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	for (int i = 0; i < particulas.size(); ++i) {
		if (particulas[i] != nullptr) {
			delete particulas[i]; //Llamara a la destructora de particula
			particulas[i] = nullptr;
		}
	}
	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr) {
			delete proyectiles[i]; //Llamara a la destructora de particula
			proyectiles[i] = nullptr;
		}
	}
	for (ForceGenerator* FG : _forceGeneratorsGlobal) {
		delete FG;
		FG = nullptr;
	}
	if (_GroundDown) {
		delete _GroundDown;
		_GroundDown = nullptr;
	}
	if (WaterSystem) {
		delete WaterSystem;
		WaterSystem = nullptr;
	}
	if (ExplosionSystem) {
		delete ExplosionSystem;
		ExplosionSystem = nullptr;
	}
	if (DefenseGenerator) {
		delete DefenseGenerator;
		DefenseGenerator = nullptr;
	}
	if (EnemySystem) {
		delete EnemySystem;
		EnemySystem = nullptr;
	}

	//if (_axes) {
	//	delete _axes;
	//	_axes = nullptr;
	//}
	//if (SpringUpGenerator) {
	//	delete SpringUpGenerator;
	//	SpringUpGenerator = nullptr;
	//}
	//if (AnchoredSpringUpGenerator) {
	//	delete AnchoredSpringUpGenerator;
	//	AnchoredSpringUpGenerator = nullptr;
	//}
	
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	gFoundation->release();
}

//BUSQUEDA DEL SIGUIENTE PROYECTIL A LANZAR, AÑADIENDOLO AL VECTOR DE PROYECTILES Y APLICANDO SUS FUERZAS
void ShootProjectile(Projectile::ProjectileType type, Projectile::IntegratorType integrator, const Vector3& posCam, const Vector3& dirCam, PxReal tam)
{
	int indexToReuse = -1;

	//Al disparar un proyectil si encontramos un hueco que no haya sido ocupado creamos el proyectil
	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] == nullptr) {
			proyectiles[i] = new Projectile(posCam, dirCam, type, integrator,tam);
			for (ForceGenerator* FG : _forceGeneratorsGlobal) {
				proyectiles[i]->addForceGenerator(FG);
			}
			return;
		}
	}

	//Si encontramos un proyectil cuyo tiempo de vida sea menor que el limite, lo guardamos para reutilizarlo
	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr && proyectiles[i]->getTimeOfLife() <= 0.0f) {
			indexToReuse = i;
			break;
		}
	}

	//Si no hemos encontrado aun un proyectil a reutilizar, decidimos entre el que tiene la menor cantidad de tiempo de vida
	if (indexToReuse == -1) {

		int indexMinTime = -1;
		float minTime = 0.0f;

		for (int i = 0; i < proyectiles.size(); ++i) {
			if (proyectiles[i] != nullptr) {

				if (indexMinTime == -1) {
					minTime = proyectiles[i]->getTimeOfLife();
					indexMinTime = i;
				}
				else if (proyectiles[i]->getTimeOfLife() < minTime) {
					minTime = proyectiles[i]->getTimeOfLife();
					indexMinTime = i;
				}
			}
		}
		indexToReuse = indexMinTime;
	}

	//En caso de encontrar el proyectil, volvemos a aplicar sus fisicas
	if (indexToReuse != -1) {
		proyectiles[indexToReuse]->resetProperties(posCam, dirCam, type, integrator);
	}
}

//FUERZA DE EXPLOSION AL GENERAR UNA DEFENSA
void PaintInScene()
{
	if (DefenseGenerator) {
		int i = 0;
		bool encontrado = false;
		while (i < proyectiles.size() && !encontrado) {
			if (proyectiles[i] != nullptr) {
				DefenseGenerator->triggerExplosion(proyectiles[i]->getPos().p, proyectiles[i]->getProjectileColor(),_forceGeneratorsGlobal); //APLICAMOS UNA EXPLOSION EN EL SITIO DEL GENERADOR
				delete proyectiles[i];
				proyectiles[i] = nullptr;
				encontrado = true;
			}
			else i++;
		}
	}
}

void ManageWindForce()
{
	if (!WindUpGenerator) return;

	if (WindUpGenerator->getWindVel() == Vector3(0.0f, 0.0f, 0.0f)) { //SI NO HAY FUERZA DEL VIENTO APLICADA
		WindUpVelocityActual = WindUpVelocityOriginal;
		WindUpGenerator->setWindVel(WindUpVelocityActual);
		for (ParticleGenerator* PG : WaterSystem->getParticleGenerators()) {
			PG->setPos(Vector3(PG->getPos().p.x, _GroundDown->getPos().y, PG->getPos().p.z));
		}
	}
	else { //SI HAY FUERZA DEL VIENTO APLICADA LA DEVOLVEMOS A LA NORMALIDAD
		WindUpVelocityActual = Vector3(0.0f, 0.0f, 0.0f);
		WindUpGenerator->setWindVel(WindUpVelocityActual);
		for (ParticleGenerator* PG : WaterSystem->getParticleGenerators()) {
			PG->setPos(Vector3(PG->getPos().p.x, _GroundUpPosition.y, PG->getPos().p.z));
		}
	}
}

//BORRAMOS TODAS LAS DEFENSAS GENERADAS EN ESCENA
void ClearAllDefensesInScene()
{
	if (DefenseGenerator) {
		DefenseGenerator->unpaint();
	}
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Camera* cam = GetCamera();
	Vector3 posCam = cam->getEye();
	Vector3 dirCam = cam->getDir();

	switch(toupper(key))
	{
		case '1': //DISPARAMOS PROYECTIL BLANCO
			ShootProjectile(Projectile::PAINT_WHITE, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case '2': //DISPARAMOS PROYECTIL NEGRO
			ShootProjectile(Projectile::PAINT_BLACK, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			break;
		case '3': //DISPARAMOS PROYECTIL ROJO
			ShootProjectile(Projectile::PAINT_RED, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			break;
		case '4': //DISPARAMOS PROYECTIL AZUL
			ShootProjectile(Projectile::PAINT_BLUE, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			break;
		case '5': //DISPARAMOS PROYECTIL VERDE
			ShootProjectile(Projectile::PAINT_GREEN, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			break; 
		case '6': //DISPARAMOS PROYECTIL AMARILLO
			ShootProjectile(Projectile::PAINT_YELLOW, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			DefenseGenerator->setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
			break;
		case ' ': //PINTAMOS SI HAY UN PROYECTIL DISPARADO Y SI NO ESTA SIENDO PINTADO OTRO MIENTRAS TANTO
			PaintInScene();
			break;
		case 'B': //BORRAMOS TODAS LAS DEFENSAS GENERADAS EN ESCENA
			ClearAllDefensesInScene();
			break;
		case 'V': //ACTIVAMOS Y DESACTIVAMOS LA FUERZA DEL VIENTO VERTICAL
			ManageWindForce();
			break;
		default:
			break;
	}

	////CANNON_BULLET
	//case 'Y':
	//	//for (int i = 0; i < particulas.size(); ++i) {
	//	//	if (particulas[i] == nullptr) {
	//	//		particulas[i] = new Particle(velParticle,posCam, accParticle, massParticle, color, timeOfLifeParticle);
	//	//		rePosBullet = false;
	//	//		break;
	//	//	}
	//	//}
	//	//
	//	//if (rePosBullet) {
	//	//	int indexMaxTimeAlive = 0;
	//	//	for (int i = 0; i < particulas.size(); ++i) {
	//	//			if (particulas[i]->getTimeOfLife() > particulas[indexMaxTimeAlive]->getTimeOfLife()) indexMaxTimeAlive = i;
	//	//	}
	//	//	particulas[indexMaxTimeAlive]->setPos(posCam);
	//	//	particulas[indexMaxTimeAlive]->setVel(velParticle);
	//	//	particulas[indexMaxTimeAlive]->setAcc(accParticle);
	//	//	particulas[indexMaxTimeAlive]->setTimeOfLife(0);
	//	//}
	//	ShootProjectile(Projectile::CANNON_BULLET, Projectile::IntegratorType::VERLET, posCam, dirCam);
	//	break;
	//Vector3 velParticle = Vector3(dirCam.x * 70.0f, dirCam.y * 70.0f, dirCam.z * 70.0f);
	//Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	//Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	//float massParticle = 1.0f;
	//float timeOfLifeParticle = 10.0f;
	//bool rePosBullet = true;

}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(116);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}