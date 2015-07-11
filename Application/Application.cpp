#include "Application.h"
#include "../Windows Handler/WindowsHandler.h"
#include "../NT Engine/Renderer.h"
#include "../NT Engine/MouseInput.h"
#include "../NT Engine/TriangleDemo.h"
#include "../NT Engine/Timer.h"
#include "../NT Engine/Model.h"


#define WINDOW_NAME "NT Engine"


#if defined(DEBUG) | defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR CmdLine, int nCmdShow)
{
	//dump memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	NTEngine::WindowsHandler Handler;
	NTEngine::GameTimer Timer;

	Timer.Reset();

	HWND HWnd = Handler.Initialize(hInstance, WindowProc, WINDOW_NAME, nCmdShow, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT);


	NTEngine::Renderer renderer;

	NTEngine::MouseInput::Instance()->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, false, true);

	renderer.Initialize(HWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	NTEngine::Model* Sponza = new NTEngine::Model();

	XMMATRIX W = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	Sponza->LoadFromFile(renderer._GetDevice(), "Assets\\sponza.obj");

	renderer.AddModel(Sponza, W);

	TriangleDemo::Initialize(renderer._GetDevice(), renderer);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Timer.Tick();
			 
			NTEngine::MouseInput::Instance()->Update();

			TriangleDemo::Update(Timer.DeltaTime(), renderer);


#if defined(DEBUG) | defined(_DEBUG) 
			renderer.CalculateFrameStats(WINDOW_NAME, Timer, HWnd);
#endif

			renderer.Draw();

		}
	}

	NTEngine::MouseInput::Instance()->Destroy();

	renderer.Destroy();

	return (BOOL)msg.wParam;
}



LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}