#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <chrono>
#include <thread>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"
#include "Particula.h"
#include "Projectile.h"
#include "WaterParticleGenerator.h"

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

RenderItem* AxeCenter = nullptr;
RenderItem* AxeX = nullptr;
RenderItem* AxeY = nullptr;
RenderItem* AxeZ = nullptr;

std::vector<Particula*> particulas(10);
std::vector<Projectile*> proyectiles(10);

WaterParticleGenerator* WaterGenerator;
std::vector<Particula*> GeneratorParticles;

void CreateAxes()
{
	//AXES
	Vector4 colorAxe = { 0.0f,0.0f,0.0f,1.0f };

	// ESFERA CENTRAL
	PxSphereGeometry _sphereCenter(2);
	//_sphere.radius = 0.5f;
	//PxMaterial* _sphereMaterial = gPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	PxShape* shapeCenter = CreateShape(_sphereCenter);
	//PxVec3 pos = PxVec3(0.0f, 0.0f, 0.0f);
	Vector3 posCenter = Vector3(0.0f, 0.0f, 0.0f);
	//PxQuat rot = PxQuat(0.0f, 0.0f, 0.0f, 0.0f);
	PxTransform* transformCenter = new PxTransform(posCenter);
	//transform->p = pos;
	//transform->q = rot;
	AxeCenter = new RenderItem(shapeCenter, transformCenter, colorAxe);
	RegisterRenderItem(AxeCenter);

	//ESFERA DERECHA
	PxSphereGeometry _sphereDch(2);
	PxShape* shapeDch = CreateShape(_sphereDch);
	Vector3 posDch = Vector3(10.0f, 0.0f, 0.0f);
	PxTransform* transformDch = new PxTransform(posDch);
	//Vector4 colorDch = { 0.3f,0.1f,0.5f,1.0f };
	AxeX = new RenderItem(shapeDch, transformDch, colorAxe);
	RegisterRenderItem(AxeX);

	//ESFERA IZQUIERDA
	PxSphereGeometry _sphereIzq(2);
	PxShape* shapeIzq = CreateShape(_sphereIzq);
	Vector3 posIzq = Vector3(0.0f, 0.0f, 10.0f);
	PxTransform* transformIzq = new PxTransform(posIzq);
	//Vector4 colorIzq = { 0.3f,0.1f,0.5f,1.0f };
	AxeY = new RenderItem(shapeIzq, transformIzq, colorAxe);
	RegisterRenderItem(AxeY);

	//ESFERA DERECHA
	PxSphereGeometry _sphereUp(2);
	PxShape* shapeUp = CreateShape(_sphereUp);
	Vector3 posUp = Vector3(0.0f, 10.0f, 0.0f);
	PxTransform* transformUp = new PxTransform(posUp);
	//Vector4 colorUp = { 0.3f,0.1f,0.5f,1.0f };
	AxeZ = new RenderItem(shapeUp, transformUp, colorAxe);
	RegisterRenderItem(AxeZ);
}

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
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	CreateAxes();
	//CreateParticles();


	Vector3 velModel = Vector3(0.0f, 8.0f, 0.0f);
	Vector3 accModel = Vector3(0.0f, -10.0f, 0.0f);
	Vector4 colorModel = Vector4(0.0f, 0.3f, 1.0f, 1.0f);
	PxReal tamModel = 0.5f;
	float timeOfLifeModel = 3.0f;
	float massModel = 0.1f;
	Vector3 generatorPos = Vector3(0.0f, 30.0f, 0.0f);
	Particula* particleModel = new Particula(velModel, generatorPos, accModel, massModel, colorModel, tamModel, timeOfLifeModel);

	// 2. CREA EL GENERADOR con el modelo
	// Posición del generador (la fuente)
	WaterGenerator = new WaterParticleGenerator(generatorPos,particleModel,1);


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
			if (proyectiles[i]->getTimeOfLife() <= 0) {
				delete proyectiles[i];
				proyectiles[i] = nullptr;
			}
		}
	}

	WaterGenerator->update(t);

	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	DeregisterRenderItem(AxeCenter);
	DeregisterRenderItem(AxeX);
	DeregisterRenderItem(AxeY);
	DeregisterRenderItem(AxeZ);

	//for (int i = 0; i < particulas.size(); ++i) {
	//	if (particulas[i] != nullptr) {
	//		delete proyectiles[i]; //Llamara a la destructora de particula
	//		proyectiles[i] = nullptr;
	//	}
	//}

	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr) {
			delete proyectiles[i]; //Llamara a la destructora de particula
			proyectiles[i] = nullptr;
		}
	}

	if (WaterGenerator) delete WaterGenerator;
	WaterGenerator = nullptr;


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

// Función auxiliar que pones antes de initPhysics o keyPress en main.cpp
void ShootProjectile(Projectile::ProjectileType type, Projectile::IntegratorType integrator, const Vector3& posCam, const Vector3& dirCam)
{
	int indexToReuse = -1;

	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] == nullptr) {
			proyectiles[i] = new Projectile(posCam, dirCam, type, integrator);
			return;
		}
	}

	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr && proyectiles[i]->getTimeOfLife() <= 0.0f) {
			indexToReuse = i;
			break;
		}
	}

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

	if (indexToReuse != -1) {
		proyectiles[indexToReuse]->resetPhysics(posCam, dirCam, type, integrator);
	}
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Camera* cam = GetCamera();
	Vector3 posCam = cam->getEye();
	Vector3 dirCam = cam->getDir();

	Vector3 velParticle = Vector3(dirCam.x * 70.0f, dirCam.y * 70.0f, dirCam.z * 70.0f);
	Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
	Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	float massParticle = 1.0f;
	float timeOfLifeParticle = 10.0f;

	bool rePosBullet = true;

	switch(toupper(key))
	{
		//CANNON_BULLET
		case 'C':
			//for (int i = 0; i < particulas.size(); ++i) {
			//	if (particulas[i] == nullptr) {
			//		particulas[i] = new Particula(velParticle,posCam, accParticle, massParticle, color, timeOfLifeParticle);
			//		rePosBullet = false;
			//		break;
			//	}
			//}
			//
			//if (rePosBullet) {
			//	int indexMaxTimeAlive = 0;
			//	for (int i = 0; i < particulas.size(); ++i) {
			//			if (particulas[i]->getTimeOfLife() > particulas[indexMaxTimeAlive]->getTimeOfLife()) indexMaxTimeAlive = i;
			//	}
			//	particulas[indexMaxTimeAlive]->setPos(posCam);
			//	particulas[indexMaxTimeAlive]->setVel(velParticle);
			//	particulas[indexMaxTimeAlive]->setAcc(accParticle);
			//	particulas[indexMaxTimeAlive]->setTimeOfLife(0);
			//}
			ShootProjectile(Projectile::CANNON_BULLET, Projectile::IntegratorType::VERLET, posCam, dirCam);
			break;
		//TANK_BULLET
		case 'T':
			ShootProjectile(Projectile::TANK_BULLET, Projectile::IntegratorType::VERLET, posCam, dirCam);
			break;
		//PISTOL
		case 'P':
			ShootProjectile(Projectile::PISTOL, Projectile::IntegratorType::VERLET, posCam, dirCam);
			break;
		//LASER_PISTOL
		case 'L':
			ShootProjectile(Projectile::LASER_PISTOL, Projectile::IntegratorType::VERLET, posCam, dirCam);
			break;
		default:
			break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
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

	return 0;
}