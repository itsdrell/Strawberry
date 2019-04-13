#include "InputSystem.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/Tools/Console.hpp"

//===============================================================================================
InputSystem*		g_theInputSystem = nullptr;

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
	// nothing for now
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
bool InputSystem::IsKeyPressed(KeyCode keyCode) const
{
	return m_keyStates[keyCode].m_isDown;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasKeyJustPressed(KeyCode keyCode) const
{
	return m_keyStates[keyCode].m_wasJustPressed;
}

//-----------------------------------------------------------------------------------------------
bool InputSystem::WasKeyJustReleased(KeyCode keyCode) const
{
	return m_keyStates[keyCode].m_wasJustReleased;
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
			Console::GetInstance()->GetInput((KeyCode)theEvent.key.state);
			theEvent.text.text;

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

	return 0;
}

//===============================================================================================
bool IsKeyPressed(KeyCode keyCode)
{
	return InputSystem::GetInstance()->IsKeyPressed(keyCode);
}

//-----------------------------------------------------------------------------------------------
bool WasKeyJustPressed(KeyCode keyCode)
{
	return InputSystem::GetInstance()->WasKeyJustPressed(keyCode);
}

//-----------------------------------------------------------------------------------------------
bool WasKeyJustReleased(KeyCode keyCode)
{
	return InputSystem::GetInstance()->WasKeyJustReleased(keyCode);
}
