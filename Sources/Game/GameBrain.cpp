#include "game\GameBrain.h"
#include "game\scene\SceneZero.h"
//#include "game\scene\SceneTitle.h"
#include "System\Framework.h"


#include "System\ImGui\imgui.h"
//#include "System\ImGui\imgui_impl_win32.h"
#include "System\ImGui\imgui_impl_dx11.h"
#if _DEBUG
#pragma comment(lib,"..\\lib\\debug\\DirectXTK_d.lib")
#else
#pragma comment(lib,"..\\lib\\release\\DirectXTK_r.lib")
#endif
DirectX::XMMATRIX GameBrain::projection_mat;

Yukitter::YQCamera* GameBrain::active_camera_reference;
DirectX::XMMATRIX  GameBrain::view_mat;


void GameBrain::Update()
{
	
}

void GameBrain::Initialize(void* hwnd)
{
	if (is_initialized)return;
	default_camera = std::make_unique<Yukitter::YQCamera>();
	active_camera_reference = default_camera.get();
	scene = std::make_unique<SceneZero>();
	scene->Initialize();
	//load = std::make_unique<Sprite>(L"DATA\\2D\\yomi.png");
	//win = std::make_unique<Sprite>(L"DATA\\2D\\win.png");
	//lose = std::make_unique<Sprite>(L"DATA\\2D\\lose.png");
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Framework::device.Get(), Framework::device_context.Get());


	projection_mat = DirectX::XMMatrixPerspectiveFovLH(3.14f / 8.0f, static_cast<float>(Framework::SCREEN_WIDTH) / Framework::SCREEN_HEIGHT, 0.1f, 500.0f);
	sprite_batch = std::make_unique<DirectX::SpriteBatch>(Framework::device_context.Get());
	sprite_font = std::make_unique<DirectX::SpriteFont>(Framework::device.Get(), L"..\\Assets\\Font\\ms_gothic.spritefont");
	IMGUI_CHECKVERSION();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//io.DisplaySize.x = 1280;
	//io.DisplaySize.y = 720;
	ImGui::StyleColorsDark();
	//io.Fonts->GetTexDataAsRGBA32();
	//UpdateImGui();
	float f = 60;

	//ImGui::Render();
	is_initialized = true;
}



void GameBrain::RenderString(std::wstring str, Vector2D location)
{
	Framework::device_context->GSSetShader(nullptr, NULL, 0);
	sprite_batch->Begin();
	sprite_font->DrawString(sprite_batch.get(), str.c_str(), DirectX::XMFLOAT2(location.x, location.y));
	sprite_batch->End();
}
void GameBrain::UpdateImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void GameBrain::RenderImGui()
{
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GameBrain::ReleaseImGui()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
	
}