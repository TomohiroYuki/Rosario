#pragma once
#include "system\Utility.h"
#include <DirectXMath.h>

namespace Yukitter
{

	struct Vector
	{

		float x;
		float y;
		float z;

		F_INLINE Vector() :
			x(0), y(0), z(0) {}

		F_INLINE Vector(float x, float y, float z) :
			x(x), y(y), z(z) {}

		F_INLINE Vector(float in_f) :
			x(in_f), y(in_f), z(in_f) {}

		F_INLINE Vector(DirectX::XMFLOAT3 in_v) :
			x(in_v.x), y(in_v.y), z(in_v.z) {}
		F_INLINE Vector(DirectX::XMVECTOR v) 
		{
			DirectX::XMFLOAT3 v3;
			DirectX::XMStoreFloat3(&v3, v);
			x = v3.x;
			y = v3.y;
			z = v3.z;
		}

		//	オペレーター
		F_INLINE Vector& operator =  (const Vector& v) { x = v.x; y = v.y; z = v.z; return *this; }
		F_INLINE Vector& operator += (const Vector& v) { x += v.x; y += v.y; z += v.z; return *this; }
		F_INLINE Vector& operator -= (const Vector& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		F_INLINE Vector& operator *= (float v) { x *= v; y *= v; z *= v; return *this; }
		F_INLINE Vector& operator /= (float v) { x /= v; y /= v; z /= v; return *this; }

		F_INLINE Vector operator + (const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
		F_INLINE Vector operator - (const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
		F_INLINE Vector operator * (float v) const { Vector ret(x*v, y*v, z*v); return ret; }
		F_INLINE Vector operator / (float v) const { Vector ret(x / v, y / v, z / v); return ret; }

		F_INLINE bool operator == (const Vector& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
		F_INLINE bool operator != (const Vector& v) const { return (x != v.x) || (y != v.y) || (z != v.z); }

		F_INLINE Vector operator + (const DirectX::XMFLOAT3& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
		F_INLINE Vector operator - (const DirectX::XMFLOAT3& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
		F_INLINE Vector& operator =  (const DirectX::XMFLOAT3& v) { x = v.x; y = v.y; z = v.z; return *this; }
		F_INLINE Vector& operator += (const DirectX::XMFLOAT4& v) { x += v.x; y += v.y; z += v.z; return *this; }
		F_INLINE Vector& operator -= (const DirectX::XMFLOAT4& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

		/*Vector operator * (FLOAT f, const Vector& v)
		{
			return Vector(f * v.x, f * v.y, f * v.z);
		}*/

		F_INLINE DirectX::XMFLOAT3 ToXMF3(const Vector& v) const { return DirectX::XMFLOAT3(v.x, v.y, v.z); }
		F_INLINE DirectX::XMFLOAT3 ToXMF3() const { return DirectX::XMFLOAT3(x, y, z); }
		F_INLINE DirectX::XMVECTOR ToXMVec() const { return DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(x,y,z))); }
		F_INLINE float Length() { return sqrtf(x*x + y * y + z * z); }
		F_INLINE float LengthSq() { return x * x + y * y + z * z; }

		F_INLINE Vector GetNormalize()
		{
			Vector copy(x, y, z);
			float l = Length();
			return l != .0f ? Vector(copy.x / l, copy.y / l, copy.z / l) : Vector();
		}




	};

	F_INLINE float Absolute(float in_f)
	{
		return in_f > 0 ? in_f : -1 * in_f;
	}

	F_INLINE float Clamp(float value,float in_min,float in_max)
	{
		return value > in_min ? (value<in_max?value: in_max) : in_min;
	}


	F_INLINE float Dot(const Vector& v0, const Vector& v1)
	{
		return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
	}

	F_INLINE Vector GetProjectedVectorToPlane(Vector in_vec,Vector normal)
	{
		return (in_vec - (normal*Dot(in_vec, normal)));
	}

	F_INLINE Vector Cross(const Vector& v0, const Vector& v1)
	{
		return Vector(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
	}
}

