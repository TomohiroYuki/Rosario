#include "Game\Actor\Actor.h"
#include "Game\Physics\RigidBody.h"
#include "Game\Collision\Collision.h"
#include <vector>
using namespace DirectX;
using namespace Yukitter;

std::vector<std::unique_ptr<Contact>> CollisionBase::contact_list;
void CollisionBase::Collide(SphereCollision* sphere, BoxCollision* box)
{
	DirectX::XMMATRIX mat, inv_mat;
	//XMrotation
	//box.actor_ref->GetActorRotation();

	Yukitter::Vector t = box->actor_ref->GetActorLocation();
	mat = box->actor_ref->GetActorRotationMatrix();
	//DirectX::XMMatrixTransformation()

	//XMmatrixrota
	XMFLOAT4X4 mat4_4;
	XMStoreFloat4x4(&mat4_4, mat);
	mat4_4._41 = t.x; mat4_4._42 = t.y; mat4_4._43 = t.z;
	Vector n(mat4_4._21, mat4_4._22, mat4_4._23);
	mat = XMLoadFloat4x4(&mat4_4);
	inv_mat = XMMatrixInverse(nullptr, mat);

	XMVECTOR v = XMLoadFloat3(&sphere->actor_ref->GetActorLocation().ToXMF3());
	XMFLOAT3 center;
	v = XMVector3TransformCoord(v, inv_mat);
	XMStoreFloat3(&center, v);
	//D3DXVECTOR3 center;

	Vector closest_point;

	closest_point.x = center.x;
	Vector extention(box->box_extention.x*box->actor_ref->GetActorScale().x, box->box_extention.y*box->actor_ref->GetActorScale().y, box->box_extention.z*box->actor_ref->GetActorScale().z);
	extention;
	if (center.x > 0.5f*extention.x) closest_point.x = 0.5f*extention.x;
	if (center.x < -0.5f*extention.x) closest_point.x = -0.5f*extention.x;

	closest_point.y = center.y;
	if (center.y > 0.5f*extention.y) closest_point.y = 0.5f*extention.y;
	if (center.y < -0.5f*extention.y) closest_point.y = -0.5f*extention.y;

	closest_point.z = center.z;
	if (center.z > 0.5f*extention.z) closest_point.z = 0.5f*extention.z;
	if (center.z < -0.5f*extention.z) closest_point.z = -0.5f*extention.z;

	FLOAT distance = (closest_point - center).Length();
	if (distance < sphere->radius*sphere->actor_ref->GetActorScale().x && distance > FLT_EPSILON)
	{
		v = XMVector3TransformCoord(closest_point.ToXMVec(), mat);
		Yukitter::Vector closest(v);
		Collision::contact_list.emplace_back(std::make_unique<Contact>(sphere, box, sphere->actor_ref->GetActorLocation() - closest, closest_point, sphere->radius*sphere->actor_ref->GetActorScale().x - distance, sphere->rigid_ref->restitution*box->rigid_ref->restitution));
		/*static int a = 0;
		std::wstring s = L"HIT:";
		s += std::to_wstring(a++);
		OutputDebugString(s.c_str());*/

	}
}

void CollisionBase::Collide(BoxCollision* box1, BoxCollision* box2)
{
	//Cross()
	auto CalcSumOfProjectedVec = [&](const Vector& v, const Vector& forward, const Vector& right,const Vector& up)
	{
		return Dot(forward, v) + Dot(right, v) + Dot(up, v);
	};

	std::vector<Vector> v;
	XMMATRIX mx = box1->actor_ref->GetActorRotationMatrix();
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, mx);
	Vector f(m._31, m._32, m._33), r(m._11, m._12, m._13), u(m._21, m._22, m._23);
	float ra = CalcSumOfProjectedVec(box1->actor_ref->GetActorTransform().scale3d.x,f,r,u);
	float rb = box1->box_extention.x;
}


bool Contact::Resolve()
{
	auto str0 = L"\nnormal:: x:" + std::to_wstring(normal.x) + L" y:" + std::to_wstring(normal.y) + L" z:" + std::to_wstring(normal.z)+ L"\n";
	std::wstring w = std::to_wstring(penetration)+L"\n";
	//OutputDebugStringW(w.c_str());
	float v_rel = 0;
	static float aaa = 0;

	normal = normal.GetNormalize();
	Vector p_dot_a = Cross(objects[0]->rigid_ref->angular_velocity, (impact_location - objects[0]->rigid_ref->actor_ref->GetActorLocation()));
	p_dot_a += objects[0]->rigid_ref->linear_velocity;

	Vector p_dot_b = Cross(objects[1]->rigid_ref->angular_velocity, (impact_location - objects[1]->rigid_ref->actor_ref->GetActorLocation()));
	p_dot_b += objects[1]->rigid_ref->linear_velocity;

	v_rel = Dot(normal, (p_dot_a - p_dot_b));

	

	//•ªŽqŒvŽZ
	float numerator = 0;
	numerator = -(1 + restitution) * v_rel;

	//•ª•êŒvŽZ	
	float denominator = 0;
	float term1 = objects[0]->rigid_ref->GetInverseMass();
	float term2 = objects[1]->rigid_ref->GetInverseMass();
	Vector ra = impact_location - objects[0]->rigid_ref->actor_ref->GetActorLocation();
	Vector rb = impact_location - objects[1]->rigid_ref->actor_ref->GetActorLocation();
	Vector ta, tb;
	ta = Cross(ra, normal);

	auto hoge = ra.GetNormalize();
	tb = Cross(rb, normal);
	XMMATRIX im0 = objects[0]->rigid_ref->CalcInverseTransformedTensor();
	XMMATRIX im1 = objects[1]->rigid_ref->CalcInverseTransformedTensor();
	ta = XMVector3TransformCoord(ta.ToXMVec(), objects[0]->rigid_ref->CalcInverseTransformedTensor());
	tb = XMVector3TransformCoord(tb.ToXMVec(), objects[1]->rigid_ref->CalcInverseTransformedTensor());
	ta = Cross(ta, ra);
	tb = Cross(tb, rb);
	float term3 = Dot(normal, ta);
	float term4 = Dot(normal, tb);
	denominator = term1 + term2 + term3 + term4;

	float j = 0;
	j = numerator / denominator;


	std::wstring imp = std::to_wstring(j);
	imp += L"\n";
	OutputDebugString(imp.c_str());

	Vector impulse = normal * j;

	//impulse+=Cross()
	if (aaa != 0)
	{
		float argument = impulse.Length() / aaa;
		if (argument < restitution*0.70)
		{

		}
	}
	//imp = L"\nimpulse:: y:" + std::to_wstring(normal.y) + L" z:" + std::to_wstring(normal.z);
	//OutputDebugString(imp.c_str());
	float mu = 0.003f;
	//aaa = impulse.Length();
	Vector relative_velocity = objects[0]->rigid_ref->linear_velocity - objects[1]->rigid_ref->linear_velocity;
	
	Vector v = Cross(Cross(normal, relative_velocity.GetNormalize()),normal);
	Vector friction_f = v * mu * j;
	friction_f = 0;;
	//OutputDebugStringW()
	objects[0]->rigid_ref->linear_velocity += impulse * objects[0]->rigid_ref->GetInverseMass()- friction_f;
	ta = Cross(ra, impulse);
	ta = XMVector3TransformCoord(ta.ToXMVec(), objects[0]->rigid_ref->CalcInverseTransformedTensor());
	objects[0]->rigid_ref->angular_velocity += ta;

	objects[1]->rigid_ref->linear_velocity -= impulse * objects[1]->rigid_ref->GetInverseMass() + friction_f;
	tb = Cross(rb, impulse);
	tb = XMVector3TransformCoord(tb.ToXMVec(), objects[1]->rigid_ref->CalcInverseTransformedTensor());
	objects[1]->rigid_ref->angular_velocity -= tb;

	//‚ß‚èž‚Ý—Ê‚Ì‰ðŒˆ
	normal = normal.GetNormalize();
	auto weight = objects[1]->rigid_ref->mass / (objects[0]->rigid_ref->mass + objects[1]->rigid_ref->mass);
	objects[0]->rigid_ref->actor_ref->GetActorTransform().AddLocation(normal*(penetration * weight));
	weight = objects[0]->rigid_ref->mass / (objects[0]->rigid_ref->mass + objects[1]->rigid_ref->mass);
	objects[1]->rigid_ref->actor_ref->GetActorTransform().AddLocation(normal*(penetration *weight*-1.0f));
	/*body[0]->position += penetration * body[1]->inertial_mass / (body[0]->inertial_mass + body[1]->inertial_mass) * normal;
	body[1]->position -= penetration * body[0]->inertial_mass / (body[0]->inertial_mass + body[1]->inertial_mass) * normal;*/
	/*count++;
	if (count > 0)return true;
*/
	return true;
}


void CollisionBase::Tick()
{
	for (auto it = contact_list.begin(); it != contact_list.end();/**/)
	{
		if ((*it)->Resolve())
		{
			it = contact_list.erase(it);
		}
		else it++;
	}
}

void BoxCollision::CalcInertiaTensor()
{
	Yukitter::Vector v(actor_ref->GetActorScale());
	rigid_ref->SetInertiaTensor(Yukitter::Vector((1.0f / 3.0f)*(v.y*v.y + v.z*v.z)*rigid_ref->mass, (1.0f / 3.0f)*(v.x*v.x + v.z*v.z)*rigid_ref->mass, (1.0f / 3.0f)*(v.x*v.x + v.y*v.y)*rigid_ref->mass));
}

void SphereCollision::CalcInertiaTensor()
{
	float r = radius * actor_ref->GetActorScale().x;
	float tensor = 2.0f / 5.0f*r*r*rigid_ref->mass;
	rigid_ref->SetInertiaTensor(Yukitter::Vector(tensor));
}