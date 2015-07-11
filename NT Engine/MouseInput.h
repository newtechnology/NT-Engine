#ifndef _MOUSE_INPUT_H_
#define _MOUSE_INPUT_H_

#include "BasicIncludes.h"

//THIS MOUSEINPUT CLASS IS LITTLE BUGGY//

namespace NTEngine
{
	class MouseInput
	{
	public:
		//this function needs to be re-called if screen is re-sized
		//if AllowCursorMovementsOutsideScreen is set true, then the cursor would be brought back to the center of the screen every frame
		//this is useful if user is using mouse input for player's look movements
		DLLEXPORT void Initialize(UINT ScreenWidth, UINT ScreenHeight, bool ShowCursor, bool AllowCursorMovementsOutsideScreen);

		//releases memory
		DLLEXPORT void Destroy();

		//call this every frame
		DLLEXPORT void Update();

		DLLEXPORT inline POINT GetCurrentMousePos()const;
		DLLEXPORT inline POINT GetLastMousePos()const;

		DLLEXPORT inline bool MouseMoved()const;

		DLLEXPORT static MouseInput* Instance();

	private:
		MouseInput() {};
		MouseInput(MouseInput const&) {};
		~MouseInput() {};
		void operator=(MouseInput const&) {};

		POINT m_LastMousePos;
		POINT m_CurrentMousePos;

		USHORT m_ScreenCenterX;
		USHORT m_ScreenCenterY;

		static MouseInput* _instance;

		bool m_MouseMoved;
		bool m_OutsideScreenMovement;
	};


}

#endif //_MOUSE_INPUT_H_