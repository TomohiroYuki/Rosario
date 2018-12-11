
#include <Windows.h>
#include <crtdbg.h>

#include "System\Framework.h"


float Framework::init_velocity;
float Framework::init_angle[3];
float Framework::col[3];
bool  Framework::is_soccerball;
bool  Framework::is_slowmotion;
bool  Framework::reset;
bool  Framework::is_active_air_drag = true;
//_____________________________________________________________________________
Microsoft::WRL::ComPtr<ID3D11Device>			Framework::device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext>		Framework::device_context;
Microsoft::WRL::ComPtr<IDXGISwapChain>			Framework::swapchain;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	Framework::render_target_view;
//______________________________________________________________________________

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE)
		{
			PostMessage(hwnd, WM_CLOSE, 0, 0);
		}
		return 0;
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(91396);
#endif
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Yuppi Star Is You !";
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, Framework::SCREEN_WIDTH, Framework::SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(L"Yuppi Star Is You !", L"Yuppi Star Is You !", WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	ShowWindow(hwnd, cmd_show);

	Framework framework(hwnd);

	framework.MainLoop();
	GameBrain::GetInstance()->ReleaseImGui();
	return 0;
}
