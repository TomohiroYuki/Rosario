#include "game\scene\SceneZero.h"
#include "Game\YQCamera.h"
#include "Game\Collision\Collision.h"
#include "Game\Actor\Ball.h"
#include "System\Utility.h"
#include "System\Framework.h"
int Ball::current_ball_index = 0;
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
	sphere->RotateOrientation({ 0,1,0 }, 3.141592f / 2.0f);
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

	if (Framework::reset)
	{
		Framework::reset = false;
		ball_list.clear();
		balls = 0;
	}

	static bool before_ball = Framework::is_soccerball;
	if (Framework::is_soccerball != before_ball)
	{
		Ball::current_ball_index = I_CAST(!B_CAST(Ball::current_ball_index));

	}
	before_ball = Framework::is_soccerball;
	//current_ball_index
	if (bb)return;

	//TODO::”xä
	if (INPUT->KeyGet(DIK_3) == 3)
	{

		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(0, 1, 1, 1);
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->RotateOrientation({ 0,1,0 }, 3.141592f / 2.0f);
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 0, 0));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(150 * 1000 / 60.0f / 60.0f, -1.5f, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (INPUT->KeyGet(DIK_2) == 3)
	{

		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(0, 1, 0, 1);
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->RotateOrientation({ 0,1,0 }, 3.141592f / 2.0f);
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 25, 6));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(120 * 1000 / 60.0f / 60.0f, -0.3f, 0.3f));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (INPUT->KeyGet(DIK_1) == 3)
	{
		//ball_list.clear();
		//balls = 0;
		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(1, 0, 0, 1);
		ball_list[balls];
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 0, 44));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(150 * 1000 / 60.0f / 60.0f, -1.5f, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (INPUT->KeyGet(DIK_4) == 3)
	{
		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->color = Yukitter::Vector4(0, 0, 1, 1);
		ball_list[balls]->line->Clear();
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.1f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 22, -16));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(100 * 1000 / 60.0f / 60.0f, 3.9f, 0.35f));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}

	if (INPUT->KeyGet(DIK_5) == 3)
	{

		b = true;
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(Framework::col[0], Framework::col[1], Framework::col[2], 1);
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 1.8f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->RotateOrientation({ 0,1,0 }, 3.141592f / 2.0f);
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(Framework::init_angle[0], Framework::init_angle[1], Framework::init_angle[2]));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(Framework::init_velocity * 1000 / 60.0f / 60.0f, -1.5f, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	if (!b)return;
	//sphere->line->AddPoint(sphere->GetActorLocation());

	const int solv = 5;
	//sphere->rigid_body->AddMagnusForce();

	static bool aaaa = true;
	if (INPUT->KeyGet(DIK_G) == 3)
	{
		aaaa = !aaaa;
	}
	//if()

	for (auto& a : ball_list)
	{
		a->line->AddPoint(a->GetActorLocation());
		a->rigid_body->AddMagnusForce();;
		if (Framework::is_active_air_drag)
			a->rigid_body->AddDragForce();
	}
	for (int i = 0; i < solv; i++)
	{
		//sphere->rigid_body->Tick(dt, solv);

		for (auto& a : ball_list)
		{
			a->rigid_body->Tick(dt, solv);
		}

		cube->Tick(dt);

		for (auto& a : ball_list)
		{
			Collision::Collide(dynamic_cast<SphereCollision*>(a->collision.get()), dynamic_cast<BoxCollision*>(cube->collision.get()));
		}
		//Collision::Collide(dynamic_cast<SphereCollision*>(sphere->collision.get()), dynamic_cast<BoxCollision*>(cube->collision.get()));

		Collision::Tick();


	}
	cube->rigid_body->ResetAccumrator();
	//sphere->rigid_body->ResetAccumrator();
	for (auto& a : ball_list)
	{
		a->rigid_body->ResetAccumrator();
		a->Tick(1.0f / 60.0f);
	}

}
void SceneZero::Render(float dt)
{
	grid->Render(Vector(0.1f));

	if (INPUT->KeyGet(DIK_V) == 3)
	{
		ChangeView();
	}
	for (auto& a : ball_list)
	{
		a->Render();
	}
	//sphere->Render();
	cube->Render();

	//GameBrain::GetInstance()->RenderString(L"1~3Key:Throw Breaking Ball", { 0,0 });
	//GameBrain::GetInstance()->RenderString(L"0Key:Change Ball(baseball,soccer)", { 0,30 });
	//GameBrain::GetInstance()->RenderString(L"i Key:Slow Motion On/Off ", { 0,180 });
	//GameBrain::GetInstance()->RenderString(L"o Key:Pause On/Off ", { 0,150 });
	//GameBrain::GetInstance()->RenderString(L"v Key:Change View", { 0,120 });
}

void SceneZero::ChangeView()
{
	camera_mode = !camera_mode;

	if (camera_mode)
	{
		catcher_view->transform.SetLocation({ 18.44f + 1.0f,0.8f,0 });
		GameBrain::ActivateCamera(catcher_view.get());
	}
	else
	{
		GameBrain::ActivateCamera(camera.get());
	}
}