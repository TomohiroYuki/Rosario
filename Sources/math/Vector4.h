#pragma once
#include "System\Utility.h"
#include "math\Vector.h"

namespace Yukitter
{
	struct Vector4
	{
		float x, y, z, w;
		F_INLINE Vector4():
			x(0),y(0),z(0),w(1){}
		F_INLINE Vector4(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w) {}
		F_INLINE Vector4(Vector v, float w = 0.0f) :
			x(v.x), y(v.y), z(v.z), w(w) {}
		F_INLINE Vector4(DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT4 v4;
			DirectX::XMStoreFloat4(&v4, v);
			x = v4.x;
			y = v4.y;
			z = v4.z;
			w = v4.w;
		}
		F_INLINE Vector4& operator =  (const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
		F_INLINE Vector4 operator * (const Vector4& v) const{	return Vector4(DirectX::XMQuaternionMultiply(ToXMVec(), v.ToXMVec()));}
		F_INLINE Vector4 operator * (float in_f) const { return Vector4(x*in_f, y*in_f, z*in_f, w*in_f); }
		F_INLINE Vector4& operator += (const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		F_INLINE DirectX::XMFLOAT4 ToXMF4() const { return DirectX::XMFLOAT4(x, y, z, w); }
		F_INLINE DirectX::XMVECTOR ToXMVec() const { return DirectX::XMLoadFloat4(&(DirectX::XMFLOAT4(x, y, z, w))); }
	
		F_INLINE Vector4 GetNormalized()
		{
			return Vector4(DirectX::XMQuaternionNormalize(ToXMVec()));
		}


	};
	// w = DirectX::XMQuaternionMultiply(_orientation, w);
}