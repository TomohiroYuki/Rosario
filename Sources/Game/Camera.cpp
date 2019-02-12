#include "game\YQCamera.h"



void ViewerCamera::Update()
{



	l -= static_cast<float>(INPUT->GetMouseWheelDelta() / 1008.0f*6.0f);
	if (l < 0)
		l = 0.01f;


	//INPUT->GetMouseTranslationOnViewport()


	if (INPUT->KeyGet(DIK_LALT) || (INPUT->KeyGet(DIK_RALT)))
	{
	}

	if (INPUT->GetMouseLeftButton())
	{
		transform.rotation.yaw += INPUT->GetMouseTranslationOnViewport().x*-0.001f;
		transform.rotation.pitch += INPUT->GetMouseTranslationOnViewport().y*0.001f;
	}

	target = Vector();
	/*
	if (INPUT->KeyGet(DIK_A))
	{

		a += 0.01f;
	}
	if (INPUT->KeyGet(DIK_D))
	{

		a -= 0.01f;
	}
	if (INPUT->KeyGet(DIK_W))
	{

		transform.AddLocation(Vector(0, 10.1f, 0));
	}
	if (INPUT->KeyGet(DIK_S))
	{

		transform.AddLocation(Vector(0, -10.1f, 0));
	}
*/
	transform.SetLocation(Vector(l*cosf(transform.rotation.yaw), l*sinf(transform.rotation.pitch), l*sinf(transform.rotation.yaw)));


}