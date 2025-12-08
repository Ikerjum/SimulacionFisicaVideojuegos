#include "Bullet.h"

Bullet::Bullet(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color) :
	_item(nullptr), _gPhysics(gPhysics), _gScene(gScene), _timeOfLife(5.0)
{
	_actor = gPhysics->createRigidDynamic(PxTransform(pos));
	_actor->setLinearVelocity(linearVelocity);
	_actor->setAngularVelocity(angularVelocity);
	_shape = CreateShape(PxBoxGeometry(tam));
	_actor->attachShape(*_shape);
	PxRigidBodyExt::updateMassAndInertia(*_actor, 0.15);
	gScene->addActor(*_actor);

	_item = new RenderItem(_shape, _actor, color);
}

Bullet::~Bullet()
{
	if (_item) {
		_item->release();
		_item = nullptr;
	}
	if (_actor) {
		_gScene->removeActor(*_actor);
		_actor->release();
		_actor = nullptr;
	}
}

void Bullet::updateBullet(double t)
{
	_timeOfLife -= t;
}

