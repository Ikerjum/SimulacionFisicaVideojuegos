#include "Defense.h"
#include "checkML.h"
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>

using namespace physx;

//PARTICULA NORMAL CON RENDERITEMS QUE GENERA SOLIDOS RIGIDOS DINAMICOS, BALAS
Defense::Defense(Vector3 initialPos, Vector3 initialDir, Vector4 color, PxReal tam, PxPhysics* gPhysics, PxScene* gScene) :
	Particle(initialDir, initialPos, Vector3(0.0f, -9.8f, 0.0f), 20.0f, color, tam, 200.0f), _countToShoot(0.0), _momentOfShoot(0.5), _gPhysics(gPhysics), _gScene(gScene)
{
	PxBoxGeometry boxGeom(Vector3(tam * 0.75,tam,tam * 0.75));
	PxShape* shapeBox = CreateShape(boxGeom);
	_offsetBody = Vector3(0.0f, 5.0f, 0.0f);
	_bodyTransform = PxTransform(initialPos + _offsetBody);
	_body = new RenderItem(shapeBox, &_bodyTransform, color);

	PxSphereGeometry sphereGeom(tam);
	PxShape* shapeHead = CreateShape(sphereGeom);
	_offsetHead = Vector3(0.0f, 15.0f, 0.0f);
	_headTransform = PxTransform(initialPos + _offsetHead);
	_head = new RenderItem(shapeHead, &_headTransform, color);

	CreateLittlePart(tam, initialPos, color, Vector3(4.5f, 12.0f, 4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(-4.5f, 12.0f, 4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(4.5f, 12.0f, -4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(-4.5f, 12.0f, -4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(0.0f, 12.0f, -4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(-4.5f, 12.0f, 0.0f));
	CreateLittlePart(tam, initialPos, color, Vector3(0.0f, 12.0f, 4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(4.5f, 12.0f, 0.0f));

	CreateLittlePart(tam, initialPos, color, Vector3(4.5f, 2.0f, 4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(-4.5f, 2.0f, 4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(4.5f, 2.0f, -4.5f));
	CreateLittlePart(tam, initialPos, color, Vector3(-4.5f, 2.0f, -4.5f));

	_buoyancyForce = new BuoyancyForceGenerator(0.1f, 0.2f, 1000.0f, 2.5f);
	_forceGenerators.push_back(_buoyancyForce);

	_color = color;
}

void Defense::CreateLittlePart(physx::PxReal tam, Vector3& initialPos, Vector4& color, Vector3& offset)
{
	PxShape* littlePartshapeBox = CreateShape(PxBoxGeometry(Vector3(tam * 0.3, tam * 0.4, tam * 0.3)));
	_littlePartsOffset.push_back(offset);
	_littlePartsTransform.emplace_back(PxTransform(initialPos + offset));
	_littlePartsItem.push_back(new RenderItem(littlePartshapeBox, &_littlePartsTransform.back(), color));
}

Defense::~Defense() {

	_forceGenerators.clear();
	if (_buoyancyForce) {
		delete _buoyancyForce;
		_buoyancyForce = nullptr;
	}

	if (_body) {
		DeregisterRenderItem(_body);
		delete _body;
		_body = nullptr;
	}
	if (_head) {
		DeregisterRenderItem(_head);
		delete _head;
		_head = nullptr;
	}

	for (int i = 0; i < _littlePartsItem.size(); ++i) {
		DeregisterRenderItem(_littlePartsItem[i]);
		delete _littlePartsItem[i];
		_littlePartsItem[i] = nullptr;
	}
	_littlePartsItem.clear();
	_littlePartsTransform.clear();
	_littlePartsOffset.clear();

	for (Bullet* bullet : _bullets) {
		delete bullet;
		bullet = nullptr;
	}
	_bullets.clear();
}

void Defense::updatePartOfDefense()
{
	if (_body) {
		_bodyTransform = getPos();
		_bodyTransform.p += _offsetBody;
		_body->transform = &_bodyTransform;
	}

	if (_head) {
		_headTransform = getPos();
		_headTransform.p += _offsetHead;
		_head->transform = &_headTransform;
	}

	if (!_littlePartsItem.empty()) {
		for (int i = 0; i < _littlePartsItem.size(); ++i) {
			if (_littlePartsItem[i] != nullptr) {
				_littlePartsTransform[i] = getPos();
				_littlePartsTransform[i].p += _littlePartsOffset[i];
				_littlePartsItem[i]->transform = &_littlePartsTransform[i];
			}
		}
	}
}

void
Defense::update(double t) {
	if (!_body || !_head || _littlePartsItem.empty()) return;
	//Movimiento
	ApplyForces(t);
	integrate_Verlet(t);
	updatePartOfDefense();
	//Balas
	GenerateBullet(t);
	updateBullets(t);
	CheckWindForce();
	
}

void Defense::CheckWindForce()
{
	for (ForceGenerator* windForce : _forceGenerators) {
		if (windForce->getType() == ForceGenerator::WIND && !windForce->isActive()) {
			setPos(Vector3(getOldPos().x, getPos().p.y, getOldPos().z));
		}
	}
}

void Defense::updateBullets(double t)
{
	for (auto it = _bullets.begin(); it != _bullets.end(); ) {

		(*it)->updateBullet(t);	

		if ((*it)->getTimeOfLife() <= 0) {
			delete (*it);
			it = _bullets.erase(it);
		}
		else {
			++it;
		}
	}
}

void Defense::GenerateBullet(double t)
{
	_countToShoot += t;
	if (_countToShoot >= _momentOfShoot) {
		_countToShoot = 0;
		Bullet* newBullet = new Bullet(_gPhysics, _gScene, _headTransform.p, Vector3(1.0f, 1.0f, 1.0f), Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), _color);
		for (ForceGenerator* fg : _forceGenerators) {
			if (fg->getType() == ForceGenerator::GRAVITY || fg->getType() == ForceGenerator::WIND) {
				newBullet->addForceGenerator(fg);
			}
		}
		_bullets.push_back(newBullet);
	}
}



void Defense::addForceGenerator(ForceGenerator* newForceGenerator) {
	_forceGenerators.push_back(newForceGenerator);
}

void Defense::initializeForceBounce()
{
	if (!_buoyancyForce) {
		_buoyancyForce = new BuoyancyForceGenerator(0.1f, 0.2f, 1000.0f, 2.5f);
		_forceGenerators.push_back(_buoyancyForce);
	}
}

void Defense::ApplyForces(double t) {
	this->setAcc(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < _forceGenerators.size(); ++i) {
		Vector3 newForce = _forceGenerators[i]->putForce(this);
		if (this->getMass() != 0.0f) this->setAcc(this->getAcc() + newForce / this->getMass());
	}
}
