#include "MouseInput.h"


namespace NTEngine
{
	MouseInput* MouseInput::_instance = nullptr;

	MouseInput* MouseInput::Instance()
	{
		if (_instance == nullptr)
			_instance = new MouseInput();

		return _instance;
	}

	void MouseInput::Destroy()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	void MouseInput::Initialize(UINT ScreenWidth, UINT ScreenHeight, bool cursor, bool AllowCursorMovementsOutsideScreen /* This is currently not working if set to false for some reason*/)
	{
		m_CurrentMousePos.x = ScreenWidth / 2;
		m_CurrentMousePos.y = ScreenHeight / 2;

		m_LastMousePos.x = ScreenWidth / 2;
		m_LastMousePos.y = ScreenHeight / 2;

		SetCursorPos(m_CurrentMousePos.x, m_CurrentMousePos.y);

		ShowCursor((BOOL)cursor);

		m_MouseMoved = false;

		m_OutsideScreenMovement = AllowCursorMovementsOutsideScreen;

		m_ScreenCenterX = (USHORT)m_CurrentMousePos.x;
		m_ScreenCenterY = (USHORT)m_CurrentMousePos.y;
	}

	void MouseInput::Update()
	{
		GetCursorPos(&m_CurrentMousePos);

		if (!(m_CurrentMousePos.x == m_LastMousePos.x && m_CurrentMousePos.y == m_LastMousePos.y)) //if mouse moved
		{
			m_MouseMoved = true;
		}
		else
		{
			m_MouseMoved = false;
		}

		if (m_OutsideScreenMovement)
		{
			//bring back cursor to the center to allow screen player movements if cursor goes outside screen
			m_LastMousePos.x = (LONG)m_ScreenCenterX;
			m_LastMousePos.y = (LONG)m_ScreenCenterY;

			SetCursorPos((int)m_ScreenCenterX, (int)m_ScreenCenterY);
		}
		else
		{
			m_LastMousePos.x = m_CurrentMousePos.x;
			m_LastMousePos.y = m_CurrentMousePos.y;
		}
	}

	POINT MouseInput::GetCurrentMousePos()const
	{
		return m_CurrentMousePos;
	}

	POINT MouseInput::GetLastMousePos()const
	{
		return m_LastMousePos;
	}

	bool MouseInput::MouseMoved()const
	{
		return m_MouseMoved;
	}
}