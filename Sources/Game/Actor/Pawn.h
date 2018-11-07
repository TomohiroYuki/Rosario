#pragma once
#include "Game\Actor\Actor.h"
#include "Game\Collision\Collision.h"
class RigidBody;
class Pawn :public Actor
{
public:
	Pawn(const char* filename, CollisionBase::COLLISION_TYPE type = CollisionBase::COLLISION_TYPE::COL_TYPE_SPHERE);
	virtual ~Pawn() 
	{
		int a = 0;
	}
protected:
public:
	std::unique_ptr<RigidBody> rigid_body;
	std::unique_ptr<CollisionBase> collision;

	std::shared_ptr<StaticMesh> mesh;

	virtual void Tick(float dt)override;
	
	virtual void Render()override
	{
		if (!has_parent)
			
			mesh->Render(transform.scale3d, GetActorRotationMatrix(), transform.translation);

		else
			mesh->Render(transform, parent->GetActorTransform());

#ifdef _DEBUG
		collision->RenderWire(transform);
#endif

	}
	void Render(Yukitter::Transform parent)
	{
		
		//mesh->Render(transform, parent);
		mesh->Render(transform.scale3d, GetActorRotationMatrix(), transform.translation);


	}

	void SetIsSimulated(bool input);
	

	void SetMass(float mass);
	void SetMass(float volume, float dencity);

	void OrientationLerp(Yukitter::Vector axis, float angle,  float t);

	F_INLINE DirectX::XMMATRIX GetActorRotationMatrix()override;

	void RotateOrientation(Yukitter::Vector axis, float angle);

};