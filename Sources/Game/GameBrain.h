#pragma once
#include "game\YQCamera.h"
//#include "source\system\object\sprite\sprite.h"
#include "System\DirectXTK\SpriteFont.h"
#include "game\scene\Scene.h"

#include <memory>

using namespace Yukitter;

class GameBrain final
{
	// Singleton
	GameBrain() = default;
	~GameBrain() = default;
public:
	// Singleton
	GameBrain(const GameBrain&) = delete;
	GameBrain& operator=(const GameBrain&) = delete;
	GameBrain(GameBrain&&) = delete;
	GameBrain& operator=(GameBrain&&) = delete;
public:
	
private:
	static Yukitter::YQCamera* active_camera_reference;
	std::unique_ptr<Yukitter::YQCamera> default_camera;
	std::unique_ptr<DirectX::SpriteBatch> sprite_batch;
	std::unique_ptr<DirectX::SpriteFont> sprite_font;
	//std::list<std::unique_ptr<Actor>> geometry_collection;
public:
	std::unique_ptr<Scene> scene;
	int timer = 0;
	void Initialize(void* hwnd);


	enum GAME_STATE
	{
		PLAYING,
		WIN,
		LOSE
	}game_state= PLAYING;
	
	bool is_initialized = 0;
	static GameBrain* GetInstance()
	{
		static GameBrain inst;
		return &inst;
	}
	void ChangeScene(Scene* new_scene)
	{
		scene.reset(new_scene);
		scene->Initialize();
	}

	void Update();

	static void ActivateCamera(Yukitter::YQCamera* other_camera)
	{
		active_camera_reference = nullptr;
		active_camera_reference = other_camera;
	}
	void BackToDefaultCamera()
	{
		active_camera_reference = nullptr;
		active_camera_reference = default_camera.get();
	}


	static Vector GetActiveCameraLocation()
	{
		return active_camera_reference->transform.translation;
	}
	static Vector GetActiveCameraFocus()
	{
		//TODO!!!: FIX
		return Vector(0,0,0);
	}


	static DirectX::XMMATRIX view_mat;
	static DirectX::XMMATRIX projection_mat;


	void LoadingRender()
	{
		//load->Render({}, {1280,720}, {}, {1280,720}, {},0);
	}

	static Yukitter::YQCamera* GetActiveCameraReference()
	{
		return active_camera_reference;
	}
	void RenderString(std::wstring str, Vector2D location);


	void UpdateImGui();
	void RenderImGui();

	void ReleaseImGui();
};