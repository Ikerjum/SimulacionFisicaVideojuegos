#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Axes
{
private:
	RenderItem* AxeCenter;
	RenderItem* AxeXPos;
	RenderItem* AxeXNeg;
	RenderItem* AxeYPos;
	RenderItem* AxeYNeg;
	RenderItem* AxeZPos;
	RenderItem* AxeZNeg;
public:
	Axes();
	~Axes();
};

