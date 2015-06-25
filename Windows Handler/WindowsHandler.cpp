#include "WindowsHandler.h"

namespace NTEngine
{
	WindowsHandler::WindowsHandler()
	{
	}

	WindowsHandler::~WindowsHandler()
	{
	}

	HWND WindowsHandler::Initialize(HINSTANCE hInst, WNDPROC WindowProc, const std::string& WindowName, int nCmdShow, UINT Width,
		UINT Height)
	{
		WNDCLASSEX wc;

		HWND HWnd;

		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInst;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "WindowClass1";

		RegisterClassEx(&wc);

		RECT wr = { 0, 0, Width, Height };

		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		HWnd = CreateWindowEx(NULL, "WindowClass1",
			WindowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInst, NULL);

		ShowWindow(HWnd, nCmdShow);

		return HWnd;
	}


}