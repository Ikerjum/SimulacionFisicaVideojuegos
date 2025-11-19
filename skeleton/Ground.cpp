#include "Ground.h"


using namespace physx;

//Ground::Ground(Vector3 pos) : _pos(pos) {
    //PxBoxGeometry boxGeom(PxVec3(200.0f, 0.1f, 200.0f));
    //PxShape* shapeBox = CreateShape(boxGeom);
    //PxTransform* transformBox = new PxTransform(pos);
    //_groundItem = new RenderItem(shapeBox, transformBox, Vector4(0.0f, 0.01f, 0.0f, 1.0f));
    //RegisterRenderItem(_groundItem);
    //
    //PxBoxGeometry boxGeomPath1(PxVec3(10.0f, 0.3f, 200.0f));
    //PxShape* shapeBoxPath1 = CreateShape(boxGeomPath1);
    //PxTransform* transformBoxPath1 = new PxTransform(pos + Vector3(60.f,0.f,0.f));
    //_path1 = new RenderItem(shapeBoxPath1, transformBoxPath1, Vector4(0.0f, 0.2f, 0.2f, 1.0f));
    //RegisterRenderItem(_path1);
    //
    //PxBoxGeometry boxGeomPath2(PxVec3(10.0f, 0.3f, 200.0f));
    //PxShape* shapeBoxPath2 = CreateShape(boxGeomPath2);
    //PxTransform* transformBoxPath2 = new PxTransform(pos + Vector3(-60.f,0.f,0.f));
    //_path2 = new RenderItem(shapeBoxPath2, transformBoxPath2, Vector4(0.0f, 0.2f, 0.2f, 1.0f));
    //RegisterRenderItem(_path2);
//}

//Ground::Ground(PxScene* gScene, Vector3 pos)
//{
//    _groundRigid = gPhy
//}

Ground::Ground(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) : _pos(pos)
{
    _groundRigid = gPhysics->createRigidStatic(PxTransform(pos));
    PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
    _groundRigid->attachShape(*shape);
    gScene->addActor(*_groundRigid);

    _groundItem = new RenderItem(shape, _groundRigid, { 0.1,0.1,0.1,1 });
    RegisterRenderItem(_groundItem);
}

Ground::~Ground() {
    if (_groundRigid) {
        _groundRigid->release();
        _groundRigid = nullptr;
    }
    if (_groundItem) {
        _groundItem->release();
        _groundItem = nullptr;
    }
    //if (_groundItem) {
    //    _groundItem->release();
    //    _groundItem = nullptr;
    //}
    //if (_path1) {
    //    _path1->release();
    //    _path1 = nullptr;
    //}
    //if (_path2) {
    //    _path2->release();
    //    _path2 = nullptr;
    //}
}
