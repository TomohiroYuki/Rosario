#include "game\GameBrain.h"
#include "game\scene\SceneZero.h"
//#include "game\scene\SceneTitle.h"
#include "System\Framework.h"
DirectX::XMMATRIX GameBrain::projection_mat;

Yukitter::YQCamera* GameBrain::active_camera_reference;
DirectX::XMMATRIX  GameBrain::view_mat;


void GameBrain::Update()
{
	
}

void GameBrain::Initialize()
{
	if (is_initialized)return;
	default_camera = std::make_unique<Yukitter::YQCamera>();
	active_camera_reference = default_camera.get();
	scene = std::make_unique<SceneZero>();
	scene->Initialize();
	//load = std::make_unique<Sprite>(L"DATA\\2D\\yomi.png");
	//win = std::make_unique<Sprite>(L"DATA\\2D\\win.png");
	//lose = std::make_unique<Sprite>(L"DATA\\2D\\lose.png");
	projection_mat = DirectX::XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 1.0f, 5000.0f);


	is_initialized = true;
}
