#include "InputSystem.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "../Core/Tools/ErrorWarningAssert.hpp"
#include "../Core/General/EngineCommon.hpp"

//===============================================================================================
InputSystem*		g_theInputSystem = nullptr;

//-----------------------------------------------------------------------------------------------
// https://wiki.libsdl.org/SDL_Keycode
const KeyCode KEYBOARD_SPACE			= SDLK_SPACE;
const KeyCode KEYBOARD_LSHIFT			= SDLK_LSHIFT;
const KeyCode KEYBOARD_ENTER			= SDLK_RETURN;
const KeyCode KEYBOARD_BACKSPACE		= SDLK_BACKSPACE;
const KeyCode KEYBOARD_LEFT_ARROW		= SDLK_LEFT;
const KeyCode KEYBOARD_RIGHT_ARROW		= SDLK_RIGHT;
const KeyCode KEYBOARD_UP_ARROW			= SDLK_UP;
const KeyCode KEYBOARD_DOWN_ARROW		= SDLK_DOWN;

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
			//String text = SDL_GetKeyName(theEvent.key.keysym.sym);
			OnKeyPressed((KeyCode) theEvent.key.keysym.sym);
		}

		if (theEvent.type == SDL_KEYUP)
		{
			OnKeyReleased((KeyCode)theEvent.key.keysym.sym);
		}
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
