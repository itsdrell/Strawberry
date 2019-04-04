#pragma once

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef int KeyCode;

typedef void(*ShutdownFunction)();

// these have a weird case leave me alone #todo
constexpr KeyCode KEYBOARD_LEFT_ARROW =		(const KeyCode) 0;
constexpr KeyCode KEYBOARD_RIGHT_ARROW =	(const KeyCode) 1;
constexpr KeyCode KEYBOARD_UP_ARROW =		(const KeyCode) 2;
constexpr KeyCode KEYBOARD_DOWN_ARROW =		(const KeyCode) 3;

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
	void					FilterKeysAndPassToDevConsole(int code);
	void					UpdateKeyboard();
	KeyCode					CheckAndCorrectSDLArrowKeys(KeyCode code);

public:
	void					SetShutdownFunction(ShutdownFunction theFunction) { m_shutdownFunction = theFunction; }
	void					CallQuitFunction() { m_shutdownFunction(); }

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

extern const KeyCode KEYBOARD_SPACE;
extern const KeyCode KEYBOARD_LSHIFT;
extern const KeyCode KEYBOARD_ENTER;
extern const KeyCode KEYBOARD_BACKSPACE;
extern const KeyCode KEYBOARD_TILDE;


//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================