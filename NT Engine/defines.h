#ifndef _DEFINES_A_H_
#define _DEFINES_A_H_


#include <xnamath.h>
#include <iostream>
#include <sys/stat.h>

#ifdef _DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif


#define ReleaseCOM(x) { if (x) { x->Release(); x = nullptr;  }  }
#define SafeDelete(x) { if (x) { delete x; x = nullptr; } }

#define SINGLE_THREADED


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
		{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
				{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, #x, true); \
				}                                                      \
		}
#endif

#else

#ifndef HR
#define HR(x) (x)
#endif

#endif 

static void ShowError(LPCSTR error_message)
{
	MessageBox(0, error_message, "Error", MB_OK | MB_ICONERROR);
	exit(EXIT_FAILURE);

}


#if defined(DEBUG) | defined(_DEBUG)

static void OutputDebugString_Float(float f)
{
	char OutputString[256];
	sprintf_s(OutputString, "%f", f);
	OutputDebugStringA(OutputString);
}

static std::string removeExtension(std::string filename)
{
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

static inline bool FileExists(const std::string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

static void OutputDebugString_Int(int i)
{
	char OutputString[256];
	sprintf_s(OutputString, "%d", i);
	OutputDebugStringA(OutputString);
}

#endif //DEBUG

namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}




#endif //_DEFINES_H_