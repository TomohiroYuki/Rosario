#pragma once
#include "Sources\System\CoreMinimal.h"
#include "Sources\math\Transform.h"

class BaseComponent
{
public:
	BaseComponent()
	{

	}
protected:
	Yukitter::Transform transform;
	Yukitter::Vector component_velocity;
public:
	F_INLINE Yukitter::Transform GetComponentTransform()
	{
		return transform;
	}
	F_INLINE Yukitter::Vector GetComponentLocation()
	{
		return transform.translation;
	}
	F_INLINE Yukitter::Rotator GetComponentRotation()
	{
		return transform.rotation;
	}
};