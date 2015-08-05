#include "Application.h"

using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


VOID ManageCameraInput(NTEngine::Camera* Cam, float& dt);


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
	

	//============== Initialize Engine ============//

	renderer.EnableMSAA(4);
	renderer.Initialize(HWnd, SCREEN_WIDTH, SCREEN_HEIGHT);


	NTEngine::Camera* Cam = new NTEngine::Camera();
	
	Cam->SetPosition(0.0f, 0.0f, -5.0f);
	Cam->SetLens(XMConvertToRadians(60.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 1.0f, 1000.0f);
	Cam->UpdateViewMatrix(); 

	renderer.SetCamera(Cam);


	//===== Load Models From File =================//

	ID3D11Device* device = renderer._GetDevice();

	NTEngine::BasicModel* Sponza = new NTEngine::BasicModel();
	NTEngine::NormalMappedModel* Cube = new NTEngine::NormalMappedModel();


	XMMATRIX W = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	Sponza->LoadFromFile(device, "Assets\\sponza.obj");

	renderer.AddBasicModel(Sponza, W); 

	W = XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(0.0f, 10.0f, 25.0f);
	Cube->LoadFromFile(device, "Assets\\cube.obj");

	renderer.AddNormalMappedModel(Cube, W);

	//================ Add Lights ====================//


	NTEngine::Lights::DirectionalLight light;

	light.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	light.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	light.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	light.Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);

	NTEngine::Lights::DirectionalLight light2;

	light2.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	light2.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	light2.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	light2.Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);

	renderer.AddDirectionalLight(light);
	renderer.AddDirectionalLight(light2);

	//================================================//



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
			
			float dt = Timer.DeltaTime();

			ManageCameraInput(Cam, dt);


#if defined(DEBUG) | defined(_DEBUG) 
			renderer.CalculateFrameStats(WINDOW_NAME, Timer, HWnd);
#endif

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



void ManageCameraInput(NTEngine::Camera* Cam, float& dt)
{
	bool CameraUpdated = false;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Cam->Walk(50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		Cam->Walk(-50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		Cam->Strafe(-50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		Cam->Strafe(50.0f * dt);
		CameraUpdated = true;
	}

	if (NTEngine::MouseInput::Instance()->MouseMoved())
	{
		POINT CurrentMousePos = NTEngine::MouseInput::Instance()->GetCurrentMousePos();
		POINT LastMousePos = NTEngine::MouseInput::Instance()->GetLastMousePos();


		float ScaleFactor = 0.35f;

		float dx = ScaleFactor * static_cast<float>(CurrentMousePos.x - LastMousePos.x);
		dx = XMConvertToRadians(dx); //convert to radians

		float dy = ScaleFactor * static_cast<float>(CurrentMousePos.y - LastMousePos.y);
		dy = XMConvertToRadians(dy); //convert to radians

		Cam->RotateY(dx);
		Cam->Pitch(dy);

		CameraUpdated = true;
	}

	//only update viewmatrix if required
	//save cpu cycles
	if (CameraUpdated)
		Cam->UpdateViewMatrix();
}
