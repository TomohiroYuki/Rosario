#include "game\scene\SceneZero.h"
#include "Game\YQCamera.h"
#include "Game\Collision\Collision.h"
#include "Game\Actor\Ball.h"
#include "System\Utility.h"
void SceneZero::Initialize()
{

	//yuppi  = std::make_unique<StaticMesh>("DATA\\3D\\yuppi_wait.fbx");
	//yuppi = std::make_unique<StaticMesh>("DATA\\3D\\gune1.fbx");
	
	camera = std::make_unique<Yukitter::ViewerCamera>();


	GameBrain::ActivateCamera(camera.get());
	//camera->transform.SetLocation()
	cube = std::make_unique<Pawn>("..\\Assets\\StandardAssets\\cube.fbx", CollisionBase::COLLISION_TYPE::COL_TYPE_BOX);
	cube->GetActorTransform().SetScale(Yukitter::Vector(100.0f, 0.2f, 10.0f));
	cube->SetMass(FLT_MAX);
	cube->collision->CalcInertiaTensor();
	sphere = std::make_unique<Ball>();
	//sphere->GetActorTransform().SetScale(Yukitter::Vector(30.0f, 30,30.0f));
	sphere->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
	sphere->SetIsSimulated(true);
	//sphere->rigid_body->AddTorque(Yukitter::Vector(0,0, 0));

}

void SceneZero::Tick(float dt)
{

	camera->Update();
	//yuppi_star->Tick();
	//test_object->Tick();
	//rigid->Tick(1.0f/60.0f);
	static bool b = false;
	static bool bb = false;
	if (INPUT->KeyGet(DIK_O) == 3)
	{
		bb = !bb;
	}
	if (bb)return;
	if (INPUT->KeyGet(DIK_SPACE) == 3)
	{
		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(0, 1, 0, 1);
		ball_list[balls];
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 0, 48));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(140 * 1000 / 60.0f / 60.0f, 0, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (INPUT->KeyGet(DIK_END) == 3)
	{
		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(1, 0, 0, 1);
		ball_list[balls];
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 0, 0.1f));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(140 * 1000 / 60.0f / 60.0f, 0, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (INPUT->KeyGet(DIK_RSHIFT) == 3)
	{
		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->color=Yukitter::Vector4(0,0,1,1);
		ball_list[balls]->line->Clear();
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 12, 48));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(140 * 1000 / 60.0f / 60.0f, 0, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}

	
	if (!b)return;
	//sphere->line->AddPoint(sphere->GetActorLocation());

	const int solv = 5;
	//sphere->rigid_body->AddMagnusForce();

	for (auto& a : ball_list)
	{
		a->line->AddPoint(a->GetActorLocation());
		a->rigid_body->AddMagnusForce();;
	}
	for (int i = 0; i < solv; i++)
	{
		//sphere->rigid_body->Tick(dt, solv);

		for (auto& a : ball_list)
		{
			a->rigid_body->Tick(dt, solv);
		}

		cube->Tick(dt);

		Collision::Collide(dynamic_cast<SphereCollision*>(sphere->collision.get()), dynamic_cast<BoxCollision*>(cube->collision.get()));

		Collision::Tick();


	}
	cube->rigid_body->ResetAccumrator();
	//sphere->rigid_body->ResetAccumrator();
	for (auto& a : ball_list)
	{
		a->rigid_body->ResetAccumrator();
	}


}
void SceneZero::Render(float dt)
{
	grid->Render(Vector(0.1f));
	//box->Render();
	//rigid->Render();
	//yuppi->Render(dt ,DirectX::XMFLOAT3(0, 30, 0), 0, 1);
	//yuppi_star->Render();
	//test_object->Render();
	//rigid_sphere->Render();
	for (auto& a : ball_list)
	{
		a->Render();
	}
	//sphere->Render();
	cube->Render();
}