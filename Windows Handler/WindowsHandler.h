#ifndef _WINDOWS_HANDLER_H_
#define _WINDOWS_HANDLER_H_

#include "BasicIncludes.h"
#include <string>

namespace NTEngine
{
	class WindowsHandler
	{
	public:
		DLLEXPORT WindowsHandler();
		DLLEXPORT ~WindowsHandler();
	
		HWND DLLEXPORT Initialize(HINSTANCE hInst, WNDPROC WindowProc, const std::string& WindowName, int nCmdShow, UINT Width,
			UINT Height);

	
	};
}


#endif