#include "Ground.h"
#include "checkML.h"
#include <memory>
#include <assert.h>

using namespace physx;

Ground::Ground(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) : _pos(pos), _gScene(gScene)
{
    //CREATE SEA
    CreatePath(gPhysics, gScene, pos, Vector3(100.0f, 2.0f, 100.0f), Vector3(0.0f,0.0f,0.0f), Vector4(0.1f, 0.1f, 0.8f, 1.0f));
    //CREATE PATHS
    CreatePath(gPhysics, gScene ,pos, Vector3(10.0f, 3.0f ,100.0f), Vector3(30.0f, 10.0f, 0.0f), Vector4(0.1f, 0.2f, 0.2f, 1.0f));
    CreatePath(gPhysics, gScene, pos, Vector3(10.0f, 3.0f ,100.0f), Vector3(-30.0f, 10.0f, 0.0f), Vector4(0.1f, 0.2f, 0.2f, 1.0f));
}

void Ground::CreatePath(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3& pos, Vector3& scale, Vector3& offset, Vector4& color)
{
    PxBoxGeometry boxGeom(scale);
    PxShape* shape = CreateShape(boxGeom);
    physx::PxRigidStatic* actor = gPhysics->createRigidStatic(PxTransform(pos + offset));
    actor->attachShape(*shape);
    gScene->addActor(*actor);
    RenderItem* item = new RenderItem(shape, actor, color);
    //RegisterRenderItem(item);

    assert(actor != nullptr);
    assert(shape != nullptr);

    _actors.push_back(actor);
    _renderItems.push_back(item);
}

Ground::~Ground() {

    for (RenderItem* item : _renderItems) {
        item->release();
        item = nullptr;
    }
    _renderItems.clear();

    for (physx::PxRigidStatic* actor : _actors) {
        _gScene->removeActor(*actor);
        actor->release();
        actor = nullptr;
    }
    _actors.clear();
}
