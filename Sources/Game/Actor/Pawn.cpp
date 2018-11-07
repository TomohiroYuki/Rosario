#include "Game\Actor\Pawn.h"
#include "Game\Physics\RigidBody.h"

using namespace DirectX;

Pawn::Pawn(const char* filename, CollisionBase::COLLISION_TYPE type ) :
	Actor()
{
	std::string name = filename;
	mesh = RESOURCEM->FindStaticMesh(name);
	if (mesh == nullptr)
	{
		mesh = std::make_shared<StaticMesh>(filename);
		RESOURCEM->AddToResourceMap(name, mesh);
	}

	rigid_body = std::make_unique<RigidBody>(this);
	switch (type)
	{
	case CollisionBase::COL_TYPE_SPHERE:
		collision = std::make_unique<SphereCollision>(this, rigid_body.get());
		break;
	case CollisionBase::COL_TYPE_BOX:
		collision = std::make_unique<BoxCollision>(this, rigid_body.get());
		break;
	case CollisionBase::COL_TYPE_CAPSULE:
		assert(1);
		break;
	default:
		break;
	}



}

void Pawn::Tick(float dt)
{
	rigid_body->Tick(dt);
}

void Pawn::SetIsSimulated(bool input)
{
	rigid_body->is_movable = input;
}


void Pawn::SetMass(float mass)
{
	rigid_body->mass = mass;
}
void Pawn::SetMass(float volume, float dencity)
{
	rigid_body->mass = volume * dencity;
}

F_INLINE DirectX::XMMATRIX Pawn::GetActorRotationMatrix()
{
	if (rigid_body)
	{
		auto a=DirectX::XMMatrixRotationQuaternion(rigid_body->orientation.ToXMVec());
		return a;
	}
	else return Actor::GetActorRotationMatrix();
}

void Pawn::OrientationLerp(Yukitter::Vector axis, float angle,float t)
{
	auto p=DirectX::XMQuaternionRotationAxis(axis.ToXMVec(), angle);
	rigid_body->orientation=DirectX::XMQuaternionSlerp(rigid_body->orientation.ToXMVec(), DirectX::XMQuaternionNormalize(p),t);
	
}

void Pawn::RotateOrientation(Yukitter::Vector axis, float angle)
{
	auto p = DirectX::XMQuaternionRotationAxis(axis.ToXMVec(), angle);
	rigid_body->orientation = XMQuaternionMultiply(rigid_body->orientation.ToXMVec(),p);
}