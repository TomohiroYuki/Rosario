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
	auto CalcSumOfProjectedVec = [&](const Vector& v, const Vector& forward, const Vector& right, const Vector& up)
	{
		return Dot(forward, v) + Dot(right, v) + Dot(up, v);
	};

	std::vector<Vector> v;
	std::vector<float> extention;
	XMMATRIX mx1 = box1->actor_ref->GetActorRotationMatrix();
	XMFLOAT4X4 m1;
	XMStoreFloat4x4(&m1, mx1);
	XMMATRIX mx2 = box2->actor_ref->GetActorRotationMatrix();
	XMFLOAT4X4 m2;
	XMStoreFloat4x4(&m2, mx2);
	Vector f1(m1._31, m1._32, m1._33), r1(m1._11, m1._12, m1._13), u1(m1._21, m1._22, m1._23);
	Vector f2(m2._31, m2._32, m2._33), r2(m2._11, m2._12, m2._13), u2(m2._21, m2._22, m2._23);

	v.emplace_back(f2);
	v.emplace_back(r2);
	v.emplace_back(u2);
	extention.emplace_back(box2->box_extention.z);
	extention.emplace_back(box2->box_extention.x);
	extention.emplace_back(box2->box_extention.y);
	int loop_count = 0;
	for (auto& vec : v)
	{
		Vector scale = box1->actor_ref->GetActorTransform().scale3d;
		Vector l = vec;
		float ra = CalcSumOfProjectedVec(l, f1*scale.z, r1*scale.x, u1*scale.y);
		float rb = extention[loop_count];

		if (ra + rb > Absolute(Dot(box1->actor_ref->GetActorLocation() - box2->actor_ref->GetActorLocation(), l)))
		{

		}
		loop_count++;
	}
	v.clear();
	extention.clear();
	v.emplace_back(f1);
	v.emplace_back(r1);
	v.emplace_back(u1);
	extention.emplace_back(box1->box_extention.z);
	extention.emplace_back(box1->box_extention.x);
	extention.emplace_back(box1->box_extention.y);
	loop_count = 0;
	for (auto& vec : v)
	{
		Vector scale = box2->actor_ref->GetActorTransform().scale3d;
		Vector l = vec;
		float ra = CalcSumOfProjectedVec(l, f2*scale.z, r2*scale.x, u2*scale.y);
		float rb = extention[loop_count];

		if (ra + rb > Absolute(Dot(box1->actor_ref->GetActorLocation() - box2->actor_ref->GetActorLocation(), l)))
		{

		}
		loop_count++;
	}
}


bool Contact::Resolve()
{

	auto str0 = L"\nnormal:: x:" + std::to_wstring(normal.x) + L" y:" + std::to_wstring(normal.y) + L" z:" + std::to_wstring(normal.z) + L"\n";
	std::wstring w = std::to_wstring(penetration) + L"\n";
	//OutputDebugStringW(w.c_str());
	float v_rel = 0;
	static float aaa = 0;

	normal = normal.GetNormalize();
	Vector p_dot_a = Cross(objects[0]->rigid_ref->angular_velocity, (impact_location - objects[0]->rigid_ref->actor_ref->GetActorLocation()));
	p_dot_a += objects[0]->rigid_ref->linear_velocity;

	Vector p_dot_b = Cross(objects[1]->rigid_ref->angular_velocity, (impact_location - objects[1]->rigid_ref->actor_ref->GetActorLocation()));
	p_dot_b += objects[1]->rigid_ref->linear_velocity;


	static int count = 0;

	count++;
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




	Vector impulse = normal * j;

	//impulse+=Cross()

	//imp = L"\nimpulse:: y:" + std::to_wstring(normal.y) + L" z:" + std::to_wstring(normal.z);
	//OutputDebugString(imp.c_str());
	float mu = 0.003f;
	//aaa = impulse.Length();
	Vector relative_velocity = objects[0]->rigid_ref->linear_velocity - objects[1]->rigid_ref->linear_velocity;

	Vector v = Cross(Cross(normal, relative_velocity.GetNormalize()), normal);
	Vector friction_f = v * mu * j;
	friction_f = 0;;
	//OutputDebugStringW()
	ta = Cross(ra, impulse);
	ta = XMVector3TransformCoord(ta.ToXMVec(), objects[0]->rigid_ref->CalcInverseTransformedTensor());
	//objects[0]->rigid_ref->angular_velocity += ta;
	if (objects[0]->rigid_ref->is_movable)
	{
		objects[0]->rigid_ref->angular_velocity -= tb;
		objects[0]->rigid_ref->linear_velocity += impulse * objects[0]->rigid_ref->GetInverseMass() - friction_f;

	}
	tb = Cross(rb, impulse);
	tb = XMVector3TransformCoord(tb.ToXMVec(), objects[1]->rigid_ref->CalcInverseTransformedTensor());
	if (objects[1]->rigid_ref->is_movable)
	{
		objects[1]->rigid_ref->angular_velocity -= tb;
		objects[1]->rigid_ref->linear_velocity -= impulse * objects[1]->rigid_ref->GetInverseMass() + friction_f;

	}

	/*std::wstring imp = std::to_wstring(tb.Length());
	imp = L"tb" + std::to_wstring(count) + L":" + imp + L"\n";
	OutputDebugString(imp.c_str());*/

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