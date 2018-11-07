#pragma once

#include "math\Vector.h"
#include "math\Rotator.h"

namespace Yukitter
{
	struct Transform
	{
	public:
		Vector translation;
		Rotator rotation;
		Vector scale3d;
	public:
		F_INLINE Transform() :
			translation(),rotation(),  scale3d(1,1,1) {}

		F_INLINE Transform(Vector translation,Rotator rotator, Vector scale3d) :
			translation(translation),rotation(rotator),  scale3d(scale3d) {}

		F_INLINE Transform& operator =  (const Transform& t) { translation = t.translation; rotation = t.rotation; scale3d = t.scale3d; return *this; }

		//=====================================================================
		//Translation (Location) Functions
		//=====================================================================
		F_INLINE Vector SetLocation(Vector in_location)
		{
			translation = in_location;
			return translation;
		}
		F_INLINE Vector AddLocation(Vector in_translation)
		{
			translation += in_translation;
			return translation;
		}
		//=====================================================================
		//Rotation Functions
		//=====================================================================
		
		F_INLINE Rotator SetRotation(Rotator in_rotation)
		{
			rotation = in_rotation;
			return rotation;
		}
		
		F_INLINE Rotator AddRotation(Rotator in_rotation)
		{
			rotation += in_rotation;
			return rotation;
		}
		//=====================================================================
		//Scale Functions
		//=====================================================================
		F_INLINE Vector SetScale(Vector in_scale3d)
		{
			scale3d = in_scale3d;
			return scale3d;
		}
		F_INLINE Vector SetScale(float scale)
		{
			scale3d = Vector(scale, scale, scale);
			return scale3d;
		}


	};

}