#pragma once

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef unsigned char KeyCode;

typedef void(*ShutdownFunction)();

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct KeyButtonState
{
	bool m_isDown = false;
	bool m_wasJustPressed = false;
	bool m_wasJustReleased = false;
};

//====================================================================================
// Classes
//====================================================================================
class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	static InputSystem*		GetInstance();

public:
	void					BeginFrame();
	void					Update();
	void					EndFrame();

public:
	void					OnKeyPressed(KeyCode keyCode);
	void					OnKeyReleased(KeyCode keyCode);
	bool					IsKeyPressed(KeyCode keyCode) const;
	bool					WasKeyJustPressed(KeyCode keyCode) const;
	bool					WasKeyJustReleased(KeyCode keyCode) const;

protected:
	void					PollEvents();
	void					UpdateKeyboard();

public:
	void					SetShutdownFunction(ShutdownFunction theFunction) { m_shutdownFunction = theFunction; }

public:
	static const int		NUM_KEYS = 256; // Values match Windows VK_XXX virtual keys; common keys are 'A' and so on.

protected:
	KeyButtonState			m_keyStates[NUM_KEYS];

	ShutdownFunction		m_shutdownFunction = nullptr;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
bool					IsKeyPressed(KeyCode keyCode);
bool					WasKeyJustPressed(KeyCode keyCode);
bool					WasKeyJustReleased(KeyCode keyCode);

//====================================================================================
// Externs
//====================================================================================
extern InputSystem*		g_theInputSystem;

//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================