#pragma once

#include "Game\Actor\Pawn.h"
#include <string>
#include "Game\Physics\RigidBody.h"
#include "System\Object\3D\WirePrimitive.h"
struct BallParameters
{
	std::string name;
	float r;
	float mass;
	float restitution;
	std::string path;
};

const BallParameters BALL_PARAMS[] =
{
	{"baseball_npb2015-",(0.0729f + 0.0748f) / 2.0f / 2.0f,(141.7f + 148.8f) / 2.0f,0.4134f ,"..\\Assets\\3D\\BaseBall\\ball.fbx"},
	{"soccer_ball",0.22f / 2.0f,(410.0f + 450.0f) / 2.0f,0.4134f ,"..\\Assets\\3D\\Soccer\\ball.fbx" },
	{"table_tennis_3star",0.040f / 2.0f,2.4f,0.8134f ,"..\\Assets\\3D\\BaseBall\\ball.fbx" },
};


class Ball :public Pawn
{
public:
	Ball() :
		Pawn(BALL_PARAMS[current_ball_index].path.c_str(), CollisionBase::COL_TYPE_SPHERE)
	{
		int ball_index = 1;
		GetActorTransform().SetScale(BALL_PARAMS[current_ball_index].r);
		rigid_body->mass = BALL_PARAMS[current_ball_index].mass;
		rigid_body->restitution = BALL_PARAMS[current_ball_index].restitution;
		line = std::make_unique<LINE>();
		axis = std::make_unique<Axis>();
		axis->scale = 1;
	}
	static int current_ball_index;
	std::unique_ptr<LINE> line;
	std::unique_ptr<Axis> axis;
	void Tick(float dt)override
	{
		Pawn::Tick(dt);
		axis->UpdateAxisByAngularVelocity(rigid_body->angular_velocity);
		axis->transform.SetLocation(transform.translation);
	}

	void Render()override
	{
		Pawn::Render();

		line->Render(Yukitter::Vector(1));
		axis->Render(rigid_body->angular_velocity);
	}
};