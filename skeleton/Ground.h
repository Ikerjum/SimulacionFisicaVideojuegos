#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Ground
{
private:
	RenderItem* _groundItem;
	RenderItem* _path1;
	RenderItem* _path2;
	Vector3 _pos;
public:
	Ground(Vector3 pos);
	~Ground();

	Vector3 getPos() const { return _pos; }
};

