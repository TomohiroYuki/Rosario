#pragma once
#include "Game\Actor\Pawn.h"
#include "Game\Physics\RigidBody.h"
class Yuppi :public Pawn
{
public:
private:
public:
	Yuppi() :
		Pawn("..\\Assets\\3D\\yuppi_wait.fbx")
	{

	}


	void Tick(float dt) override
	{
		//transform.AddLocation(Vector(0, 0, 0.001f));
		//transform.AddRotation(Rotator(0.01f, 0, 0));
	}
};


class TestObject :public Pawn
{
public:
private:
public:
	TestObject() :
		Pawn("..\\Assets\\3D\\totsu.fbx")
	{
		transform.SetScale(Vector(10, 10, 10));
		transform.SetLocation(Vector(0, 50, -50));
		//SetParent()
	}

	void Tick(float dt) override
	{
		
	}
};