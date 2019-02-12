#pragma once
#include "Sources\System\Object\Component\PrimitiveComponent.h"

class CapsuleComponent :public PrimitiveComponent
{
public:
	CapsuleComponent() :
		PrimitiveComponent() {}
protected:
	float capsule_half_height;
	float radius;
public:

};
