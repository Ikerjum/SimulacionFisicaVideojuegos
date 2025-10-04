#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"
#include "Particula.h"

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
Particula* particle = nullptr;
double dumping = 0.99;
std::vector<Particula*> particulas(10);


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

	//PARTICULA
	Vector3 posParticle = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 velParticle = Vector3(10.0f, 20.0f, 0.0f);
	float massParticle = 1.0f;
	float timeOfLifeParticle = 10.0f;
	particle = new Particula(velParticle,posParticle,massParticle,timeOfLifeParticle);

	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	particle->integrate(t,dumping,0.0f,-9.8f,0.0f);
	particle->setTimeOfLife(particle->getTimeOfLife() - t);
	if (particle->getTimeOfLife() < 0) particle->~Particula();
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
	particle->~Particula();
	gFoundation->release();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'p': {
		Camera* cam = GetCamera();
		Vector3 pos = cam->getEye();
		pos = cam->getDir();
		//particle->setPos(pos);
		//myScene.CreateScene(PARTICLES);
		break;
	}
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