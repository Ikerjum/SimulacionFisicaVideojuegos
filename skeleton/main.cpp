#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"
#include "Particula.h"
#include "Projectile.h"

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
double dumping = 0.99;
std::vector<Particula*> particulas(10);
std::vector<Projectile*> proyectiles(10);

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
	Vector4 color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	float massParticle = 1.0f;
	float timeOfLifeParticle = 10.0f;
	particulas[0] = new Particula(velParticle,posParticle,accParticle,massParticle,color);
}

void CreateParticles() {
	for (int i = 0; i < 3; ++i) {
		//PARTICULA
		Vector3 posParticle = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 velParticle = Vector3(-165.0f, 0.0f, 0.0f);
		Vector3 accParticle = Vector3(0.0f, -9.8f, 0.0f);
		Vector4 color1 = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		Vector4 color2 = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4 color3 = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		float massParticle = 1.0f;
		float timeOfLifeParticle = 10.0f;
		particulas[0] = new Particula(velParticle, posParticle, accParticle, massParticle ,color1 );
		particulas[1] = new Particula(velParticle, posParticle, accParticle, massParticle ,color2 );
		particulas[2] = new Particula(velParticle, posParticle, accParticle, massParticle ,color3 );
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

	}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	//particulas[0]->integrate_EulerExplicit(t, dumping);
	//particulas[1]->integrate_EulerSemiImplicit(t, dumping);
	
	//for (int i = 0; i < particulas.size(); ++i) {
	//	if (particulas[i] != nullptr) {
	//		particulas[i]->integrate_EulerSemiImplicit(t, dumping);
	//		particulas[i]->incrementTimeOfLife(t);
	//	}
	//}

	for (int i = 0; i < proyectiles.size(); ++i) {
		if (proyectiles[i] != nullptr) {
			proyectiles[i]->update(t);
		}
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
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

	gFoundation->release();
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
	//case 'B': break;
	//case ' ':	break;
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
			for (int i = 0; i < proyectiles.size(); ++i) {
				if (proyectiles[i] == nullptr) {
					proyectiles[i] = new Projectile(posCam,dirCam,Projectile::ProjectileType::CANNON_BULLET,Projectile::IntegratorType::EULER_SEMIEXPLICIT);
					rePosBullet = false;
					break;
				}
			}
			if (rePosBullet) {
				int indexMaxTimeAlive = 0;
				for (int i = 0; i < proyectiles.size(); ++i) {
					if (proyectiles[i]->getTimeOfLife() > proyectiles[indexMaxTimeAlive]->getTimeOfLife()) {
						indexMaxTimeAlive = i;
					}
				}
				proyectiles[indexMaxTimeAlive]->resetPhysics(posCam,dirCam,Projectile::ProjectileType::CANNON_BULLET, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
			}
			break;
		//TANK_BULLET
		case 'T':
			for (int i = 0; i < proyectiles.size(); ++i) {
				if (proyectiles[i] == nullptr) {
					proyectiles[i] = new Projectile(posCam, dirCam, Projectile::ProjectileType::TANK_BULLET, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
					rePosBullet = false;
					break;
				}
			}
			if (rePosBullet) {
				int indexMaxTimeAlive = 0;
				for (int i = 0; i < proyectiles.size(); ++i) {
					if (proyectiles[i]->getTimeOfLife() > proyectiles[indexMaxTimeAlive]->getTimeOfLife()) {
						indexMaxTimeAlive = i;
					}
				}
				proyectiles[indexMaxTimeAlive]->resetPhysics(posCam, dirCam, Projectile::ProjectileType::TANK_BULLET, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
			}
			break;
		//PISTOL
		case 'P':
			for (int i = 0; i < proyectiles.size(); ++i) {
				if (proyectiles[i] == nullptr) {
					proyectiles[i] = new Projectile(posCam, dirCam, Projectile::ProjectileType::PISTOL, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
					rePosBullet = false;
					break;
				}
			}
			if (rePosBullet) {
				int indexMaxTimeAlive = 0;
				for (int i = 0; i < proyectiles.size(); ++i) {
					if (proyectiles[i]->getTimeOfLife() > proyectiles[indexMaxTimeAlive]->getTimeOfLife()) {
						indexMaxTimeAlive = i;
					}
				}
				proyectiles[indexMaxTimeAlive]->resetPhysics(posCam, dirCam, Projectile::ProjectileType::PISTOL, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
			}
			break;
		//LASER_PISTOL
		case 'L':
			for (int i = 0; i < proyectiles.size(); ++i) {
				if (proyectiles[i] == nullptr) {
					proyectiles[i] = new Projectile(posCam, dirCam, Projectile::ProjectileType::LASER_PISTOL, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
					rePosBullet = false;
					break;
				}
			}
			if (rePosBullet) {
				int indexMaxTimeAlive = 0;
				for (int i = 0; i < proyectiles.size(); ++i) {
					if (proyectiles[i]->getTimeOfLife() > proyectiles[indexMaxTimeAlive]->getTimeOfLife()) {
						indexMaxTimeAlive = i;
					}
				}
				proyectiles[indexMaxTimeAlive]->resetPhysics(posCam, dirCam, Projectile::ProjectileType::LASER_PISTOL, Projectile::IntegratorType::EULER_SEMIEXPLICIT);
			}
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