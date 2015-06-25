#include "Application.h"
#include "../Windows Handler/WindowsHandler.h"
#include "../NT Engine/Renderer.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR CmdLine, int nCmdShow)
{
	NTEngine::WindowsHandler Handler;

	HWND HWnd = Handler.Initialize(hInstance, WindowProc, "Application", nCmdShow, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT);


	NTEngine::Renderer renderer;


	renderer.Initialize(HWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

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
			renderer.Draw();
		}
	}

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