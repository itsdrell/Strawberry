#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Input/InputSystem.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Camera;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr float		MAX_LINES_OF_TEXT_ON_SCREEN = 25.f;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct ConsoleDialogue
{
	ConsoleDialogue(const String& whatWasEntered, const Rgba& theColor)
		: m_text(whatWasEntered)
		, m_color(theColor) {}

	ConsoleDialogue() {}

	String			m_text;
	Rgba			m_color;
};

//====================================================================================
// Classes
//====================================================================================
class Console
{
public:
	Console();
	~Console();

	void StartUp();

public:
	void Update();

public:
	void Render() const;
	void RenderInputBar() const;
	void RenderInputText() const;
	void RenderHistory() const;

public:
	void Open();
	void Close();
	void Toggle();
	bool IsOpen() { return m_isOpen; }

public:
	void GetInput(KeyCode theCode);
	void ConsumeInput();
	void RemoveACharacter();
	void DeleteKey();
	void AddConsoleDialogue(const String& dialogue, const Rgba& color = Rgba::WHITE);
	void AddErrorMessage(const String& message);
	void AddACommandToHistory();
	void ClearHistory();

public:
	static Console* GetInstance() { return s_instance; }

public:
	bool			m_isOpen = false;
	
private:
	Camera*			m_camera;
	String			m_currentEntry;

	float			m_windowWidth;
	float			m_windowHeight;

	AABB2			m_consoleWindow;
	Vector2			m_inputBarLocation = Vector2(0.05f, .05f);
	Vector2			m_historyStartAnchor = Vector2(.05f, .1f);

	float			m_fontSize;
	float			m_fontWidth;
	float			m_maxLengthOfTextToShow;
	float			m_maxRowsOfText;

	uint			m_barIndex = 0U;

private:
	std::vector<ConsoleDialogue>		m_history;
	Strings								m_commandHistory;

private:
	static Console*		s_instance;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/3/2019]
//====================================================================================