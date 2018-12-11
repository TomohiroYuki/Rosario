#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "sound\sound.h"
#include "input\Input.h"
#include "Game\GameBrain.h"
#pragma comment(lib,"d3d11.lib")
#include "System\ImGui\imgui.h"
#include "System\ImGui\imgui_impl_win32.h"
#include "System\ImGui\imgui_impl_dx11.h"


class Framework final
{
public:
	Framework(HWND& hwnd) :
		hwnd(hwnd)
	{}
private:
	const HWND hwnd;
public:
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;

	static Microsoft::WRL::ComPtr<ID3D11Device>				device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		device_context;
	static Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>			depth_stencil_view;

	static float init_velocity;
	static float init_angle[3];
	static bool  is_soccerball;
	static bool  is_slowmotion;
	static bool  reset;
	static bool  is_active_air_drag;
	static float col[3];
	void DrawGrid()
	{
		//device_context->draw



	}



	bool MainLoop()
	{
		MSG msg = {};

		if (!Initialize()) return false;
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{

				Tick();
				Render();
			}
		}
		swapchain->SetFullscreenState(false, NULL);

		return static_cast<int>(msg.wParam);
	}


	bool Initialize()
	{
		// デバイスとスワップチェーンの作成
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = SCREEN_WIDTH;
		sd.BufferDesc.Height = SCREEN_HEIGHT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		D3D_FEATURE_LEVEL  feature_level = D3D_FEATURE_LEVEL_11_0;

		UINT create_device_flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, create_device_flags,
			&feature_level, 1, D3D11_SDK_VERSION, &sd, swapchain.GetAddressOf(), device.GetAddressOf(), NULL, device_context.GetAddressOf())))
		{
			return 0;
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D>		back_buffer;
		HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)back_buffer.GetAddressOf());

		if (FAILED(hr))
			return 0;

		hr = device->CreateRenderTargetView(back_buffer.Get(), NULL, render_target_view.GetAddressOf());

		if (FAILED(hr))
			return 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>		depth_stencil;

		//深度/ステンシルテクスチャの作成
		D3D11_TEXTURE2D_DESC dep_desc;
		dep_desc.Width = Framework::SCREEN_WIDTH;
		dep_desc.Height = Framework::SCREEN_HEIGHT;
		dep_desc.MipLevels = 1;
		dep_desc.ArraySize = 1;
		dep_desc.Format = DXGI_FORMAT_D32_FLOAT;
		dep_desc.SampleDesc.Count = 1;
		dep_desc.SampleDesc.Quality = 0;
		dep_desc.Usage = D3D11_USAGE_DEFAULT;
		dep_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dep_desc.CPUAccessFlags = 0;
		dep_desc.MiscFlags = 0;

		hr = device->CreateTexture2D(&dep_desc, NULL, depth_stencil.GetAddressOf());
		if (FAILED(hr))return false;

		//深度/ステンシルビューの作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dep_ste_desc;
		dep_ste_desc.Format = dep_desc.Format;
		dep_ste_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dep_ste_desc.Flags = 0;
		dep_ste_desc.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(depth_stencil.Get(), &dep_ste_desc, depth_stencil_view.GetAddressOf());

		InputBrain::getInstance()->Initialize(hwnd);

		SoundBrain::getInstance()->Initialize(hwnd);

		RESOURCEM->Initialize();
		GameBrain::GetInstance()->Initialize(hwnd);
		init_velocity = 100;
		is_soccerball = false;
		is_slowmotion = false;
		for (int i = 0; i < 3; i++)
		{
			//col[i] = 0;
			init_angle[i] = 0;
		}
		col[0] = 1.0f;
		col[1] = 0.3f;
		col[2] = 0.3f;
		reset = false;
		return true;
	}

	bool Tick()
	{
		GameBrain::GetInstance()->UpdateImGui();
		InputBrain::getInstance()->Tick();

		
		/////////
		static bool isOpen = true;
		//ImGui::ShowDemoWindow(&isOpen);

		ImGui::Begin("Stats", nullptr);
		float f = 60.0f;
		ImGui::LabelText("", "FPS:%4.2f (%4.2f ms)", f);
		//ImGui::LabelText("", "Camera Pos:(%.2f, %.2f, %.2f )", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		ImGui::Spacing();
		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::CollapsingHeader("5Key"))
		{
			ImGui::Checkbox("soccer ball", &is_soccerball);
			ImGui::Checkbox("slow motion", &is_slowmotion);
			ImGui::Checkbox("Air Drag", &is_active_air_drag);
			//ImGui::SliderFloat("Gamma", 0, 0.0f, 4.0f);
			// ...
			ImGui::SliderFloat("Init Velocity", &init_velocity, 0, 200);
			ImGui::SliderFloat3("Init Rot Per Sec", init_angle, -100, 100);
			ImGui::ColorEdit3("Line Color",col );

			// ...
			//ImGui::TreePop();
		}
		if (ImGui::Button("Variables Reset"))
		{
			init_velocity = 100;
			for (int i = 0; i < 3; i++)
			{
				init_angle[i] = 0;
			}
		}

		if (ImGui::Button("Line Reset"))
		{
			reset = true;
		}
		//...
		ImGui::End();
		static bool i = is_slowmotion;

		if (i != is_slowmotion)
		{
			//is_slowmotion = !is_slowmotion;
		}
		if (!is_slowmotion)
			GameBrain::GetInstance()->scene->Tick(1.0f / 60.0f);
		else
			GameBrain::GetInstance()->scene->Tick(1.0f / 300.0f);
		i = is_slowmotion;
		/////////

		return true;
	}

	void Render()
	{

		D3D11_VIEWPORT vp;
		vp.Width = SCREEN_WIDTH;
		vp.Height = SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		device_context->RSSetViewports(1, &vp);
		float ClearColor[4] = { 0.2f,0.2f,0.2f,1.0f }; //消去色
		device_context->ClearRenderTargetView(render_target_view.Get(), ClearColor);
		device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
		GameBrain::GetInstance()->scene->Render(1.0f / 60.0f);
		GameBrain::GetInstance()->RenderImGui();
		//ImGui::EndFrame();
		//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swapchain->Present(1, 0);
	}

};