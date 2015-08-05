#ifndef _DEFINES_A_H_
#define _DEFINES_A_H_


#include <DirectXMath.h>
#include "DDSTextureLoader.h"
#include <DirectXColors.h>
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

#ifndef HR
#define HR(x)									                \
		{									                    \
		HRESULT hr = (x);						                \
		if(FAILED(hr))							                \
				{								                \
			LPSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		    \
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		        \
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			        \
				NULL,						                    \
				hr,						                        \
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	    \
				(LPTSTR) &output,				                \
				0,						                        \
				NULL);					                        \
			MessageBox(NULL, output, "Error", MB_OK);		    \
				}								                \
		}
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

static void OutputDebugString_Int(int i)
{
	char OutputString[256];
	sprintf_s(OutputString, "%d", i);
	OutputDebugStringA(OutputString);
}

#endif //DEBUG


static inline std::string removeExtension(std::string filename)
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




#endif //_DEFINES_H_