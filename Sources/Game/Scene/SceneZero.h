#pragma once

#include "game\scene\Scene.h"
//#include "game\scene\SceneTitle.h"
#include "game\GameBrain.h"
//#include "Game\YQCamera.h"
#include "system\Object\3d\StaticMesh.h"
#include "system\Object\3d\SkeltalMesh.h"
#include "System\Object\3D\WirePrimitive.h"
#include "Game\Characters\Yuppi.h"
#include "Game\Physics\RigidBody.h"
#include "Game\Actor\Ball.h"
//class ViewerCamera;
class Ball;
class SceneZero :public Scene
{
public:
	SceneZero()
	{
		grid = std::make_unique<Grid>();
		box = std::make_shared<BOX>();
		//dynamic_pointer

		std::dynamic_pointer_cast<PrimitiveBase>(box);
		RESOURCEM->AddToResourceMap(std::string("BOX"), std::dynamic_pointer_cast<PrimitiveBase>(box));
		//yuppi = std::make_unique<StaticMesh>("..\\Assets\\3D\\3d-model.fbx");
		//yuppi_star = std::make_unique<Yuppi>();
		//rigid=std::make_unique<RigidBody_Box>(std::dynamic_pointer_cast<Pawn>(std::make_shared<Yuppi>()),Vector(50.0f,100.0f,50.0f),0.01f);
		//test_object = std::make_unique<TestObject>();
		//rigid_sphere=std::make_unique<RigidBody_Sphere>(std::dynamic_pointer_cast<Pawn>(std::make_shared<Yuppi>()),50 ,1);
		//test_object->SetParent(yuppi_star.get());
		catcher_view = std::make_unique<Yukitter::YQCamera>();

	}
	~SceneZero() {}
private:
	//std::unique_ptr<SkeltalMesh> yuppi;
	//std::unique_ptr<Yuppi>					yuppi_star;
	std::unique_ptr<TestObject>				test_object;
	//std::unique_ptr<RigidBody_Box>				rigid;
	//std::unique_ptr<RigidBody_Sphere>			rigid_sphere;
	std::unique_ptr<StaticMesh>				yuppi;
	std::unique_ptr<Pawn>				cube;
	std::unique_ptr<Ball>				sphere;
	std::unique_ptr<Yukitter::YQCamera>			catcher_view;

	std::vector<std::unique_ptr<Ball>> ball_list;
	std::unique_ptr<Yukitter::ViewerCamera>	camera;
	std::unique_ptr<Grid>					grid;
	std::shared_ptr<BOX>					box;
public:
	int balls=0;
	bool camera_mode=false;
	void Initialize();

	void ChangeView();

	void Tick(float dt)override;

	void Render(float dt);
};
