#pragma once
#include "system\object\3d\StaticMesh.h"
//#include "source\game\YQCamera.h"

#include "math\Transform.h"
#include <DirectXMath.h>
#include <memory>
#include "System\ResourceManager.h"
//using Matrix4x4 = DirectX::XMFLOAT4X4;
struct Matrix4x4
{

};

//void A()
//{
//	Matrix4x4 m;
//}

class Actor
{
public:
	Actor() :
		parent(nullptr),
		has_parent(false) 
	{}
	
	virtual ~Actor() {}	
	
protected:

	Actor* parent;
	bool has_parent;
	Yukitter::Transform transform;

	//std::unique_ptr<StaticMesh> mesh;
	
public:
	
	F_INLINE Yukitter::Vector& GetActorLocation()
	{
		return transform.translation;
	}
	virtual F_INLINE Yukitter::Rotator& GetActorRotation()
	{
		return transform.rotation;
	}

	F_INLINE Yukitter::Vector& GetActorScale()
	{
		return transform.scale3d;
	}

	F_INLINE virtual void Tick(float dt) {}
	
	F_INLINE virtual void Render() {}
	
	F_INLINE Yukitter::Transform& GetActorTransform()
	{
		return transform;
	}

	F_INLINE bool SetParent(Actor* parent_actor)
	{
		if (parent == parent_actor)return false;
		parent = parent_actor;
		has_parent = true;
		return true;
	}

	virtual F_INLINE DirectX::XMMATRIX GetActorRotationMatrix()
	{
		return DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.pitch, transform.rotation.yaw, transform.rotation.roll);
	}

};
