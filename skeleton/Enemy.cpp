#include "Enemy.h"

Enemy::Enemy(PxPhysics* gPhysics, PxScene* gScene, Vector3& pos, Vector3& tam, Vector3& linearVelocity, Vector3& angularVelocity, Vector4& color) {
	_actor = gPhysics->createRigidDynamic(PxTransform(pos));
	_actor->setLinearVelocity(linearVelocity);
	_actor->setAngularVelocity(angularVelocity);
	_shape = CreateShape(PxBoxGeometry(tam));
	_actor->attachShape(*_shape);
	PxRigidBodyExt::updateMassAndInertia(*_actor, 0.15);
	gScene->addActor(*_actor);

	_item = new RenderItem(_shape, _actor, color);
}

Enemy::~Enemy()
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
