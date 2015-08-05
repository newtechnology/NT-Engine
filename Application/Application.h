#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "BasicIncludes.h"


#include "../Windows Handler/WindowsHandler.h"
#include "../NT Engine/Renderer.h"
#include "../NT Engine/MouseInput.h"
#include "../NT Engine/Timer.h"
#include "../NT Engine/Math.h"


#if defined(DEBUG) | defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define WINDOW_NAME "NT Engine"

#endif