#include "Axes.h"
#include <PxPhysicsAPI.h>

using namespace physx;

Axes::Axes()
{
	// ESFERA CENTRAL
	PxSphereGeometry _sphereCenter(2);
	PxShape* shapeCenter = CreateShape(_sphereCenter);
	Vector3 posCenter = Vector3(0.0f, 0.0f, 0.0f);
	PxTransform* transformCenter = new PxTransform(posCenter);
	AxeCenter = new RenderItem(shapeCenter, transformCenter, Vector4(1.0f,1.0f,1.0f,1.0f));
	RegisterRenderItem(AxeCenter);

	//+X
	PxSphereGeometry _sphereXPos(2);
	PxShape* shapeXPos = CreateShape(_sphereXPos);
	Vector3 posXPos = Vector3(10.0f, 0.0f, 0.0f);
	PxTransform* transformXPos = new PxTransform(posXPos);
	AxeXPos = new RenderItem(shapeXPos, transformXPos, Vector4(1.0f,0.0f,0.0f,1.0f));
	RegisterRenderItem(AxeXPos);

	//-X
	PxSphereGeometry _sphereXNeg(2);
	PxShape* shapeXNeg = CreateShape(_sphereXNeg);
	Vector3 posXNeg = Vector3(-10.0f, 0.0f, 0.0f);
	PxTransform* transformXNeg = new PxTransform(posXNeg);
	AxeXNeg = new RenderItem(shapeXNeg, transformXNeg, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(AxeXNeg);

	//Y
	PxSphereGeometry _sphereYPos(2);
	PxShape* shapeYPos = CreateShape(_sphereYPos);
	Vector3 posYPos = Vector3(0.0f, 10.0f, 0.0f);
	PxTransform* transformYPos = new PxTransform(posYPos);
	AxeYPos = new RenderItem(shapeYPos, transformYPos, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(AxeYPos);

	//-Y
	PxSphereGeometry _sphereYNeg(2);
	PxShape* shapeYNeg = CreateShape(_sphereYNeg);
	Vector3 posYNeg = Vector3(0.0f, -10.0f, 0.0f);
	PxTransform* transformYNeg = new PxTransform(posYNeg);
	AxeYNeg = new RenderItem(shapeYNeg, transformYNeg, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(AxeYNeg);

	//Z
	PxSphereGeometry _sphereZPos(2);
	PxShape* shapeZPos = CreateShape(_sphereZPos);
	Vector3 posZPos = Vector3(0.0f, 0.0f, 10.0f);
	PxTransform* transformZPos = new PxTransform(posZPos);
	AxeZPos = new RenderItem(shapeZPos, transformZPos, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(AxeZPos);

	//-Z
	PxSphereGeometry _sphereZNeg(2);
	PxShape* shapeZNeg = CreateShape(_sphereZNeg);
	Vector3 posZNeg = Vector3(0.0f, 0.0f, -10.0f);
	PxTransform* transformZNeg = new PxTransform(posZNeg);
	AxeZNeg = new RenderItem(shapeZNeg, transformZNeg, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(AxeZNeg);
}

Axes::~Axes()
{
	if (AxeCenter) {
		AxeCenter->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeCenter = nullptr;
	}
	if (AxeXPos) {
		AxeXPos->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeXPos = nullptr;
	}
	if (AxeXNeg) {
		AxeXNeg->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeXNeg = nullptr;
	}
	if (AxeYPos) {
		AxeYPos->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeYPos = nullptr;
	}
	if (AxeYNeg) {
		AxeYNeg->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeYNeg = nullptr;
	}
	if (AxeZPos) {
		AxeZPos->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeZPos = nullptr;
	}
	if (AxeZNeg) {
		AxeZNeg->release(); //LLAMA A DEREGISTER-RENDER-ITEM POR SU CUENTA Y HACE EL SHAPE->RELEASE
		AxeZNeg = nullptr;
	}
}
