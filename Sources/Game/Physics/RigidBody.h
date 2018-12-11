#pragma once
#include "Game\Actor\Actor.h"
#include "System\Object\3D\WirePrimitive.h"
#include "System\input\Input.h"
#include <memory>
#include "math\Vector4.h"
//#include "math\Transform.h"
//#include <DirectXMath.h>
class RigidBody//:// public Actor
{
public:
	//RigidBody(/*Actor* attached_actor,*/float density):
	//	density(density)
	//	//actor_ref(attached_actor)
	//{}
	RigidBody(Actor* attached_actor, float density = 1.0f) :
		density(density),
		actor_ref(attached_actor)
		//actor_ref(attached_actor)
	{
		SetInertiaTensor(Yukitter::Vector(1, 1, 1));

		//DirectX::XMFLOAT4 init(0, 0, 0, 1);
		//orientation = DirectX::XMLoadFloat4(&init);
	}
	virtual ~RigidBody() {}

protected:
	Yukitter::Vector force_accumrator;
	Yukitter::Vector torque_accumrator;
	//Yukitter::Transform transform;
	DirectX::XMMATRIX inertia_tensor;
	//Actor* const actor_ref;
public:
	float restitution = 1.0f;
	//DirectX::XMVECTOR Yukitter::;
	Yukitter::Vector4 orientation;
	float mass = 1;

#ifdef _DEBUG
	//std::shared_ptr<PrimitiveBase> debug_object;

#endif
	Actor* actor_ref;


	Yukitter::Vector linear_velocity;
	Yukitter::Vector angular_velocity;
	float density;

	//inertia_tensor

	bool is_movable;


	virtual void Tick(float dt, int solv = 1)
	{
		AddForce(Yukitter::Vector(0, -9.8f*mass / solv, 0));
		//AddForce(Yukitter::Vector(22, 0, 0));

		//				ƒÏ‹ó‹C–§“x d’¼Œa v1‘Š‘Î‘¬“x n‰ñ“]”
		//	—g—Í::Ft=1/8(ƒÎ^2)ƒÏ(d^3)V1N
		
		CalculatePhysicsBehaviour(dt / solv);
	}

	void AddMagnusForce()
	{
		// ƒÏ::1.293kg / m ^ 3

		float pi = 3.1415926535f;
		float rho = 1.293f*1000.0f;
		float r = actor_ref->GetActorScale().x;
		//AddForce();
		Yukitter::Vector v = Cross(angular_velocity*-1, linear_velocity*-1).GetNormalize()*(1.0f-Dot(angular_velocity.GetNormalize(),linear_velocity.GetNormalize()));
		float f = (1.0f / 2.0f)*pi*rho*r*r*linear_velocity.Length()*angular_velocity.Length();
		AddForce(v*f*0.1f);
	}


	void AddDragForce()
	{
		float r = actor_ref->GetActorScale().x;
		float air_viscosity = 0.0181f*0.001f;
		float drag_coefficient = 0.5f;
		float rho = 1.293f*1000.0f;
		float v_len = linear_velocity.Length();

		auto a=linear_velocity* 6.0f*3.141592f*r*air_viscosity;
		auto b= linear_velocity.GetNormalize()*v_len*v_len*rho*3.141592f*r*r*drag_coefficient / 2;
		Yukitter::Vector f = a+b;
	
		AddForce(f*-1.0f);
	}

	void CalculatePhysicsBehaviour(float dt)
	{
		//dt /= F_CAST(solv);
		if (is_movable)
		{



			Yukitter::Vector linear_accelaration = force_accumrator / mass;
			linear_velocity += linear_accelaration * dt;
			actor_ref->GetActorTransform().AddLocation(linear_velocity*dt);


			//DirectX::XMMATRIX inverse_inertia_tensor = DirectX::XMMatrixInverse(nullptr, inertia_tensor);



			//DirectX::XMMATRIX rotation, transposed_rot;
			////rotation = DirectX::XMMatrixRotationRollPitchYaw(actor_ref->GetActorRotation().pitch, actor_ref->GetActorRotation().yaw, actor_ref->GetActorRotation().roll);
			//
			//rotation = DirectX::XMMatrixRotationQuaternion(orientation);
			//transposed_rot = DirectX::XMMatrixTranspose(rotation);

			//inverse_inertia_tensor = transposed_rot * inverse_inertia_tensor * rotation;
			//Yukitter::Vector angular_acceleration;
			//angular_acceleration = DirectX::XMVector3TransformCoord(torque_accumrator.ToXMVec(), inverse_inertia_tensor);

			//angular_velocity += angular_acceleration * dt;

			////Šp‘¬“x‚É‚æ‚éŽp¨‚ÌXV
			////DirectX::QUATERNION
			//DirectX::XMFLOAT4 w0(angular_velocity.x, angular_velocity.y, angular_velocity.z, 0);
			////auto _orientation = DirectX::XMQuaternionRotationMatrix(rotation);
			//DirectX::XMVECTOR w = DirectX::XMLoadFloat4(&w0);
			//w = DirectX::XMQuaternionMultiply(orientation, w);

			//DirectX::XMStoreFloat4(&w0, w);

			////w = DirectX::XMQuaternionMultiply(_orientation, w);
			//Yukitter::Vector orien(orientation);
			/////*orien +=*/  0.5f * w0 /** dt*/;
			//w0.x = w0.x*0.5f*dt;
			//w0.y = w0.y*0.5f*dt;
			//w0.z = w0.z*0.5f*dt;
			//orien += w0;

			//orientation = orien.ToXMVec();
			//orientation=DirectX::XMQuaternionNormalize(orientation);
			////DirectX::XMMatrixRotationQuaternion(orien.ToXMVec());
			////D3DXQuaternionNormalize(&orientation, &orientation);
			//ƒgƒ‹ƒN(accumulated_torque)‚©‚çŠp‰Á‘¬“x(angular_acceleration)‚ðŽZo‚µŠp‘¬“x(angular_velocity)‚ðXV‚·‚é
			DirectX::XMMATRIX inverse_inertia_tensor = DirectX::XMMatrixInverse(nullptr, inertia_tensor);
			;
			DirectX::XMMATRIX rotation, transposed_rotation;
			rotation = DirectX::XMMatrixRotationQuaternion(orientation.ToXMVec());
			transposed_rotation = DirectX::XMMatrixTranspose(rotation);

			//I1=R^-1IR
			inverse_inertia_tensor = transposed_rotation * inverse_inertia_tensor * rotation;
			//inverse_inertia_tensor = rotation*inverse_inertia_tensor * transposed_rotation  ;
			Yukitter::Vector angular_acceleration = DirectX::XMVector3TransformCoord(torque_accumrator.ToXMVec(), inverse_inertia_tensor);

			//float inv_solv = 1.0f / solv;
			angular_velocity += angular_acceleration * dt;
			auto str0 = L"\nangular_velocity:: x:" + std::to_wstring(angular_velocity.x) + L" y:" + std::to_wstring(angular_velocity.y) + L" z:" + std::to_wstring(angular_velocity.z) + L"\n";
			//std::wstring w = std::to_wstring(penetration) + L"\n";
			OutputDebugStringW(str0.c_str());
			////Šp‘¬“x‚É‚æ‚éŽp¨‚ÌXV
			Yukitter::Vector4 w(angular_velocity, 0);
			//D3DXQUATERNION w(angular_velocity.x, angular_velocity.y, angular_velocity.z, 0);
			w = Vector4(orientation) * w;
			orientation += w * dt*2.0f ;
			//D3DXQuaternionNormalize(&orientation, &orientation);
			orientation = orientation.GetNormalized();
		}

	}


	F_INLINE void AddInitialVelocity(Yukitter::Vector velocity)
	{
		linear_velocity += velocity;
	}
	F_INLINE void AddInitialTorque(Yukitter::Vector a_velocity)
	{
		angular_velocity += a_velocity;
	}

	F_INLINE void ResetAccumrator()
	{
		force_accumrator = Vector();
		torque_accumrator = Vector();
	}

	F_INLINE void AddForce(Yukitter::Vector force)
	{
		force_accumrator += force;
	}
	F_INLINE void AddTorque(Yukitter::Vector torque)
	{
		torque_accumrator += torque;
	}

	F_INLINE float GetInverseMass()
	{
		return (is_movable&&mass > 0 ? 1.0f / mass : 0);
	}

	F_INLINE DirectX::XMMATRIX CalcInverseTransformedTensor()
	{
		if (is_movable)
		{
			Yukitter::Rotator r = actor_ref->GetActorRotation();
			DirectX::XMMATRIX rot = DirectX::XMMatrixRotationQuaternion(orientation.ToXMVec());
			DirectX::XMMATRIX transposed_rot = DirectX::XMMatrixTranspose(rot);


			auto m = transposed_rot * DirectX::XMMatrixInverse(nullptr, inertia_tensor)*rot;
			return m;
		}
		else
		{
			DirectX::XMFLOAT4X4 m(
				FLT_EPSILON, 0, 0, 0,
				0, FLT_EPSILON, 0, 0,
				0, 0, FLT_EPSILON, 0,
				0, 0, 0, 1);

			return DirectX::XMLoadFloat4x4(&m);
		}

	}

	void SetInertiaTensor(Yukitter::Vector in_inertia_tensor)
	{
		DirectX::XMFLOAT4X4 mat = {};
		mat._11 = in_inertia_tensor.x;
		mat._22 = in_inertia_tensor.y;
		mat._33 = in_inertia_tensor.z;
		mat._44 = 1.0f;
		inertia_tensor = DirectX::XMLoadFloat4x4(&mat);
	}

	float GetMass()
	{
		return is_movable ? mass : FLT_MAX;
	}
};

//class RigidBody_Box :public RigidBody
//{
//public:
//	RigidBody_Box(std::shared_ptr<Pawn> pawn,Yukitter::Vector box_extention, float density):
//		RigidBody(density),
//		pawn(pawn),
//		box_extention(box_extention)
//	{
//#if _DEBUG
//		debug_box = RESOURCEM->box;
//		debug_box->transform.SetScale(box_extention);
//
//#endif // _DEBUG
//		//pawn->
//		mass = box_extention.x*box_extention.y*box_extention.z*density;
//
//		inertia_tensor._11 = (0.3333333333f)* (box_extention.y*box_extention.y + box_extention.z*box_extention.z)*mass;
//		inertia_tensor._22 = (0.3333333333f)* (box_extention.x*box_extention.x + box_extention.z*box_extention.z)*mass;
//		inertia_tensor._33 = (0.3333333333f)* (box_extention.x*box_extention.x + box_extention.y*box_extention.y)*mass;
//
//		//http://hooktail.sub.jp/mechanics/inertiaTable1/
//	}
//private:
//	Yukitter::Vector box_extention;
//	std::shared_ptr<BOX> debug_box;
//	std::shared_ptr<Pawn> pawn;
//public:
//
//	void Tick(float dt)override
//	{
//		RigidBody::Tick(dt);
//
//		if (INPUT->KeyGet(DIK_RSHIFT) == 3)
//		{
//			AddForce(Yukitter::Vector(0, 0, 100000));
//		}
//	}
//
//	void DebugRender()
//	{
//		debug_box->Render(debug_box->transform,transform);
//	}
//
//	void Render()
//	{
//		DebugRender();
//		pawn->Render(transform);
//	}
//
//	
//};

//class RigidBody_Sphere :public RigidBody
//{
//public:
//	RigidBody_Sphere(std::shared_ptr<Pawn> pawn, float r, float density) :
//		RigidBody(density),
//		pawn(pawn),
//		radius(r)
//	{
//#if _DEBUG
//		debug_sphere = RESOURCEM->sphere;
//		debug_sphere->transform.SetScale(r);
//
//#endif // _DEBUG
//		//pawn->
//		mass = r*r*r*density*0.01f*0.01f;//cm3’PˆÊ
//
//		inertia_tensor._11 = (2.0f/5.0f)* radius*radius*mass;
//		inertia_tensor._22 = (2.0f/5.0f)* radius*radius*mass;
//		inertia_tensor._33 = (2.0f/5.0f)* radius*radius*mass;
//
//		//http://hooktail.sub.jp/mechanics/inertiaTable1/
//	}
//private:
//
//	std::shared_ptr<SPHERE> debug_sphere;
//	std::shared_ptr<Pawn> pawn;
//	float radius;
//public:
//
//	void Tick(float dt)override
//	{
//		RigidBody::Tick(dt);
//
//		if (INPUT->KeyGet(DIK_RSHIFT) == 3)
//		{
//			AddForce(Yukitter::Vector(0, 0, 100000));
//		}
//	}
//
//	void DebugRender()
//	{
//		debug_sphere->Render(debug_sphere->transform, transform);
//	}
//
//	void Render()
//	{
//		DebugRender();
//		pawn->Render(transform);
//	}
//
//};