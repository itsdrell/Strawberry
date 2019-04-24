#include "Console.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/BitmapFont.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/EngineConsoleCommands.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

#include "Engine/ThirdParty/SDL2/SDL_keycode.h"

//===============================================================================================
Console* Console::s_instance = nullptr;

//===============================================================================================
Console::Console()
{
	m_windowWidth = Window::GetInstance()->GetWidth();
	m_windowHeight = Window::GetInstance()->GetWidth();

	m_fontSize = 4.f;

	StartUp();

	s_instance = this;
}

//-----------------------------------------------------------------------------------------------
Console::~Console()
{
	delete m_camera;
	m_camera = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Console::StartUp()
{
	Renderer* r = Renderer::GetInstance();

	m_consoleWindow = AABB2(Vector2(0.f, 0.f), Vector2(Window::GetInstance()->GetAspect(), 1));
	m_fontSize = m_consoleWindow.GetHeight() * (1.f / MAX_LINES_OF_TEXT_ON_SCREEN);

	m_fontWidth = r->m_defaultFont->GetWidthOfAGlyph(m_fontSize, Window::GetInstance()->GetAspect());
	m_maxLengthOfTextToShow = ((m_consoleWindow.GetWidth() / m_fontWidth) - 2.f);
	m_maxRowsOfText = ((m_consoleWindow.GetHeight() / m_fontSize) - 4.f);
	
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->m_projectionMatrix = Matrix44::MakeOrtho2D(m_consoleWindow.mins, m_consoleWindow.maxs);
	Matrix44 view = Matrix44::MakeView(Vector3(0.f, 0.f, -10.f), Vector3::ZERO);

	r->SetCamera();

	BindAllEngineCommands();
}

//-----------------------------------------------------------------------------------------------
void Console::Update()
{
	if (m_isOpen == false) return;

}

//-----------------------------------------------------------------------------------------------
void Console::Render() const
{
	if (m_isOpen == false) return;
	
	Renderer* r = Renderer::GetInstance();

	r->SetCamera(m_camera);

	r->DrawAABB2Filled(m_consoleWindow, Rgba(0,0,0,175));
	r->DrawAABB2Outline(m_consoleWindow, Rgba(0,0,0,255));

	RenderInputText();
	RenderInputBar();
	RenderHistory();
}

//-----------------------------------------------------------------------------------------------
void Console::RenderInputBar() const
{
	Renderer* r = Renderer::GetInstance();
	
	AABB2 inputBarBox;
	inputBarBox.mins.x = m_inputBarLocation.x + ((float)m_barIndex * m_fontWidth);
	inputBarBox.mins.y = m_inputBarLocation.y;

	inputBarBox.maxs.x = inputBarBox.mins.x + (.1f * m_fontWidth);
	inputBarBox.maxs.y = m_inputBarLocation.y + m_fontSize;

	float fract = GetFractionOf(g_theMasterClock->totalTime);
	float alphaAmount = RangeMapFloat(fract, 0.f, 1.f, 255.f, 0.f);
	r->DrawAABB2Filled(inputBarBox, Rgba(255,0,255, (unsigned char) alphaAmount));
}

//-----------------------------------------------------------------------------------------------
void Console::RenderInputText() const
{
	Renderer* r = Renderer::GetInstance();

	String inputToShow = m_currentEntry;

	if (m_currentEntry.length() > m_maxLengthOfTextToShow)
	{
		int startPos = ((int)inputToShow.length()) - ((int)m_maxLengthOfTextToShow);

		inputToShow = String(m_currentEntry, startPos, ((int) inputToShow.length()) - startPos);
	}
	
	r->DrawText2D(m_inputBarLocation, inputToShow, m_fontSize, Rgba::WHITE, Window::GetInstance()->GetAspect());
}

//-----------------------------------------------------------------------------------------------
void Console::RenderHistory() const
{
	if (m_history.size() == 0) return;

	Renderer* r = Renderer::GetInstance();

	// dont have to cap, could let it run off the screen
	int amountToPrint = (int) m_history.size();

	if (amountToPrint > (int)m_maxRowsOfText)
		amountToPrint = (int)m_maxRowsOfText;

	Vector2 startPos = m_historyStartAnchor;
	for (int i = amountToPrint; i > 0; i--)
	{
		ConsoleDialogue current = m_history.at(i - 1);
		r->DrawText2D(startPos, current.m_text, m_fontSize, current.m_color);

		startPos.y += (m_fontSize + (m_fontSize * .1f));
	}

}

//-----------------------------------------------------------------------------------------------
void Console::Open()
{
	m_isOpen = true;
}

//-----------------------------------------------------------------------------------------------
void Console::Close()
{
	m_isOpen = false;
	m_currentEntry.clear();
}

//-----------------------------------------------------------------------------------------------
void Console::Toggle()
{
	m_isOpen = !m_isOpen;

	if (m_isOpen) Open();
	else Close();
}

//-----------------------------------------------------------------------------------------------
void Console::GetInput(KeyCode theCode)
{
	if (m_isOpen == false) return;
	
	switch (theCode)
	{
	case SDLK_RETURN:
		ConsumeInput();
		break;
	case SDLK_BACKSPACE:
		RemoveACharacter();
		break;
	case KEYBOARD_LEFT_ARROW:
		if (m_barIndex > 0)
			m_barIndex--;
		break;
	case KEYBOARD_RIGHT_ARROW:
		if (!(m_barIndex >= m_currentEntry.size()))
			m_barIndex++;
		break;
	case SDLK_DELETE:
		DeleteKey();
		break;
	case SDLK_ESCAPE:
		break;
	case SDLK_BACKQUOTE:
		break;
	default:

		m_currentEntry.insert(m_currentEntry.begin() + m_barIndex, (const char)theCode);
		m_barIndex++;

		break;
	}

}

//-----------------------------------------------------------------------------------------------
void Console::ConsumeInput()
{
	if (m_currentEntry.length() == 0) return;

	AddACommandToHistory();

	bool check = CommandRun(m_currentEntry);

	if (check == false)
	{
		AddErrorMessage("Syntax Error");
	}
	else
	{
		if(m_currentEntry != "clear")
			AddConsoleDialogue(m_currentEntry, Rgba::WHITE);
	}

	m_barIndex = 0;
	m_currentEntry.clear();
}

//-----------------------------------------------------------------------------------------------
void Console::RemoveACharacter()
{
	// make sure we dont delete when there is nothing there
	if (m_currentEntry.length() <= 0) return;

	m_currentEntry.erase(m_currentEntry.begin() + m_barIndex - 1);
	m_barIndex--;
}

//-----------------------------------------------------------------------------------------------
void Console::DeleteKey()
{
}

//-----------------------------------------------------------------------------------------------
void Console::AddConsoleDialogue(const String & dialogue, const Rgba & color)
{
	ConsoleDialogue message = ConsoleDialogue(dialogue, color);
	
	m_history.push_back(message);
}

//-----------------------------------------------------------------------------------------------
void Console::AddErrorMessage(const String & message)
{
	AddConsoleDialogue(message, Rgba::GetColorByName("red"));
}

//-----------------------------------------------------------------------------------------------
void Console::AddACommandToHistory()
{
	if (m_currentEntry == "") return;

	m_commandHistory.push_back(m_currentEntry);
}

//-----------------------------------------------------------------------------------------------
void Console::ClearHistory()
{
	m_history.clear();
}

//===============================================================================================
// c functions
//===============================================================================================
void AddConsoleDialogue(const String & message, const Rgba & color)
{
	Console::GetInstance()->AddConsoleDialogue(message, color);
}

//-----------------------------------------------------------------------------------------------
void AddConsoleErrorMessage(const String & message)
{
	Console::GetInstance()->AddErrorMessage(message);
}
