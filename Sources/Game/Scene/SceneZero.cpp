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


	static bool before_ball = Framework::is_soccerball;
	if (Framework::is_soccerball != before_ball)
	{
		Ball::current_ball_index = I_CAST(!B_CAST(Ball::current_ball_index));

	}
	before_ball = Framework::is_soccerball;

	//TODO::�x��

	//sphere->line->AddPoint(sphere->GetActorLocation());


	if (INPUT->KeyGet(DIK_RSHIFT))
	{
		cube->RotateOrientation({ 0,0,1 }, 0.001f);
	}
	if (INPUT->KeyGet(DIK_LSHIFT))
	{
		cube->RotateOrientation({ 0,0,-1 }, 0.001f);
	}
	const int solv = 5;
	//sphere->rigid_body->AddMagnusForce();

	
	if (INPUT->KeyGet(DIK_SPACE) == 3)
	{
		auto a_cube = std::make_unique<Pawn>("..\\Assets\\StandardAssets\\cube.fbx", CollisionBase::COLLISION_TYPE::COL_TYPE_BOX);
		a_cube->GetActorTransform().SetLocation({ 0,1,0 });
		a_cube->GetActorTransform().SetScale(0.2f);
		a_cube->SetIsSimulated(true);
		cubes.emplace_back(std::move(a_cube));
	
	}
	if (INPUT->KeyGet(DIK_3) == 3)
	{
		ball_list.emplace_back(std::make_unique<Ball>());
		ball_list[balls]->line->Clear();
		ball_list[balls]->line->color = Yukitter::Vector4(0, 1, 1, 1);
		ball_list[balls]->GetActorTransform().SetLocation(Yukitter::Vector(0, 0.2f, 0));
		ball_list[balls]->collision->CalcInertiaTensor();
		ball_list[balls]->RotateOrientation({ 0,1,0 }, 3.141592f / 2.0f);
		ball_list[balls]->rigid_body->linear_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->angular_velocity = Yukitter::Vector();
		ball_list[balls]->rigid_body->AddInitialTorque(Yukitter::Vector(0, 1*2*3.14f,0 ));

		ball_list[balls]->rigid_body->AddInitialVelocity(Yukitter::Vector(0 * 1000 / 60.0f / 60.0f, 0, 0));
		ball_list[balls]->SetIsSimulated(true);
		balls++;
	}
	for (auto& a : ball_list)
	{
		a->line->AddPoint(a->GetActorLocation());
		a->rigid_body->AddMagnusForce();;
		if (Framework::is_active_air_drag)
		{
			a->rigid_body->AddDragForce();

		}
	}


	for (auto& a_cube : cubes)
	{
		//a_cube->rigid_body->AddDragForce();
		a_cube->rigid_body->Tick(dt);
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
	/*for (auto& a : ball_list)
	{
		a->rigid_body->ResetAccumrator();
		a->Tick(1.0f / 60.0f);
	}*/

	for (auto& a_cube : cubes)
	{
		a_cube->rigid_body->ResetAccumrator();
	}
}
void SceneZero::Render(float dt)
{
	grid->Render(Vector(0.1f));

	/*if (INPUT->KeyGet(DIK_V) == 3)
	{
		ChangeView();
	}*/

	for (auto& a_cube : cubes)
	{
		a_cube->Render();
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