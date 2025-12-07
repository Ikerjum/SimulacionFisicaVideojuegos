#ifndef __RENDER_UTILS_H__
#define __RENDER_UTILS_H__

#include "PxPhysicsAPI.h"
#include "core.hpp"

class RenderItem;
void RegisterRenderItem(const RenderItem* _item);
void DeregisterRenderItem(const RenderItem* _item);

class RenderItem
{
public:
	RenderItem(physx::PxShape* _shape, const physx::PxTransform* _trans, const Vector4& _color) :
		shape(_shape), transform(_trans), actor(NULL), color(_color), references(1), ownsShape(true)
	{
		if (shape) shape->acquireReference();
		RegisterRenderItem(this);
	}

	RenderItem(physx::PxShape* _shape, const Vector4& _color) :
		shape(_shape), transform(NULL), actor(NULL), color(_color), references(1), ownsShape(true)
	{
		if (shape) shape->acquireReference();
		RegisterRenderItem(this);
	}

	RenderItem(physx::PxShape* _shape, const physx::PxRigidActor* _actor, const Vector4& _color) :
		shape(nullptr), transform(NULL), actor(_actor), color(_color), references(1), ownsShape(false)
	{
		//shape->acquireReference();
		RegisterRenderItem(this);
	}

	RenderItem() : shape(NULL), transform(NULL), references(1), ownsShape(false) {}

	void addReference()
	{
		++references;
	}

	void release()
	{
		if (references == 0) return;
		--references;
		if (references == 0)
		{
			DeregisterRenderItem(this);
			if (shape && ownsShape) {
				shape->release();
			}
			delete this;
		}
	}

public:
	physx::PxShape* shape;
	const physx::PxTransform* transform;
	const physx::PxRigidActor* actor;
	Vector4 color;

	unsigned references;
	bool ownsShape;
};

double GetLastTime();
Camera* GetCamera();

physx::PxShape* CreateShape(const physx::PxGeometry& geo, const physx::PxMaterial* mat = nullptr);

#endif