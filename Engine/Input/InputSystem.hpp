#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class AABB2;

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

// Windows look up: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
// this is SDL2 look up https://wiki.libsdl.org/SDLKeycodeLookup
// all the f1 are max int numbers so I am making my own : (
constexpr KeyCode KEYBOARD_F1 = 0x80; // started at 0x70 but was bad so i am going up 1
constexpr KeyCode KEYBOARD_F2 = 0x81;
constexpr KeyCode KEYBOARD_F3 = 0x82;
constexpr KeyCode KEYBOARD_F4 = 0x83;
constexpr KeyCode KEYBOARD_F5 = 0x84;
constexpr KeyCode KEYBOARD_F6 = 0x85;
constexpr KeyCode KEYBOARD_F7 = 0x86;
constexpr KeyCode KEYBOARD_F8 = 0x87;
constexpr KeyCode KEYBOARD_F9 = 0x88;
constexpr KeyCode KEYBOARD_F10 = 0x89;
constexpr KeyCode KEYBOARD_F11 = 0x8A;
constexpr KeyCode KEYBOARD_F12 = 0x8B;

constexpr KeyCode KEYBOARD_SPACE = 0x20;
constexpr KeyCode KEYBOARD_LSHIFT = 0xA0;
constexpr KeyCode KEYBOARD_ENTER = 0x0D;
constexpr KeyCode KEYBOARD_BACKSPACE = 0x08;
constexpr KeyCode KEYBOARD_TILDE = 96;
constexpr KeyCode KEYBOARD_ESC = 0x1B;
constexpr KeyCode KEYBOARD_ALT = 0x12;
constexpr KeyCode KEYBOARD_CTRL = 0x11;

//====================================================================================
// ENUMS
//====================================================================================
enum MouseButtons
{
	LEFT_MOUSE_BUTTON,
	RIGHT_MOUSE_BUTTON,
	NUM_OF_MOUSE_BUTTONS
};
MouseButtons StringToMouseButtons(std::string string);
typedef bool(*MouseInputFunc)(MouseButtons);

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

private:
	void					OnMouseButtonPressed(MouseButtons theButton);
	void					OnMouseButtonReleased(MouseButtons theButton);

public:
	bool					IsMouseButtonPressed(MouseButtons theButton);
	bool					WasMouseButtonJustPressed(MouseButtons theButton);
	bool					WasMouseButtonJustReleased(MouseButtons theButton);
	bool					DidMouseWheelScrollUp();
	bool					DidMouseWheelScrollDown();
	Vector2					GetMousePositionInPixels() { return m_mousePosition; }
	void					HideMouseCursor(bool isHidden);

public:
	void	ClearKeyState(KeyCode keycode);
	void	ClearKeyboardState();

public:
	String					GetTextInputedThisFrame();
private:
	void					AddTextInputThisFrame(KeyCode keyCode);

protected:
	void					PollEvents();
	void					FilterKeysAndPassToDevConsole(int code);
	void					UpdateKeyboard();
	void					UpdateMouse();
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
	KeyButtonState			m_mouseButtonStates[NUM_OF_MOUSE_BUTTONS];

	Vector2					m_mouseScrollWheel;

	// note: this mouse position is based on pixel mouse position on the app
	// not based on the ortho
	Vector2					m_mousePosition;

	ShutdownFunction		m_shutdownFunction = nullptr;

	String					m_textInputed = "";
};

//====================================================================================
// Standalone C Functions
//====================================================================================
bool					IsKeyPressed(KeyCode keyCode);
bool					WasKeyJustPressed(KeyCode keyCode);
bool					WasKeyJustReleased(KeyCode keyCode);

bool					IsMouseButtonPressed(MouseButtons theButton);
bool					WasMouseButtonJustPressed(MouseButtons theButton);
bool					WasMouseButtonJustReleased(MouseButtons theButton);

bool					DidMouseWheelScrollUp();
bool					DidMouseWheelScrollDown();

Vector2					GetMousePosition(const AABB2& orthoBounds);

//====================================================================================
// Externs
//====================================================================================
extern InputSystem*		g_theInputSystem;


//====================================================================================
// Written by Zachary Bracken : [3/26/2019]
//====================================================================================