#pragma once
#include "math\Transform.h"
#include "math\yuki_math.h"
#include "game\scene\Scene.h"
#include "system\input\Input.h"
#include "system\Utility.h"
#include "game\Actor\Actor.h"

namespace Yukitter {

	class YQCamera
	{
	public:
		YQCamera(){}
		/*YQCamera(Vector up, Vector pos, Vector focus) :
			up(up),
			pos(pos),
			focus(focus)
		{}*/
		virtual  ~YQCamera() {}
	protected:


	public:
	
		Transform transform;


		virtual void Update()
		{

			/*if (InputBrain::getInstance()->KeyGet(DIK_W) > 0)
			{
				pos.y += 1.0f;
			}
			if (InputBrain::getInstance()->KeyGet(DIK_S) > 0)
			{
				pos.y -= 1.0f;

			}

			if (InputBrain::getInstance()->KeyGet(DIK_A) > 0)
			{
				a -= 0.01f;
			}
			if (InputBrain::getInstance()->KeyGet(DIK_D) > 0)
			{
				a += 0.01f;
			}

			l-=static_cast<float>(InputBrain::getInstance()->GetMouseWheelDelta()/8.0f);
			if (l < 0)
				l = 0;
			this->pos.x = l*sinf(a);
			this->pos.z = l*cosf(a);*/

			//pos.x=
		}

	};




	//Viewer Camera constant variables
	const Vector DEFAULT_CAMERA_LOCATION{ 15.00f,1.500f*6,0 };



	class ViewerCamera final : public YQCamera
	{
	public:
		ViewerCamera() :
			YQCamera()
		{
			transform.SetLocation(DEFAULT_CAMERA_LOCATION);
			transform.SetRotation(Rotator(0,0,3.14f/60.0f));
			target = Vector();
			l = 18.44f+7.0f;
		}
	private:
		float a{};
		float l{};
		Vector target;
		
		
	public:


		void Update();
	};


	//class ArmCamera :public YQCamera
	//{
	//public:
	//	ArmCamera(Actor*const target, float max_arm_length) :
	//		YQCamera(),
	//		MAX_ARM_LENGTH(max_arm_length),
	//		target_reference(target)
	//	{}
	//private:
	//	float MAX_ARM_LENGTH;
	//	Actor*  const target_reference;
	//public:

	//	void Update()
	//	{


	//		//focus = target_reference->GetActorLocation();
	//		//pos = focus+(pos - focus).Normalize()*MAX_ARM_LENGTH;

	//		focus = target_reference->GetActorLocation();
	//		pos = focus;
	//		pos.z = focus.z + MAX_ARM_LENGTH;
	//		float a = (focus - pos).Length();
	//	}

	//};



	//class Game2DCamera :public YQCamera
	//{
	//public:
	//	Game2DCamera(Actor*const target, float max_arm_length) :
	//		YQCamera(),
	//		MAX_ARM_LENGTH(max_arm_length),
	//		target_reference(target)
	//	{}
	//private:
	//	float MAX_ARM_LENGTH;
	//	Actor*  const target_reference;
	//public:

	//	void Update()
	//	{

	//		pos -= (pos - target_reference->GetActorLocation()) / 10.0f;
	//		pos.x -= 0.75f;
	//		focus = pos;

	//		pos.z = focus.z + MAX_ARM_LENGTH;
	//		pos.x += 7;
	//	}

	//};



}