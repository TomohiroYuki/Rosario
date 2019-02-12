#pragma once

#include "Sources\System\Object\Component\PrimitiveComponent.h"

class SphereComponent :public PrimitiveComponent
{
public:
	SphereComponent() :
		PrimitiveComponent() {}
protected:
	float radius;
public:

};
