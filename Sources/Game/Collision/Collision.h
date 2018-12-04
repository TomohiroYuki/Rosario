#pragma once
#include "math\Transform.h"
class Actor;
class SphereCollision;
class BoxCollision;
class RigidBody;
class Contact;
class CollisionBase
{
public:
	CollisionBase(Actor* actor_ref, RigidBody* rigid_ref) :
		actor_ref(actor_ref),
		rigid_ref(rigid_ref)
	{}
	virtual ~CollisionBase() {}

	enum COLLISION_TYPE
	{
		COL_TYPE_SPHERE = 0,
		COL_TYPE_BOX,
		COL_TYPE_CAPSULE
	}collision_type = COL_TYPE_SPHERE;

	virtual void RenderWire(const Yukitter::Transform&) = 0;

	Yukitter::Transform offset;
	RigidBody* rigid_ref;
protected:
	Actor * const actor_ref;
public:
	const Actor* GetActorReference()
	{
		return actor_ref;
	}

	static std::vector<std::unique_ptr<Contact>> contact_list;

	static void Tick();

	static void Collide(SphereCollision* sphere, BoxCollision* box);
	static void Collide(BoxCollision* box1, BoxCollision* box2);

	virtual void CalcInertiaTensor() {}
	
}typedef Collision;

class Contact
{
public:

	//CollisionBase* object0, CollisionBase* object1, Vector& normal, Vector& impact_location, float penetration, float restitution
	Contact(CollisionBase* object0, CollisionBase* object1, Yukitter::Vector& normal, Yukitter::Vector& impact_location, float penetration, float restitution) :
		normal(normal),
		impact_location(impact_location),
		penetration(penetration),
		restitution(restitution)
	{
		objects[0] = object0;
		objects[1] = object1;
	}
private:
	CollisionBase * objects[2];
	Yukitter::Vector normal;
	Yukitter::Vector impact_location;
	float penetration;
	float restitution;
public:

	bool Resolve();

	int count = 0;
};

class SphereCollision :public CollisionBase
{
public:
	SphereCollision(Actor* actor_ref, RigidBody* rigid_ref) :
		CollisionBase(actor_ref, rigid_ref)
	{
		//collision_type=
		collision_type = COL_TYPE_SPHERE;

		//offset.SetScale(1);
		//radius = 30;
		
	}
	virtual ~SphereCollision() {}

private:

public:
	float radius = 1;
	void RenderWire(const Yukitter::Transform& parent)override
	{
		//RESOURCEM->sphere->Render(offset, parent);
	}
	void CalcInertiaTensor()override;


};



class BoxCollision :public CollisionBase
{
public:
	BoxCollision(Actor* actor_ref, RigidBody* rigid_ref) :
		CollisionBase(actor_ref, rigid_ref),
		box_extention(Yukitter::Vector(1, 1, 1))
	{
		collision_type = COL_TYPE_BOX;
		//offset.SetScale(box_extention);
		
		//CalcInertiaTensor();
	}
	virtual ~BoxCollision() {}

	Yukitter::Vector box_extention;
private:
	void RenderWire(const Yukitter::Transform& parent)override
	{
		//RESOURCEM->box->Render(offset, parent);
	}

	void CalcInertiaTensor()override;
};

