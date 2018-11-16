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
};

const BallParameters BALL_PARAMS[] =
{
	{"baseball_npb2015-",(0.0729f + 0.0748f) / 2.0f / 2.0f,(141.7f + 148.8f) / 2.0f,0.4134f }
};


class Ball :public Pawn
{
public:
	Ball() :
		Pawn("..\\Assets\\3D\\ball.fbx", CollisionBase::COL_TYPE_SPHERE)
	{

		GetActorTransform().SetScale(BALL_PARAMS[0].r);
		rigid_body->mass = BALL_PARAMS[0].mass;
		rigid_body->restitution = BALL_PARAMS[0].restitution;
		line = std::make_unique<LINE>();
		
	}

	std::unique_ptr<LINE> line;

	void Tick(float dt)override
	{
		Pawn::Tick(dt);


	}

	void Render()override
	{
		Pawn::Render();
		
		line->Render(Yukitter::Vector(1));
	}
};