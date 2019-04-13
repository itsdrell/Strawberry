#pragma once

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef int KeyCode;

typedef void(*ShutdownFunction)();

// Defining the ascii version of all of these keys
constexpr KeyCode KEYBOARD_LEFT_ARROW =	0x25;
constexpr KeyCode KEYBOARD_RIGHT_ARROW = 0x27;
constexpr KeyCode KEYBOARD_UP_ARROW =	0x26;
constexpr KeyCode KEYBOARD_DOWN_ARROW =	0x28;

constexpr KeyCode KEYBOARD_F1 = 0x70;
constexpr KeyCode KEYBOARD_F2 = 0x71;
constexpr KeyCode KEYBOARD_F3 = 0x72;
constexpr KeyCode KEYBOARD_F4 = 0x73;
constexpr KeyCode KEYBOARD_F5 = 0x74;
constexpr KeyCode KEYBOARD_F6 = 0x75;
constexpr KeyCode KEYBOARD_F7 = 0x76;
constexpr KeyCode KEYBOARD_F8 = 0x77;
constexpr KeyCode KEYBOARD_F9 = 0x78;
constexpr KeyCode KEYBOARD_F10 = 0x79;
constexpr KeyCode KEYBOARD_F11 = 0x7A;
constexpr KeyCode KEYBOARD_F12 = 0x7B;

constexpr KeyCode KEYBOARD_SPACE = 0x20;
constexpr KeyCode KEYBOARD_LSHIFT = 0xA0;
constexpr KeyCode KEYBOARD_ENTER = 0x0D;
constexpr KeyCode KEYBOARD_BACKSPACE = 0x08;
constexpr KeyCode KEYBOARD_TILDE = 96;

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

	// this function is important because the function keys are really big numbers
	// so we need to look them up otherwise they go out of bounds in our array
	KeyCode					LookUpKeyCodeFromSDLKeySym(KeyCode code);

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


//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================