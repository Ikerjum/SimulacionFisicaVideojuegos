#include "Ground.h"

#include <PxPhysicsAPI.h>

using namespace physx;

Ground::Ground() {
	// ESFERA CENTRAL
	PxSphereGeometry _sphereCenter(1);
	//_sphere.radius = 0.5f;
	//PxMaterial* _sphereMaterial = gPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	PxShape* shapeCenter = CreateShape(_sphereCenter);
	//PxVec3 pos = PxVec3(0.0f, 0.0f, 0.0f);
	Vector3 posCenter = Vector3(0.0f, 0.0f, 0.0f);
	//PxQuat rot = PxQuat(0.0f, 0.0f, 0.0f, 0.0f);
	PxTransform* transformCenter = new PxTransform(posCenter);
	//transform->p = pos;
	//transform->q = rot;
	_groundItem = new RenderItem(shapeCenter, transformCenter,Vector4(0.0f,1.0f,0.0f,1.0f));
	RegisterRenderItem(_groundItem);
}

Ground::~Ground() {
	if (_groundItem) {
		_groundItem->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		_groundItem = nullptr;
	}
}
