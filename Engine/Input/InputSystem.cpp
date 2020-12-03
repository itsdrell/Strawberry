#include "InputSystem.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "../Core/General/BlackBoard.hpp"

//===============================================================================================
InputSystem*		g_theInputSystem = nullptr;

//-----------------------------------------------------------------------------------------------
MouseButtons StringToMouseButtons(std::string string)
{
	if(string.length() == 0)
	{
		return NUM_OF_MOUSE_BUTTONS;
	}
	
	if(string[0] == 'l' || string[0] == 'L')
	{
		return LEFT_MOUSE_BUTTON;
	}

	if (string[0] == 'r' || string[0] == 'R')
	{
		return RIGHT_MOUSE_BUTTON;
	}

	return LEFT_MOUSE_BUTTON;
}

//-----------------------------------------------------------------------------------------------
// https://wiki.libsdl.org/SDL_Keycode
#pragma warning( disable : 4310 ) // weird warning about "cast truncates constant value" should be fine tho :o
//const KeyCode KEYBOARD_SPACE			= (const KeyCode) SDLK_SPACE;
//const KeyCode KEYBOARD_LSHIFT			= (const KeyCode) SDLK_LSHIFT;
//const KeyCode KEYBOARD_ENTER			= (const KeyCode) SDLK_RETURN;
//const KeyCode KEYBOARD_BACKSPACE		= (const KeyCode) SDLK_BACKSPACE;
//const KeyCode KEYBOARD_TILDE			= (const KeyCode) SDLK_BACKQUOTE;


//===============================================================================================
// for some reason these are weird :l if I use SDL_Left it's like 12341234134 which makes no sense
// so im using the scan codes which share a slop with capital P O R Q :l
//===============================================================================================
//const KeyCode KEYBOARD_LEFT_ARROW = (const KeyCode)SDLK_LEFT;
//const KeyCode KEYBOARD_RIGHT_ARROW = (const KeyCode)SDLK_RIGHT;
//const KeyCode KEYBOARD_UP_ARROW = (const KeyCode)SDLK_UP;
//const KeyCode KEYBOARD_DOWN_ARROW = (const KeyCode)SDLK_DOWN;

//===============================================================================================
InputSystem::InputSystem()
{
	if (g_theInputSystem != nullptr)
		delete g_theInputSystem;

	HideMouseCursor(g_theEngineBlackboard->GetValue("hideMouse", true));

	g_theInputSystem = this;
}

//-----------------------------------------------------------------------------------------------
InputSystem::~InputSystem()
{
	g_theInputSystem = nullptr;
}

//-----------------------------------------------------------------------------------------------
InputSystem * InputSystem::GetInstance()
{
	return g_theInputSystem;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::BeginFrame()
{
	UpdateKeyboard();
	UpdateMouse();
	PollEvents(); 
}

//-----------------------------------------------------------------------------------------------
void InputSystem::Update()
{
	// nothing for now
}

//-----------------------------------------------------------------------------------------------
void InputSystem::EndFrame()
{
	// nothing yet
	m_textInputed.clear();
}

//-----------------------------------------------------------------------------------------------
void InputSystem::OnKeyPressed(KeyCode keyCode)
{
	if (m_keyStates[keyCode].m_isDown == false)
		m_keyStates[keyCode].m_wasJustPressed = true;
	m_keyStates[keyCode].m_isDown = true;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::OnKeyReleased(KeyCode keyCode)
{
	if (m_keyStates[keyCode].m_isDown == true)
		m_keyStates[keyCode].m_wasJustReleased = true;
	m_keyStates[keyCode].m_isDown = false;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::IsKeyPressed(KeyCode keyCode, bool consume)
{
	bool result = m_keyStates[keyCode].m_isDown;
	
	if (consume)
		m_keyStates[keyCode].m_isDown = false;

	return result;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasKeyJustPressed(KeyCode keyCode, bool consume)
{
	bool result = m_keyStates[keyCode].m_wasJustPressed;

	if (consume)
		m_keyStates[keyCode].m_wasJustPressed = false;

	return result;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasKeyJustReleased(KeyCode keyCode, bool consume)
{
	bool result = m_keyStates[keyCode].m_wasJustReleased;

	if (consume)
		m_keyStates[keyCode].m_wasJustReleased = false;

	return result;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::OnMouseButtonPressed(MouseButtons theButton)
{
	if (m_mouseButtonStates[theButton].m_isDown == false)
		m_mouseButtonStates[theButton].m_wasJustPressed = true;
	m_mouseButtonStates[theButton].m_isDown = true;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::OnMouseButtonReleased(MouseButtons theButton)
{
	if (m_mouseButtonStates[theButton].m_isDown == true)
		m_mouseButtonStates[theButton].m_wasJustReleased = true;
	m_mouseButtonStates[theButton].m_isDown = false;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::IsMouseButtonPressed(MouseButtons theButton)
{
	return m_mouseButtonStates[theButton].m_isDown;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasMouseButtonJustPressed(MouseButtons theButton)
{
	return m_mouseButtonStates[theButton].m_wasJustPressed;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasMouseButtonJustReleased(MouseButtons theButton)
{
	return m_mouseButtonStates[theButton].m_wasJustReleased;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::DidMouseWheelScrollUp()
{
	return (m_mouseScrollWheel.y > 0);
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::DidMouseWheelScrollDown()
{
	return (m_mouseScrollWheel.y < 0);
}

//-----------------------------------------------------------------------------------------------
void InputSystem::HideMouseCursor(bool isHidden)
{
	SDL_ShowCursor(!isHidden);
}

//-----------------------------------------------------------------------------------------------
void InputSystem::ClearKeyState(KeyCode keyCode)
{
	m_keyStates[keyCode].m_wasJustReleased = false;
	m_keyStates[keyCode].m_wasJustPressed = false;
	m_keyStates[keyCode].m_isDown = false;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::ClearKeyboardState()
{
	for(uint i = 0; i < NUM_KEYS; i++)
	{
		m_keyStates[i].m_wasJustReleased = false;
		m_keyStates[i].m_wasJustPressed = false;
		m_keyStates[i].m_isDown = false;
	}

	for(uint i = 0; i < NUM_OF_MOUSE_BUTTONS; i++)
	{
		m_mouseButtonStates[i].m_wasJustReleased = false;
		m_mouseButtonStates[i].m_wasJustPressed = false;
		m_mouseButtonStates[i].m_isDown = false;
	}
}

//-----------------------------------------------------------------------------------------------
String InputSystem::GetTextInputedThisFrame()
{
	return m_textInputed;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::AddTextInputThisFrame(KeyCode keyCode)
{
	if (keyCode == KEYBOARD_SPACE)
	{
		m_textInputed += " ";
	}
	else
	{
		m_textInputed += ((unsigned char) keyCode);
	}
}

//-----------------------------------------------------------------------------------------------
void InputSystem::PollEvents()
{
	SDL_Event theEvent;
	while (SDL_PollEvent(&theEvent))
	{
		if (theEvent.type == SDL_QUIT)
		{
			// this is a callback so we don't have to include game code in engine
			if (m_shutdownFunction)
				m_shutdownFunction();
		}

		if (theEvent.type == SDL_KEYDOWN)
		{
			KeyCode theCode = LookUpKeyCodeFromSDLKeySym(theEvent.key.keysym.sym);
			OnKeyPressed(theCode);
			FilterKeysAndPassToDevConsole(theCode);
		}

		if (theEvent.type == SDL_KEYUP)
		{
			KeyCode theCode = LookUpKeyCodeFromSDLKeySym(theEvent.key.keysym.sym);
			OnKeyReleased(theCode);
		}

		if (theEvent.type == SDL_TEXTINPUT)
		{
			// just lettered keys
			AddTextInputThisFrame((KeyCode)theEvent.key.state);
			Console::GetInstance()->GetInput((KeyCode)theEvent.key.state);
			theEvent.text.text;

		} 

		if (theEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			if (theEvent.button.button == SDL_BUTTON_LEFT)
				OnMouseButtonPressed(LEFT_MOUSE_BUTTON);
			if (theEvent.button.button == SDL_BUTTON_RIGHT)
				OnMouseButtonPressed(RIGHT_MOUSE_BUTTON);
		}

		if (theEvent.type == SDL_MOUSEBUTTONUP)
		{
			if (theEvent.button.button == SDL_BUTTON_LEFT)
				OnMouseButtonReleased(LEFT_MOUSE_BUTTON);
			if (theEvent.button.button == SDL_BUTTON_RIGHT)
				OnMouseButtonReleased(RIGHT_MOUSE_BUTTON);
		}

		if (theEvent.type == SDL_MOUSEWHEEL)
		{
			m_mouseScrollWheel.y = (float) theEvent.wheel.y;
			m_mouseScrollWheel.x = (float) theEvent.wheel.x;
		}

		if (theEvent.type == SDL_MOUSEMOTION)
		{
			m_mousePosition.x = (float) theEvent.motion.x;
			m_mousePosition.y = (float) theEvent.motion.y;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void InputSystem::FilterKeysAndPassToDevConsole(int code)
{
	if (code == KEYBOARD_ENTER || code == KEYBOARD_BACKSPACE ||
		code == KEYBOARD_LEFT_ARROW || code == KEYBOARD_RIGHT_ARROW)
	{
		Console::GetInstance()->GetInput( (KeyCode) code);
	}

}

//-----------------------------------------------------------------------------------------------
void InputSystem::UpdateKeyboard()
{
	// Clear all just-changed flags, in preparation for the next round of WM_KEYDOWN, etc. messages
	for (int keyCode = 0; keyCode < InputSystem::NUM_KEYS; ++keyCode)
	{
		m_keyStates[keyCode].m_wasJustPressed = false;
		m_keyStates[keyCode].m_wasJustReleased = false;
	}
}

//-----------------------------------------------------------------------------------------------
void InputSystem::UpdateMouse()
{
	for (int i = 0; i < NUM_OF_MOUSE_BUTTONS; i++)
	{
		m_mouseButtonStates[i].m_wasJustPressed = false;
		m_mouseButtonStates[i].m_wasJustReleased = false;
	}

	m_mouseScrollWheel = Vector2(0.f, 0.f);
}

//-----------------------------------------------------------------------------------------------
KeyCode InputSystem::CheckAndCorrectSDLArrowKeys(KeyCode code)
{
	if (code > 256)
	{
		if (code == SDLK_LEFT)
			return KEYBOARD_LEFT_ARROW;
		if (code == SDLK_RIGHT)
			return KEYBOARD_RIGHT_ARROW;
		if (code == SDLK_UP)
			return KEYBOARD_UP_ARROW;
		if (code == SDLK_DOWN)
			return KEYBOARD_DOWN_ARROW;
	}

	return code;
}

//-----------------------------------------------------------------------------------------------
KeyCode InputSystem::LookUpKeyCodeFromSDLKeySym(KeyCode code)
{
	// early out for most letter keys
	if (code < 256)
		return code;

	if (code == SDLK_SPACE)
		return KEYBOARD_SPACE;
	if (code == SDLK_LSHIFT)
		return KEYBOARD_LSHIFT;
	if (code == SDLK_RETURN)
		return KEYBOARD_ENTER;
	if (code == SDLK_BACKSPACE)
		return KEYBOARD_BACKSPACE;
	if (code == SDLK_BACKQUOTE)
		return KEYBOARD_TILDE;

	if (code == SDLK_LEFT)
		return KEYBOARD_LEFT_ARROW;
	if (code == SDLK_RIGHT)
		return KEYBOARD_RIGHT_ARROW;
	if (code == SDLK_UP)
		return KEYBOARD_UP_ARROW;
	if (code == SDLK_DOWN)
		return KEYBOARD_DOWN_ARROW;

	if (code == SDLK_F1)
		return KEYBOARD_F1;
	if (code == SDLK_F2)
		return KEYBOARD_F2;
	if (code == SDLK_F3)
		return KEYBOARD_F3;
	if (code == SDLK_F4)
		return KEYBOARD_F4;
	if (code == SDLK_F5)
		return KEYBOARD_F5;
	if (code == SDLK_F6)
		return KEYBOARD_F6;
	if (code == SDLK_F7)
		return KEYBOARD_F7;
	if (code == SDLK_F8)
		return KEYBOARD_F8;
	if (code == SDLK_F9)
		return KEYBOARD_F9;
	if (code == SDLK_F10)
		return KEYBOARD_F10;
	if (code == SDLK_F11)
		return KEYBOARD_F11;
	if (code == SDLK_F12)
		return KEYBOARD_F12;

	if (code == SDLK_LALT)
		return KEYBOARD_ALT;
	if (code == SDLK_LCTRL)
		return KEYBOARD_CTRL;

	return 0;
}

//===============================================================================================
bool IsKeyPressed(KeyCode keyCode, bool consume)
{
	return InputSystem::GetInstance()->IsKeyPressed(keyCode, consume);
}

//-----------------------------------------------------------------------------------------------
bool WasKeyJustPressed(KeyCode keyCode, bool consume)
{
	return InputSystem::GetInstance()->WasKeyJustPressed(keyCode, consume);
}

//-----------------------------------------------------------------------------------------------
bool WasKeyJustReleased(KeyCode keyCode, bool consume)
{
	return InputSystem::GetInstance()->WasKeyJustReleased(keyCode, consume);
}

//-----------------------------------------------------------------------------------------------
bool IsMouseButtonPressed(MouseButtons theButton)
{
	return InputSystem::GetInstance()->IsMouseButtonPressed(theButton);
}

//-----------------------------------------------------------------------------------------------
bool WasMouseButtonJustPressed(MouseButtons theButton)
{
	return 	InputSystem::GetInstance()->WasMouseButtonJustPressed(theButton);
}

//-----------------------------------------------------------------------------------------------
bool WasMouseButtonJustReleased(MouseButtons theButton)
{
	return InputSystem::GetInstance()->WasMouseButtonJustReleased(theButton);
}

//-----------------------------------------------------------------------------------------------
bool DidMouseWheelScrollUp()
{
	return InputSystem::GetInstance()->DidMouseWheelScrollUp();
}

//-----------------------------------------------------------------------------------------------
bool DidMouseWheelScrollDown()
{
	return InputSystem::GetInstance()->DidMouseWheelScrollDown();
}

//-----------------------------------------------------------------------------------------------
Vector2 GetMousePosition(const AABB2& orthoBounds)
{
	Vector2 windowDimensions = Window::GetInstance()->GetDimensions();
	Vector2 mousePositionInPixels = InputSystem::GetInstance()->GetMousePositionInPixels();

	float x = RangeMapFloat(mousePositionInPixels.x, 0.f, windowDimensions.x, orthoBounds.mins.x, orthoBounds.maxs.x);

	// this is swapped cause window coordinates are top left is 0,0 but we want bottom left to 
	// be zero zero 
	float y = RangeMapFloat(mousePositionInPixels.y, windowDimensions.y, 0.f, orthoBounds.mins.y, orthoBounds.maxs.y);

	return Vector2( x,y );
}
