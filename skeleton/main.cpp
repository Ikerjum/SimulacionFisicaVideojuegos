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
#include "Particula.h"
#include "Projectile.h"
#include "WaterParticleGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionParticleGenerator.h"
#include "PaintParticleGenerator.h"
#include "WindForceGenerator.h"
#include "Ground.h"
#include "ParticleSystem.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyBounceGenerator.h"

std::string display_text = "This is a test";


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

std::vector<Particula*> particulas(10,nullptr);
std::vector<Projectile*> proyectiles(1,nullptr);

Axes* _axes = nullptr;
Ground* _GroundDown = nullptr;
Ground* _GroundUp = nullptr;

std::vector<ForceGenerator*> _forceGeneratorsGlobal;

Particula* particleModelWater = nullptr;
WaterParticleGenerator* WaterGenerator = nullptr;

Particula* particleModelExplosion = nullptr;
ExplosionParticleGenerator* ExplosionGenerator = nullptr;


PaintParticleGenerator* PaintGenerator = nullptr;

GravityForceGenerator* GravityDownGenerator = nullptr;
WindForceGenerator* WindUpGenerator = nullptr;

SpringForceGenerator* SpringUpGenerator = nullptr;
AnchoredSpringFG* AnchoredSpringUpGenerator = nullptr;

ParticleSystem* WaterSystem = nullptr;

static constexpr PxReal TAM_PROJECTILE = 3;

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
	particulas[0] = new Particula(velParticle,posParticle,accParticle,massParticle,color,tamParticle,timeOfLifeParticle);
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
		particulas[0] = new Particula(velParticle, posParticle, accParticle, massParticle ,color1, tamParticle, timeOfLifeParticle );
		particulas[1] = new Particula(velParticle, posParticle, accParticle, massParticle ,color2, tamParticle, timeOfLifeParticle );
		particulas[2] = new Particula(velParticle, posParticle, accParticle, massParticle ,color3, tamParticle, timeOfLifeParticle );
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
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//_axes = new Axes(); //Creamos ejes para posicionarnos en el espacio
	//_GroundDown = new Ground(PxVec3(0.0f,0.0f,0.0f)); //Creamos suelo abajjo
	//_GroundUp = new Ground(PxVec3(0.0f,60.0f,0.0f)); //Creamos suelo arriba
	_GroundDown = new Ground(gPhysics, gScene, PxVec3(0.0f, 0.0f, 0.0f));
	_GroundUp = new Ground(gPhysics, gScene, PxVec3(0.0f, 60.0f, 0.0f));
	//CreateParticles();

	//MODELO DE LA PARTICULA DE AGUA DE LLUVIA
	Vector3 velModelWater = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 accModelWater = Vector3(0.0f, 0.0f, 0.0f); //La aceleracion se gestiona el generador de fuerzas
	Vector4 colorModelWater = Vector4(0.0f, 0.3f, 1.0f, 1.0f);
	PxReal tamModelWater = 0.3f;
	float timeOfLifeModelWater = 5.0f;
	float massModelWater = 20.0f;
	Vector3 generatorPosWater = Vector3(0.0f, _GroundUp->getPos().y, 0.0f);
	particleModelWater = new Particula(velModelWater, generatorPosWater, accModelWater, massModelWater, colorModelWater, tamModelWater, timeOfLifeModelWater);

	//MODELO DE LA PARTICULA DE PINTURA
	Vector3 velModelExplosion = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 accModelExplosion = Vector3(0.0f, 0.0f, 0.0f);
	Vector4 colorModelExplosion = Vector4(1.0f, 0.3f, 0.0f, 1.0f);
	PxReal tamModelExplosion = 0.6f;
	float timeOfLifeModelExplosion = 100.0f;
	float massModelExplosion = 100.0f;
	Vector3 generatorPosExplosion = Vector3(0.0f, 0.0f, 0.0f);
	particleModelExplosion = new Particula(velModelExplosion, generatorPosExplosion, accModelExplosion, massModelExplosion, colorModelExplosion, tamModelExplosion, timeOfLifeModelExplosion);


	//GENERADORES
	WindUpGenerator = new WindForceGenerator(Vector3(0.0f, 6000.0f, 0.0f),false);
	_forceGeneratorsGlobal.push_back(WindUpGenerator);
	GravityDownGenerator = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
	_forceGeneratorsGlobal.push_back(GravityDownGenerator);

	WaterGenerator = new WaterParticleGenerator(generatorPosWater,particleModelWater,2);
	WaterGenerator->addForceGenerator(GravityDownGenerator);
	WaterGenerator->addForceGenerator(WindUpGenerator);

	PaintGenerator = new PaintParticleGenerator(generatorPosExplosion, particleModelExplosion, 6);
	
	//SpringUpGenerator = new SpringForceGenerator();
	//AnchoredSpringUpGenerator = new AnchoredSpringFG(500.0, 0.5, _GroundDown->getPos());

	//WaterSystem = new ParticleSystem();
	//WaterSystem->addParticleGenerator(WaterGenerator);
}

void PaintInScene(Projectile* projectile) {
	if (PaintGenerator && projectile) {
		PaintGenerator->triggerExplosion(projectile->getPos().p - Vector3(0.0f, 0.5f, 0.0f), projectile->getProjectileColor(), _forceGeneratorsGlobal);
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

	//particulas[0]->integrate_EulerExplicit(t);
	//particulas[1]->integrate_EulerSemiImplicit(t);
	
	//for (int i = 0; i < particulas.size(); ++i) {
	//	if (particulas[i] != nullptr) {
	//		particulas[i]->integrate_EulerSemiImplicit(t);
	//	}
	//}
	
	for (int i = 0; i < proyectiles.size(); ++i) {

		if (proyectiles[i] != nullptr) {
			proyectiles[i]->update(t);

			if (//proyectiles[i]->getTimeOfLife() <= 0 || 
				proyectiles[i]->getPos().p.y + TAM_PROJECTILE >= _GroundUp->getPos().y || 
				proyectiles[i]->getPos().p.y - TAM_PROJECTILE <= _GroundDown->getPos().y) {

				PaintInScene(proyectiles[i]);
				delete proyectiles[i];
				proyectiles[i] = nullptr;

			}
		}

	}

	WaterGenerator->update(t);
	//ExplosionGenerator->update(t);
	PaintGenerator->update(t);

	std::this_thread::sleep_for(std::chrono::microseconds(10));
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

	if (_axes) {
		delete _axes;
		_axes = nullptr;
	}
	if (_GroundDown) {
		delete _GroundDown;
		_GroundDown = nullptr;
	}
	if (_GroundUp) {
		delete _GroundUp;
		_GroundUp = nullptr;
	}

	if (WaterSystem) {
		delete WaterSystem;
		WaterSystem = nullptr;
	}
	if (WaterGenerator) {
		delete WaterGenerator;
		WaterGenerator = nullptr;
	}

	if (ExplosionGenerator) {
		delete ExplosionGenerator;
		ExplosionGenerator = nullptr;
	}
	if (PaintGenerator) {
		delete PaintGenerator;
		PaintGenerator = nullptr;
	}

	if (SpringUpGenerator) {
		delete SpringUpGenerator;
		SpringUpGenerator = nullptr;
	}
	if (AnchoredSpringUpGenerator) {
		delete AnchoredSpringUpGenerator;
		AnchoredSpringUpGenerator = nullptr;
	}


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

void PaintInScene()
{
	if (PaintGenerator) {
		int i = 0;
		bool encontrado = false;
		while (i < proyectiles.size() && !encontrado) {
			if (proyectiles[i] != nullptr) {
				PaintGenerator->triggerExplosion(proyectiles[i]->getPos().p, proyectiles[i]->getProjectileColor(),_forceGeneratorsGlobal);
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

	bool newState = !WindUpGenerator->isActive();
	WindUpGenerator->setActive(newState);

	if (WaterGenerator) {
		if (WindUpGenerator->isActive()) {
			WaterGenerator->setPos(Vector3(WaterGenerator->getPos().p.x, _GroundDown->getPos().y, WaterGenerator->getPos().p.z));
		}
		else {
			WaterGenerator->setPos(Vector3(WaterGenerator->getPos().p.x, _GroundUp->getPos().y, WaterGenerator->getPos().p.z));
		}
	}
}

void UnPaintAllInScene()
{
	if (PaintGenerator) {
		PaintGenerator->unpaint(); //Borramos toda la pintura generada en la escena
	}
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Camera* cam = GetCamera();
	Vector3 posCam = cam->getEye();
	Vector3 dirCam = cam->getDir();

	//Vector3 velParticle = Vector3(dirCam.x * 70.0f, dirCam.y * 70.0f, dirCam.z * 70.0f);
	//Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	//Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	//float massParticle = 1.0f;
	//float timeOfLifeParticle = 10.0f;
	//bool rePosBullet = true;

	switch(toupper(key))
	{
		case '1': //DISPARAMOS PROYECTIL BLANCO
			ShootProjectile(Projectile::PAINT_WHITE, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case '2': //DISPARAMOS PROYECTIL NEGRO
			ShootProjectile(Projectile::PAINT_BLACK, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			break;
		case '3': //DISPARAMOS PROYECTIL ROJO
			ShootProjectile(Projectile::PAINT_RED, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			break;
		case '4': //DISPARAMOS PROYECTIL AZUL
			ShootProjectile(Projectile::PAINT_BLUE, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			break;
		case '5': //DISPARAMOS PROYECTIL VERDE
			ShootProjectile(Projectile::PAINT_GREEN, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			break; 
		case '6': //DISPARAMOS PROYECTIL AMARILLO
			ShootProjectile(Projectile::PAINT_YELLOW, Projectile::IntegratorType::VERLET, posCam, dirCam, TAM_PROJECTILE);
			PaintGenerator->setColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
			break;
		case ' ': //PINTAMOS SI HAY UN PROYECTIL DISPARADO Y SI NO ESTA SIENDO PINTADO OTRO MIENTRAS TANTO
			PaintInScene();
			break;
		case 'B': //BORRAMOS TODO LO PINTADO
			UnPaintAllInScene();
			break;
		case 'V': //ACTIVAMOS Y DESACTIVAMOS LA FUERZA DEL VIENTO
			ManageWindForce();
			break;
		default:
			break;
	}

	////CANNON_BULLET
	//case 'Y':
	//	//for (int i = 0; i < particulas.size(); ++i) {
	//	//	if (particulas[i] == nullptr) {
	//	//		particulas[i] = new Particula(velParticle,posCam, accParticle, massParticle, color, timeOfLifeParticle);
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
	////TANK_BULLET
	//case 'U':
	//	ShootProjectile(Projectile::TANK_BULLET, Projectile::IntegratorType::VERLET, posCam, dirCam);
	//	break;
	////PISTOL
	//case 'I':
	//	ShootProjectile(Projectile::PISTOL, Projectile::IntegratorType::VERLET, posCam, dirCam);
	//	break;
	////LASER_PISTOL
	//case 'O':
	//	ShootProjectile(Projectile::LASER_PISTOL, Projectile::IntegratorType::VERLET, posCam, dirCam);
	//	break;
	////EXPLOSIVE_MINE
	//case 'P':
	//	ShootProjectile(Projectile::EXPLOSIVE_MINE, Projectile::IntegratorType::VERLET, posCam, dirCam);
	//	break;
	//-----------------------------------------------------------------------------------------------------

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